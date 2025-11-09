#pragma once

#include "platform/sys/config.h"  // IWYU pragma: keep

#if defined(PLATFORM_ARDUINO)

#include "platform/hal/int/frameworks/arduino/mode.h"

#include "platform/hal/int/type.h"

#include <Arduino.h>

namespace platform {

template <uint8_t IntNo>
struct Interrupt {
    void attach(type::InterruptFunc func, InterruptMode mode) const {
        ::attachInterrupt(IntNo, func, native(mode));
    }

    void detach() const { ::detachInterrupt(IntNo); }
};

}  // namespace platform

#endif
