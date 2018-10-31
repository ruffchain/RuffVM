#pragma once

#include <functional>
#include <string>
#include "bridge.h"
namespace ruffvm {

typedef std::function<std::unique_ptr<bridge::VMPacket>(const bridge::VMPacket&)> Callback;

} // namespace ruffvm
