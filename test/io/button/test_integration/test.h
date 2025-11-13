#pragma once

#include <platform/io/button/Button.h>

using namespace platform;

namespace t {

enum Action : uint8_t {
    None,
    Press,
    Release,
    Suspend,
};

struct Step {
    Action action = None;
    ButtonEvent expected = ButtonEvent::None;
    uint32_t delay_ms = 0;
    uint8_t clicks = 0;
};

auto press = [](uint8_t clicks) {
    return Step{
        .action = Press,
        .expected = ButtonEvent::Pressed,
        .delay_ms = 0,
        .clicks = clicks,
    };
};

auto release = [](uint8_t clicks, ButtonEvent ev = ButtonEvent::Clicked) {
    return Step{
        .action = Release,
        .expected = ev,
        .delay_ms = 0,
        .clicks = clicks,
    };
};

auto delay = [](uint8_t clicks, uint32_t ms, ButtonEvent ev = ButtonEvent::None) {
    return Step{
        .action = None,
        .expected = ev,
        .delay_ms = ms,
        .clicks = clicks,
    };
};

auto suspend = [](uint8_t clicks) {
    return Step{
        .action = Suspend,
        .expected = ButtonEvent::None,
        .delay_ms = 0,
        .clicks = clicks,
    };
};

}  // namespace t
