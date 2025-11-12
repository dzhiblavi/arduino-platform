#pragma once

#include "platform/sys/config.h"  // IWYU pragma: keep

#include "platform/hal/int/frameworks/arduino/mode.h"
#include "platform/hal/int/vector.h"

#include <supp/no_match.h>

namespace platform {

template <uint8_t IntNo>
struct Interrupt {
    constexpr uint8_t number() const { return IntNo; }

    void attach(InterruptMode mode) const {
        ::attachInterrupt(IntNo, &::platform::detail::_vector<IntNo>, native(mode));
    }

    void detach() const { ::detachInterrupt(IntNo); }
};

}  // namespace platform
