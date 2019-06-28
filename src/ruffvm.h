#pragma once

#include "callback.h"
#include "bridge.h"
#include "plog/Log.h"

#include "jerryscript.h"
#include <string>

namespace ruffvm {

struct Result
{
    Result(): errorCode(0) {}
    int errorCode;
    std::string value;
};

struct MemInfo {
    uint32_t memSizeMax;
    uint32_t memSizeUsed;
};

class RuffVM
{
public:
    RuffVM(uint32_t cpuCount, uint32_t memSizeKB);
    ~RuffVM();

    std::unique_ptr<bridge::VMPacket> run(const std::string& script, const char* context, uint32_t context_size);
    void registerCallback(const std::string& functionName, Callback callback);

private:
    jerry_context_t* m_ctx;
    uint32_t m_cpuCount;
    struct MemInfo m_memInfo;
};

}  // namespace ruffvm
