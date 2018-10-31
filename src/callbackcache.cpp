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

void CallbackCache::registerContext(jerry_context_t* ctx)
{
    m_callbackCache[ctx];
}

void CallbackCache::unregisterContext(jerry_context_t* ctx)
{
    auto context = m_callbackCache.find(ctx);
    if(context != m_callbackCache.end())
    {
        m_callbackCache.erase(context);
    }
}

void CallbackCache::addCallback(jerry_context_t* ctx, const std::string& functionName, ruffvm::Callback callbackFunc)
{
    auto context = m_callbackCache.find(ctx);
    if(context != m_callbackCache.end())
    {
        auto callback = context->second.find(functionName);
        if(callback == context->second.end())
        {
            context->second[functionName] = callbackFunc;
        }
    }
}

std::unique_ptr<bridge::VMPacket> CallbackCache::doCallbackToV8(jerry_context_t* ctx,
										  const std::string& callbackName,
										  const bridge::VMPacket& parameter)
{
    auto context = m_callbackCache.find(ctx);
    if(context != m_callbackCache.end())
    {
        auto callback = context->second.find(callbackName);
        if(callback != context->second.end())
        {
            return callback->second(parameter);
        }
    }
    return std::unique_ptr<bridge::VMPacket>(nullptr);
}

} // namespace ruffvm
