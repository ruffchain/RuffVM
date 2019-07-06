#include "run_async.h"
#include "ruffvm.h"
#include "callback.h"
#include "nan_addon.h"

#include "Queue.hpp"

#include <string>
#include <vector>

using namespace v8;

namespace {

using nan_addon::PersistentCallback;

// Forward declaration for APICallbackSignaling destructor.
void cleanupUvAsync(uv_handle_s* handle);

//PolyM::Queue queue;

// Forward declaration for CallbackHelper.
NAN_ADDON_UV_ASYNC_CB(callV8FunctionOnMainThread);

struct WorkRequest
{
    WorkRequest(const std::string& script, const std::string& context, uint32_t cpuCount, uint32_t memSizeKB, Local<Function> callback):
     script(std::move(script))
    ,context(std::move(context))
    ,vm(ruffvm::RuffVM(cpuCount, memSizeKB))
    ,callback(callback)
    ,hasError(false)
    {
    };

    ~WorkRequest()
    {
        callback.Reset();
    }

    // in
    std::string script;
    std::string context;
    ruffvm::RuffVM vm;
	ruffvm::VMAddOn vmAddon;
    // out
    Nan::Persistent<Function> callback;
    bool hasError;
    std::unique_ptr<bridge::VMPacket> pReturnValue;
};

struct ScopedUvWorkRequest
{
    explicit ScopedUvWorkRequest(uv_work_t* work):
     m_work(work)
    ,m_workRequest(static_cast<WorkRequest*> (m_work->data))
    {
    }

    ~ScopedUvWorkRequest()
    {
        delete m_workRequest;
        delete m_work;
    }

    WorkRequest* getWorkRequest()
    {
        return m_workRequest;
    }

private:
    uv_work_t* m_work;
    WorkRequest* m_workRequest;
};

struct APICallbackSignaling
{

    APICallbackSignaling(const PersistentCallback& cb,
						 ruffvm::VMAddOn* pVmAddOn,
						 const bridge::VMPackets& parameter,
                         std::unique_ptr<bridge::VMPacket> pRetVal,
						 uv_async_cb cbFunc):
     callback(cb)
    ,pVmAddOn(pVmAddOn)
    ,parameter(parameter)
    ,pRetVal(std::move(pRetVal))
    ,done(false)
    ,isPromise(false)
    ,cbFunc(cbFunc)
    ,async(new uv_async_t)
    {
        uv_mutex_init(&mutex);
        uv_cond_init(&cv);
        uv_async_init(uv_default_loop(), async, cbFunc);
    }

    ~APICallbackSignaling()
    {
        uv_mutex_destroy(&mutex);
        uv_cond_destroy(&cv);
        uv_close((uv_handle_t*) async, &cleanupUvAsync);
    }

    APICallbackSignaling(const APICallbackSignaling&) = delete;
    APICallbackSignaling& operator=(const APICallbackSignaling&) = delete;
    const PersistentCallback& callback;
    ruffvm::VMAddOn* pVmAddOn;
    const bridge::VMPackets& parameter;
    std::unique_ptr<bridge::VMPacket> pRetVal;

    bool done;
    bool isPromise;
    uv_cond_t cv;
    uv_mutex_t mutex;
    uv_async_cb cbFunc;

    // Has to be on heap, because of closing logic.
    uv_async_t* async;
};

struct CallbackHelper
{
    CallbackHelper(Local<Function> apiCallbackFunc, ruffvm::VMAddOn* pVmAddOn):
    m_persistentApiCallbackFunc(apiCallbackFunc), pVmAddOn(pVmAddOn)
    {
        PLOG(plog::info) << "CallbackHelper Enter: " << this;
    }
    ~CallbackHelper()
    {
        PLOG(plog::info) << "CallbackHelper Exit: " << this;
        m_persistentApiCallbackFunc.Reset();
    }

