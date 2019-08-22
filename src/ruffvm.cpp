#include <iostream>

#include "ruffvm.h"
#include "bridge.h"
#include "snapshot.h"
#include "callbackcache.h"
#include "jerryscript.h"
#include "jerryscript-port.h"
#include "jerryscript-port-default.h"
#include "jerryscript-ext/handler.h"

namespace {

using ruffvm::Result;

static ruffvm::CallbackCache cbCache;

Result ruffvm_jerry_string_value_to_string(jerry_value_t value, const char* buffer, size_t length, bool isUft8=true)
{
    Result ret;
    jerry_size_t req_sz;

    if (jerry_value_is_string(value)) {
        req_sz = jerry_get_utf8_string_size(value);
        if (req_sz >= length) {
            ret.errorCode = 2; // exceed max
        } else {
            jerry_string_to_utf8_char_buffer(value, (jerry_char_t*)buffer, req_sz);
            ret.value = std::string(buffer);
        }
    } else {
        ret.errorCode = 1; // invalid parameter
    }
    return ret;
}

#define RUFFVM_MAX_FUNC_NAME_SIZE   128
static jerry_value_t
callbackHandler(const jerry_value_t function_obj,
                const jerry_value_t this_val,
                const jerry_value_t args_p[],
                const jerry_length_t args_cnt)
{
    jerry_value_t prop_name = jerry_create_string((const jerry_char_t *) "__callbackName");
    jerry_value_t prop_value = jerry_get_property(function_obj, prop_name);
    jerry_char_t str_buf_p[RUFFVM_MAX_FUNC_NAME_SIZE + 1] = { 0 };
    jerry_value_t ret_val;
    Result func_name;
    bridge::VMPackets params;

    if (jerry_value_is_string(prop_value)) {
        func_name = ruffvm_jerry_string_value_to_string(prop_value,
                                                        (const char*)str_buf_p,
                                                        RUFFVM_MAX_FUNC_NAME_SIZE);
        if (func_name.errorCode == 0) {
            std::string callbackName(func_name.value);
            if (args_cnt > 0) {
                for (jerry_length_t index = 0; index < args_cnt; index++) {
                    auto pPacket = new bridge::VMPacket();
                    pPacket->from(args_p[index]);
                    params.push_back(pPacket);
                    if (pPacket->type() == bridge::PacketFailToParse) {
				        ret_val = jerry_create_undefined();
				        goto clean;
			        }
                }
            } else {
                auto pPacket = new bridge::VMPacket();
                jerry_value_t undefined_value = jerry_create_undefined();
                pPacket->from(undefined_value);
                jerry_release_value(undefined_value);
                params.push_back(pPacket);
            }

            jerry_context_t *ctx = jerry_port_get_current_context();
            auto pVMPacket = cbCache.doCallbackToV8(ctx, callbackName, params);

            if (pVMPacket) {
                ret_val = pVMPacket->toJerryValue();
            } else {
                ret_val = jerry_create_undefined();
            }
        } else {
            //Todo: return error object to Jerry
            PLOG(plog::error) << "exceed max name size" << RUFFVM_MAX_FUNC_NAME_SIZE;
            ret_val = jerry_create_undefined();
        }
    } else {
        ret_val = jerry_create_undefined();
    }

clean:
    for (auto iter = params.begin(); iter != params.end(); iter++) {
        delete(*iter);
        *iter = nullptr;
    }

    // do clean here
    jerry_release_value(prop_name);
    jerry_release_value(prop_value);
    return ret_val;
}

static void
ruffvm_register_js_function(const jerry_char_t* name_p, /**< name of the function */
                            jerry_external_handler_t handler_p) /**< function callback */
{
    jerry_value_t global_obj_val = jerry_get_global_object();
    jerry_value_t function_name_val = jerry_create_string(name_p);
    jerry_value_t function_val = jerry_create_external_function(handler_p);

    jerry_value_t prop_name = jerry_create_string((const jerry_char_t*) "__callbackName");
    jerry_value_t prop_value = jerry_create_string(name_p);
    jerry_value_t result_val = jerry_set_property(function_val, prop_name, prop_value);

    // ToDo: finish sanity check here
    if (jerry_value_is_error(result_val)) {
        PLOG(plog::error) << "Error!!! failed to set prop_name";
    }
    jerry_release_value(prop_name);
    jerry_release_value(prop_value);
    jerry_release_value(result_val);

    result_val = jerry_set_property(global_obj_val, function_name_val, function_val);

    if (jerry_value_is_error(result_val)) {
        PLOG(plog::error) << "Error!!! failed to set prop_name";
    }

    jerry_release_value(function_val);
    jerry_release_value(function_name_val);
    jerry_release_value(global_obj_val);
    jerry_release_value(result_val);
    return;
}


static void *
context_alloc (size_t size,
               void *cb_data_p)
{
    //struct ruffvm::VmInfo* pVmInfo = (struct ruffvm::VmInfo*)cb_data_p;
    return malloc (size);
} /* context_alloc */

} // unnamed namespace

