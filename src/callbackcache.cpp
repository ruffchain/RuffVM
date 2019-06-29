#include "callbackcache.h"
#include "bridge.h"

namespace ruffvm
{

CallbackCache::CallbackCache()
{
}

CallbackCache::~CallbackCache()
{
}

void CallbackCache::registerContext(jerry_context_t* ctx, VmInfo* pVmInfo)
{
    m_callbackCache[ctx];
    m_vmInfoTable[ctx] = pVmInfo;
}

void CallbackCache::unregisterContext(jerry_context_t* ctx)
{
    auto context = m_callbackCache.find(ctx);
    if (context != m_callbackCache.end()) {
        m_callbackCache.erase(context);
    }
    auto vmInfo = m_vmInfoTable.find(ctx);
    if (vmInfo != m_vmInfoTable.end()) {
        m_vmInfoTable.erase(vmInfo);
    }
}

void CallbackCache::addCallback(jerry_context_t* ctx, const std::string& functionName, ruffvm::Callback callbackFunc)
{
    auto context = m_callbackCache.find(ctx);
    PLOG(plog::debug) << "callback is" << &callbackFunc;
    if (context != m_callbackCache.end()) {
        auto callback = context->second.find(functionName);
        if (callback == context->second.end()) {
            context->second[functionName] = std::move(callbackFunc);
        }
    }
}

std::unique_ptr<bridge::VMPacket> CallbackCache::doCallbackToV8(jerry_context_t* ctx,
										  const std::string& callbackName,
										  const bridge::VMPackets& parameter)
{
    auto context = m_callbackCache.find(ctx);
    VmInfo* pVmInfo = m_vmInfoTable[ctx];
    if(context != m_callbackCache.end()) {
        auto callback = context->second.find(callbackName);
        if (callback != context->second.end()) {
            auto ret = callback->second(parameter);
            if (pVmInfo) {
                auto count = pVmInfo->defInnerCpuCycle;
                if (pVmInfo->cpuCount > count) {
                    pVmInfo->cpuCount -= count;
                } else {
                    pVmInfo->cpuCount = 0;
                }
            }
            return ret;
        }
    }
    return std::unique_ptr<bridge::VMPacket>(nullptr);
}

} // namespace ruffvm
