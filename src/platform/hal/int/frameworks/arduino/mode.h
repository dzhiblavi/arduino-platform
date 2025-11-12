#pragma once

#include "platform/hal/int/mode.h"

#include <Arduino.h>

namespace platform {

constexpr uint8_t native(InterruptMode mode) {
    switch (mode) {
        case InterruptMode::Change:
            return CHANGE;
        case InterruptMode::Rising:
            return RISING;
        case InterruptMode::Falling:
            return FALLING;
    }

    __builtin_unreachable();
}

}  // namespace platform