    std::unique_ptr<bridge::VMPacket> operator()(const bridge::VMPackets& param)
    {
        // We're on not on libuv/V8 main thread. Signal main to run
        // callback function and wait for an answer.

        APICallbackSignaling cbsignaling(m_persistentApiCallbackFunc,
                                         pVmAddOn,
                                         param,
                                         std::make_unique<bridge::VMPacket>(),
                                         callV8FunctionOnMainThread);

        uv_mutex_lock(&cbsignaling.mutex);

        cbsignaling.async->data = (void*) &cbsignaling;
        uv_async_send(cbsignaling.async);
        while(!cbsignaling.done)
        {
            uv_cond_wait(&cbsignaling.cv, &cbsignaling.mutex);
        }
        uv_mutex_unlock(&cbsignaling.mutex);

        if (cbsignaling.isPromise && pVmAddOn) {
            auto m = pVmAddOn->m_queue.get();
            auto& dm = static_cast<PolyM::DataMsg<std::unique_ptr<bridge::VMPacket>>&>(*m);
            return std::move(dm.getPayload());
        } else {
            return std::move(cbsignaling.pRetVal);
        }
    }

private:
    PersistentCallback m_persistentApiCallbackFunc;
	ruffvm::VMAddOn* pVmAddOn;
};

void cleanupUvAsync(uv_handle_s* handle) {
    // "handle" is "async"-parameter passed to uv_close
    delete (uv_async_t*) handle;
}

void resolveFunction(const FunctionCallbackInfo<v8::Value>& info) {
    // bool isError = false;
    if (info.Length() == 0) {
        return;
    }

    // if (info.Length() == 2 && info[1]->IsBoolean()) {
    //     isError = Nan::To<bool>(info[1]).FromJust();
    // }
    auto pVmAddOn = reinterpret_cast<ruffvm::VMAddOn*>(info.Data().As<External>()->Value());
    auto pVMPacket = std::make_unique<bridge::VMPacket>();
    pVMPacket->from(info[0]);
    pVmAddOn->m_queue.put(PolyM::DataMsg<std::unique_ptr<bridge::VMPacket>>(1, std::move(pVMPacket)));

    return;
}

#define RUFFVM_MAX_PARAM_TOV8 8
NAN_ADDON_UV_ASYNC_CB(callV8FunctionOnMainThread) {
    auto signalData = static_cast<APICallbackSignaling*> (handle->data);
    uv_mutex_lock(&signalData->mutex);

    Nan::HandleScope scope;
    Local<Value> argv[RUFFVM_MAX_PARAM_TOV8];
    Isolate *isolate = Isolate::GetCurrent();

    Local<External> external = External::New(isolate, signalData->pVmAddOn);
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, resolveFunction, external);
    int argc = 1;
    argv[0] = tpl->GetFunction();

    for (auto iter = signalData->parameter.begin();
            iter != signalData->parameter.end() && argc < RUFFVM_MAX_PARAM_TOV8; iter++,argc++) {
	    const bridge::VMPacket* pVmPacket = *iter;
        argv[argc] = pVmPacket->toV8Value(isolate);
    }

    Nan::TryCatch try_catch;
    auto callbackHandle = Nan::New<Function>(signalData->callback);
    auto retVal = callbackHandle->Call(Nan::GetCurrentContext()->Global(), argc, argv);

    if (try_catch.HasCaught()) {
        String::Utf8Value stack_trace(try_catch.StackTrace().ToLocalChecked());
        if (stack_trace.length() > 0) {
            PLOG(plog::error) << *stack_trace;
        }
        signalData->pRetVal->from(bridge::HostException);
    } else {

        if (retVal->IsPromise()) {
            signalData->isPromise = true;
        } else {
            signalData->pRetVal->from(retVal);
        }
    }

    signalData->done = true;
    uv_cond_signal(&signalData->cv);
    uv_mutex_unlock(&signalData->mutex);
}

