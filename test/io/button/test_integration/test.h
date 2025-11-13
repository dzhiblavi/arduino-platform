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

Step steps[] = {
    // single click
    press(0),
    delay(0, 100),
    release(1, ButtonEvent::Clicked),
    delay(0, 600, ButtonEvent::Timeout),

    // single hold
    press(0),
    delay(0, 600, ButtonEvent::HoldStarted),
    release(0, ButtonEvent::Released),
    delay(0, 600, ButtonEvent::Timeout),

    // multiple clicks
    press(0),
    delay(0, 100),
    release(1, ButtonEvent::Clicked),
    delay(1, 100),
    press(1),
    delay(1, 100),
    release(2, ButtonEvent::Clicked),
    delay(1, 100),
    press(2),
    delay(2, 100),
    release(3, ButtonEvent::Clicked),
    delay(0, 600, ButtonEvent::Timeout),

    // clicks then hold then clicks
    press(0),
    delay(0, 100),
    release(1, ButtonEvent::Clicked),
    delay(1, 100),
    press(1),
    delay(1, 100),
    release(2, ButtonEvent::Clicked),
    delay(1, 100),
    press(2),
    delay(2, 600, ButtonEvent::HoldStarted),
    release(0, ButtonEvent::Released),
    delay(1, 100),
    press(0),
    delay(0, 100),
    release(1, ButtonEvent::Clicked),
    delay(1, 100),
    press(1),
    delay(1, 100),
    release(2, ButtonEvent::Clicked),
    delay(0, 600, ButtonEvent::Timeout),

    // does not register click when suspended
    press(0),
    suspend(0),
    delay(0, 200),
    release(0, ButtonEvent::Released),
    delay(0, 1000, ButtonEvent::Timeout),

    // does not register hold when suspended
    press(0),
    suspend(0),
    delay(0, 700),
    release(0, ButtonEvent::Released),
    delay(0, 700, ButtonEvent::Timeout),

    // registers clicks after unsuspended
    press(0),
    suspend(0),
    delay(0, 100),
    release(0, ButtonEvent::Released),
    delay(1, 100),
    press(0),
    delay(0, 100),
    release(1, ButtonEvent::Clicked),
    delay(0, 700, ButtonEvent::Timeout),

    // registers hold after unsuspended
    press(0),
    suspend(0),
    delay(0, 100),
    release(0, ButtonEvent::Released),
    delay(1, 100),
    press(0),
    delay(0, 700, ButtonEvent::HoldStarted),
    release(0, ButtonEvent::Released),
    delay(0, 700, ButtonEvent::Timeout),
};

constexpr size_t numSteps = sizeof(steps) / sizeof(Step);

}  // namespace t
