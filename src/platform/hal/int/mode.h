#pragma once

#include <cstdint>

namespace platform {

enum class InterruptMode : uint8_t {
    Change,
    Falling,
    Rising,
};

}  // namespace platform
