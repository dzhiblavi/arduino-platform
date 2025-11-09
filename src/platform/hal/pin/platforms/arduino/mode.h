#pragma once

#include "platform/sys/config.h"  // IWYU pragma: keep

#if defined(PLATFORM_ARDUINO)

#include "platform/hal/pin/mode.h"

#include <Arduino.h>

namespace platform {

constexpr uint8_t native(GPIOMode mode) {
    switch (mode) {
        case GPIOMode::Input:
            return INPUT;
        case GPIOMode::InputPullup:
            return INPUT_PULLUP;
        case GPIOMode::Output:
            return OUTPUT;
    }
}

}  // namespace platform

#endif
