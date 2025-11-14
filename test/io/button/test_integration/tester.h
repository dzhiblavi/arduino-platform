#include "io/button/emulator/actions.h"

#include <platform/hal/pin/frameworks/arduino/boards/current.h>

#include <logging/log.h>

using namespace platform;

constexpr auto settings = ButtonSettings{
    .hold_ms = 50,
    .timeout_ms = 50,
    .debounce_ms = 8,
};

using ButtonPin = D2::Input<GPIOMode::Input>;
PLATFORM_BUTTON_ISR(ButtonPin, settings);

struct Tester {
    Tester(t::Step* steps, size_t num_steps) : steps{steps}, num_steps{num_steps} {}

    void init() { button.init(); }

    bool shouldSkip(const t::Step& step) {
        if (step.action == t::Suspend) {
            return false;
        }

        return step.expected == ButtonEvent::None;
    }

    // returns true if should continue
    bool loop() {
        while (index < num_steps && shouldSkip(steps[index])) {
            ++index;
        }

        if (index == num_steps) {
            return false;
        }

        Serial.flush();
        button.tick();

        const auto& step = steps[index];
        const auto event = button.event();
        const auto clicks = button.clicks();

        if (step.action == t::Suspend) {
            LINFO("[tester] suspend button");
            DASSERT(button.pressing(), "[tester] button should be in pressing state");
            DASSERT(event == ButtonEvent::None, "[tester] invalid step");

            button.suspendIfPressing();
            ++index;
            LINFO("[tester] wait for ", index);
            return true;
        }

        if (event != ButtonEvent::None) {
            if (event != step.expected) {
                LFATAL("[tester] expected ", toString(step.expected), ", found ", toString(event));
                index = num_steps + 1;
                return ok_ = false;
            }

            if (clicks != step.clicks) {
                LFATAL("[tester] expected ", step.clicks, " clicks, found ", clicks);
                index = num_steps + 1;
                return ok_ = false;
            }

            LINFO("[tester] OK: ", toString(event), ", clicks=", clicks);
            ++index;

            if (event == ButtonEvent::Timeout) {
                LINFO("[tester] OK, test part done\n");
            }

            LINFO("[tester] wait for ", index);
            return true;
        }

        return true;
    }

    bool ok() const { return ok_; }

    t::Step* steps;
    size_t index = 0;
    size_t num_steps;

    Button<ButtonPin, settings> button;
    bool ok_ = true;
};
