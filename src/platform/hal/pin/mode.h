#pragma once

#include <cstdint>

namespace platform {

enum class GPIOMode : uint8_t {
    Input,
    InputPullup,
    Output,
};

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
            return true;
        default:
            return false;
    }
}

}  // namespace platform