void onWork(uv_work_t* req)
{
    // Do not use scoped-wrapper as req is still needed in onWorkDone.
    WorkRequest* work = static_cast<WorkRequest*> (req->data);

    PLOG(plog::info) << "Before rm run";
    work->pReturnValue = work->vm.run(work->script, work->context.c_str(), work->context.length());
    PLOG(plog::info) << "after rm run";
    work->hasError = work->pReturnValue->type() == bridge::VMError;
}

void onWorkDone(uv_work_t* req, int status)
{
    ScopedUvWorkRequest uvReq(req);
    WorkRequest* work = uvReq.getWorkRequest();
    Isolate *isolate = Isolate::GetCurrent();

    Nan::HandleScope scope;
    Handle<Value> argv[2];
    if (work->pReturnValue->type() == bridge::VMError) {
        argv[0] = work->pReturnValue->toV8Value(isolate);
        argv[1] = Nan::Null();
    } else {
        argv[0] = Nan::Null();
        argv[1] = work->pReturnValue->toV8Value(isolate);
    }

    Nan::TryCatch try_catch;
    auto callbackHandle = Nan::New(work->callback);
    callbackHandle->Call(Nan::GetCurrentContext()->Global(), 2, argv);

    if (try_catch.HasCaught())
    {
        Nan::FatalException(try_catch);
    }
    PLOG(plog::info) << "Exit";
}

} // unnamed namespace

namespace ruffvm {

NAN_METHOD(run) {
    Nan::Utf8String script(info[0]);
    std::string context;
    uint32_t cpuTime = 0xFFFFFFFF;
    uint32_t memSizeKB = 128;

    PLOG(plog::info) << "module run Enter";

    if (info[1]->IsArrayBufferView()) {
        Nan::TypedArrayContents<uint8_t> data(info[1]);
        context.assign((const char *)(*data), data.length());
    } else {
        context = "";
    }

    if (info[3]->IsObject()) {
        auto object = Handle<Object>::Cast(info[3]);
        Local<String> cpuTimeKey = Nan::New("cpuCount").ToLocalChecked();
        Local<String> memSizeKey = Nan::New("memSizeKB").ToLocalChecked();

        Local<Value> cpuCountValue = Nan::Get(object, cpuTimeKey).ToLocalChecked();
        Local<Value> memSizeKBValue = Nan::Get(object, memSizeKey).ToLocalChecked();
        if (cpuCountValue->IsNumber() && memSizeKBValue->IsNumber()) {
            cpuTime = Nan::To<uint32_t>(cpuCountValue).FromJust();
            memSizeKB = Nan::To<uint32_t>(memSizeKBValue).FromJust();

        } else {
            Nan::ThrowError(Nan::New("Error in resource limit control").ToLocalChecked());
            return;
        }
    }

    auto workReq = new WorkRequest(std::string(*script),
                                   context,
                                   cpuTime,
                                   memSizeKB,
                                   Local<Function>::Cast(info[4]));

    // API Handling
    if (info[2]->IsObject()) {
        auto object = Handle<Object>::Cast(info[2]);
        auto properties = object->GetPropertyNames();

        auto len = properties->Length();
        for(unsigned int i = 0; i < len; ++i) {
            Local<Value> key = properties->Get(i);
            Local<Value> value = object->Get(key);
            if (!key->IsString() || !value->IsFunction()) {
                Nan::ThrowError(Nan::New("Error in API-definition").ToLocalChecked());
                return;
            }

            Nan::Utf8String  keyStr(key);
            CallbackHelper vmToNodeBridge(Local<Function>::Cast(value), &(workReq->vmAddon));
            PLOG(plog::debug) << "vmToNodeBridge is" << &vmToNodeBridge;
            workReq->vm.registerCallback(std::string(*keyStr), vmToNodeBridge);
        }
    }


    uv_work_t* req = new uv_work_t();
    req->data = workReq;

    PLOG(plog::info) << "request work";
    uv_queue_work(uv_default_loop(), req, onWork, onWorkDone);

    return;
}

} // namespace ruffvm
