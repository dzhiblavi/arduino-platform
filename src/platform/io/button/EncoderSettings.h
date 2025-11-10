#pragma once

#include <cstdint>

namespace platform {

enum EncoderType : uint8_t {
    Step4Low,
    Step4High,
    Step2,
    Step1,
};

struct EncoderSettings {
    EncoderType type = EncoderType::Step4Low;
};

}  // namespace platform
