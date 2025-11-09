#pragma once

#include "platform/sys/config.h"  // IWYU pragma: keep

#include "platform/hal/int/mode.h"

#include <cstdint>

namespace platform {

enum class ButtonLevel : uint8_t {
    High,
    Low,
};

constexpr InterruptMode interruptMode(ButtonLevel level) {
    switch (level) {
        case ButtonLevel::High:
            return InterruptMode::Rising;
        case ButtonLevel::Low:
            return InterruptMode::Falling;
    }
}

}  // namespace platform
