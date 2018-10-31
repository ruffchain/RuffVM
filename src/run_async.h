#pragma once
#include <nan.h>
#include "Queue.hpp"

namespace ruffvm {

struct VMAddOn {
    VMAddOn() {
    }

    ~VMAddOn() {
    }
    PolyM::Queue m_queue;
};

NAN_METHOD(run);

} // namespace ruffvm
