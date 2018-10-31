#include "run_async.h"
#include <nan.h>

namespace {

NAN_MODULE_INIT(init)
{
    using Nan::GetFunction;
    using Nan::New;
    using Nan::Set;
    using v8::String;
    using v8::FunctionTemplate;

    Set(target,
        New<String>("run").ToLocalChecked(),
        GetFunction(New<FunctionTemplate>(ruffvm::run)).ToLocalChecked());
}

} // unnamed namespace

NODE_MODULE(ruffvm, init)
