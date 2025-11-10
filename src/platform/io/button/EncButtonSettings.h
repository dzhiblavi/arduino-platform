#pragma once

#include "platform/io/button/ButtonSettings.h"
#include "platform/io/button/EncoderSettings.h"

namespace platform {

struct EncButtonSettings {
    ButtonSettings button{};
    EncoderSettings encoder{};
};

}  // namespace platform
