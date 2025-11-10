#pragma once

#include <cstdint>

namespace platform {

enum class InterruptMode : uint8_t {
    Change,
    Falling,
    Rising,
};

constexpr const char* toString(InterruptMode mode) {
    switch (mode) {
        case InterruptMode::Change:
            return "Change";
        case InterruptMode::Falling:
            return "Falling";
        case InterruptMode::Rising:
            return "Rising";
    }
}

}  // namespace platform
