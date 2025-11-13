#pragma once

#include <platform/io/button/Encoder.h>

using namespace platform;

namespace t {

enum Action : uint8_t {
    None,
    Right,
    Left,
};

struct Step {
    Action action = None;
    EncoderEvent expected = EncoderEvent::None;
    int8_t counter = 0;
    uint32_t delay_ms = 0;
};

auto right = [](int8_t counter) {
    return Step{
        .action = Right,
        .expected = EncoderEvent::TurnRight,
        .counter = counter,
        .delay_ms = 0,
    };
};

auto left = [](int8_t counter) {
    return Step{
        .action = Left,
        .expected = EncoderEvent::TurnLeft,
        .counter = counter,
        .delay_ms = 0,
    };
};

auto delay = [](int8_t counter, uint32_t ms, EncoderEvent ev) {
    return Step{
        .action = None,
        .expected = ev,
        .counter = counter,
        .delay_ms = ms,
    };
};

}  // namespace t