static jerry_value_t
vm_exec_stop_callback (void *user_p)
{
    ruffvm::VmInfo* pInfo = (ruffvm::VmInfo*)user_p;

    PLOG(plog::debug) << "Enter" << pInfo->cpuCount;
    while (pInfo->cpuCount != 0) {
        pInfo->cpuCount--;
        return jerry_create_undefined ();
    }

    PLOG(plog::info) << "Exit";
    // The error flag is added automatically.
    return jerry_create_string ((const jerry_char_t *) "{\"error\": \"Abort script\"}");
}


namespace ruffvm {
RuffVM::RuffVM(uint32_t cpuCount, uint32_t memSizeKB)
{
    PLOG(plog::info) << "Enter";
    if (memSizeKB > 512) {
        memSizeKB = 512;
    }
    m_vmInfo.memSizeMax = memSizeKB * 1024;
    m_vmInfo.memSizeUsed = 0;
    m_vmInfo.defInnerCpuCycle = 8;
    m_vmInfo.cpuCount = cpuCount;
    m_ctx = jerry_create_context(memSizeKB * 1024, context_alloc, &m_vmInfo);
    cbCache.registerContext(m_ctx, &m_vmInfo);
    jerry_port_default_set_current_context(m_ctx);
    jerry_init(JERRY_INIT_EMPTY);
    //jerryx_handler_register_global ((const jerry_char_t *) "print",
    //                              jerryx_handler_print);
    if (m_vmInfo.cpuCount != 0xFFFFFFFF) {
        jerry_set_vm_exec_stop_callback (vm_exec_stop_callback, &m_vmInfo, 2);
    }
    snapshotLoad();
}

RuffVM::~RuffVM()
{
    PLOG(plog::info) << "Exit";
    cbCache.unregisterContext(m_ctx);
    jerry_cleanup();
    free(m_ctx);
}

std::unique_ptr<bridge::VMPacket> RuffVM::run(const std::string& script, const char* context, uint32_t context_size)
{
    Result ret;
    jerry_value_t contextRet;
    auto pVMPacket = std::make_unique<bridge::VMPacket>();

    PLOG(plog::info) << "Enter";
    if (context_size != 0) {
        contextRet = jerry_eval((const jerry_char_t*)context, context_size, JERRY_PARSE_STRICT_MODE);
    }
    if (context_size == 0 || !jerry_value_is_error(contextRet)) {
        jerry_value_t scriptRet = jerry_eval((const jerry_char_t*)script.c_str(),
                                             script.length(),
                                             JERRY_PARSE_STRICT_MODE);
        pVMPacket->from(scriptRet);
        jerry_release_value(scriptRet);
    } else {
        PLOG(plog::error) << "error when eval context";
        pVMPacket->from(bridge::InvalidScript);
    }

    if (context_size) {
        jerry_release_value(contextRet);
    }
    PLOG(plog::info) << "Exit";
    return pVMPacket;
}

void RuffVM::registerCallback(const std::string& functionName, Callback callback)
{
    ruffvm_register_js_function((const jerry_char_t*)functionName.c_str(), callbackHandler);
    cbCache.addCallback(m_ctx, functionName, std::forward<Callback>(callback));
}

} // namespace ruffvm
