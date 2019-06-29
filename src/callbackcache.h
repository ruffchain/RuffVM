#pragma once

#include "callback.h"
#include "bridge.h"
#include "ruffvm.h"

#include "jerryscript.h"
#include <string>
#include <map>

namespace ruffvm {

class CallbackCache
{
public:
    CallbackCache();
    ~CallbackCache();
    void registerContext(jerry_context_t* ctx, VmInfo* pVmInfo);
    void unregisterContext(jerry_context_t* ctx);
    void addCallback(jerry_context_t* ctx, const std::string& functionName, ruffvm::Callback callbackFunc);
    std::unique_ptr<bridge::VMPacket> doCallbackToV8(jerry_context_t* ctx,
                                                     const std::string& callbackName,
                                                     const bridge::VMPackets& parameter);

private:
    /*
    Key: pointer to context
    Value: Map of function name and callback
    */
    typedef std::map<void*, std::map<std::string, ruffvm::Callback> > CallbackContainer;
    typedef std::map<void*, VmInfo*> VmInfoContainer;
    CallbackContainer m_callbackCache;
    VmInfoContainer m_vmInfoTable;
};

} // namespace ruffvm
