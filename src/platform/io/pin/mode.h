#pragma once

#include "platform/sys/config.h"  // IWYU pragma: keep

#if defined(PLATFORM_ARDUINO)

#include <Arduino.h>

#include <supp/verify.h>

#include <cstdint>

namespace platform {

enum class GPIOMode : uint8_t {
    Input = INPUT,
    InputPullup = INPUT_PULLUP,
    Output = OUTPUT,
    Adc = 255,
};

constexpr uint8_t native(GPIOMode mode) {
    DASSERT(mode != GPIOMode::Adc);
    return static_cast<uint8_t>(mode);
}

constexpr bool isDigitalOutput(GPIOMode mode) {
    switch (mode) {
        case GPIOMode::Output:
            return true;
        default:
            return false;
    }
}

constexpr bool isOutput(GPIOMode mode) {
    switch (mode) {
        case GPIOMode::Output:
            return true;
        default:
            return false;
    }
}

constexpr bool isDigitalInput(GPIOMode mode) {
    switch (mode) {
        case GPIOMode::Input:
        case GPIOMode::InputPullup:
            return true;
        default:
            return false;
    }
}

constexpr bool isInput(GPIOMode mode) {
    switch (mode) {
        case GPIOMode::Input:
        case GPIOMode::InputPullup:
        case GPIOMode::Adc:
            return true;
        default:
            return false;
    }
}

}  // namespace platform

#endif
