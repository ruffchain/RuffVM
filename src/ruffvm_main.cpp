#include "ruffvm.h"
#include "run_async.h"
#include <nan.h>
#include <cstdlib>

namespace {

NAN_MODULE_INIT(init)
{
    using Nan::GetFunction;
    using Nan::New;
    using Nan::Set;
    using v8::String;
    using v8::FunctionTemplate;

    const char* level = std::getenv("RUFFVM_LOG_LEVEL");
    const char* fileName = std::getenv("RUFFVM_LOG_FILE");

    if (level) {
        plog::Severity severity;
        if (!fileName) {
            fileName = "ruffvm_log.txt";
        }
        plog::init(plog::severityFromString(level), fileName, 1000000, 5);
    }

    PLOG(plog::info) << "RuffVM Module Enter";
    Set(target,
        New<String>("run").ToLocalChecked(),
        GetFunction(New<FunctionTemplate>(ruffvm::run)).ToLocalChecked());
}

} // unnamed namespace

NODE_MODULE(ruffvm, init)
