#pragma once

#include "platform/io/button/ButtonSettings.h"
#include "platform/io/button/EncoderSettings.h"

namespace platform {

struct EncButtonSettings : ButtonSettings, EncoderSettings {};

}  // namespace platform
