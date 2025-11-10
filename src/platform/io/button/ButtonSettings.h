#pragma once

#include "platform/io/button/ButtonLevel.h"

namespace platform {

struct ButtonSettings {
    uint16_t hold_ms = 500;
    uint16_t timeout_ms = 500;  // after the last release
    uint8_t debounce_ms = 50;
    ButtonLevel level = ButtonLevel::High;
};

}  // namespace platform
