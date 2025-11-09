#pragma once

#include "platform/sys/config.h"  // IWYU pragma: keep

#if defined(PLATFORM_ARDUINO)

#include "platform/hal/int/mode.h"

#include <Arduino.h>

#include <cstdint>

namespace platform {

enum class ButtonLevel : uint8_t {
    High = HIGH,
    Low = LOW,
};

constexpr uint8_t native(ButtonLevel level) {
    return static_cast<uint8_t>(level);
}

constexpr InterruptMode interruptMode(ButtonLevel level) {
    switch (level) {
        case ButtonLevel::High:
            return InterruptMode::Rising;
        case ButtonLevel::Low:
            return InterruptMode::Falling;
    }
}

}  // namespace platform

#endif
