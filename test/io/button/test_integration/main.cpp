#include "test.h"

#include <platform/hal/pin/frameworks/arduino/boards/current.h>

#include <logging/log.h>

#include <Arduino.h>

using namespace platform;

constexpr auto settings = ButtonSettings{};

using ButtonPin = D2::Input<GPIOMode::Input>;
using ControlPin = D9::Output<GPIOMode::Output>;

PLATFORM_BUTTON_ISR(ButtonPin, settings);

struct Emulator {
    enum State : uint8_t {
        Ready,
        Bouncing,
        Waiting,
    };

    void init() {
        pin.init();
        pin.clear();
    }

    void doBounce() {
        bounces = 6;
        timer = millis();
        pin.toggle();
        state = Bouncing;
        LINFO("[emulator] bounce started");
    }

    void bounceResume() {
        if (millis() - timer < 5) {
            return;
        }

        if (--bounces == 0) {
            LINFO("[emulator] bounce done");
            state = Ready;
        } else {
            state = Bouncing;
            pin.toggle();
            timer = millis();
        }
    }

    void doWait(uint32_t dur) {
        timer = millis();
        sleep_for = dur;
        state = Waiting;
        LINFO("[emulator] wait for ", dur);
    }

    void waitResume() {
        if (millis() - timer < sleep_for) {
            return;
        }

        LINFO("[emulator] wait resume");
        state = Ready;
    }

    void press() {
        LINFO("[emulator] press");
        pin.set();
        doBounce();
    }

    void release() {
        LINFO("[emulator] release");
        pin.clear();
        doBounce();
    }

    void run(const t::Step& step) {
        switch (step.action) {
            case t::Suspend:
            case t::None:
                break;

            case t::Press:
                press();
                break;

            case t::Release:
                release();
                break;
        }

        if (step.delay_ms > 0) {
            doWait(step.delay_ms);
        }
    }

    void loop() {
        static size_t index = 0;

        switch (state) {
            case Ready:
                if (index < t::numSteps) {
                    run(t::steps[index++]);
                }
                break;

            case Bouncing:
                bounceResume();
                break;

            case Waiting:
                waitResume();
                break;
        }
    }

    State state = Ready;
    uint32_t timer = 0;
    ControlPin pin;

    uint8_t bounces = 0;
    uint32_t sleep_for = 0;
};

struct Tester {
    void init() { button.init(); }

    bool shouldSkip(const t::Step& step) {
        if (step.action == t::Suspend) {
            return false;
        }

        return step.expected == ButtonEvent::None;
    }

    void loop() {
        static size_t index = 0;

        if (index == t::numSteps + 1) {
            return;
        }

        while (index < t::numSteps && shouldSkip(t::steps[index])) {
            ++index;
        }

        if (index == t::numSteps) {
            LINFO("tests done");
            ++index;
            return;
        }

        button.tick();

        const auto& step = t::steps[index];
        const auto event = button.event();
        const auto clicks = button.clicks();

        if (step.action == t::Suspend) {
            LINFO("suspend button");
            DASSERT(button.pressing(), "button should be in pressing state");
            DASSERT(event == ButtonEvent::None, "invalid step");

            button.suspendIfPressing();
            ++index;
        } else if (event != ButtonEvent::None) {
            if (event != step.expected) {
                LFATAL("expected ", toString(step.expected), ", found ", toString(event));
                index = t::numSteps + 1;
                return;
            }

            if (clicks != step.clicks) {
                LFATAL("expected ", step.clicks, " clicks, found ", clicks);
                index = t::numSteps + 1;
                return;
            }

            LINFO("OK: ", toString(event), ", clicks=", clicks);
            ++index;

            if (event == ButtonEvent::Timeout) {
                LINFO("OK, test part done\n");
            }
        }
    }

    Button<ButtonPin, settings> button;
};

logging::Logger global;
Emulator emulator;
Tester tester;

void setup() {
    global.begin();
    global.setLevel(logging::Level::Info);
    logging::setGlobal(&global);

    emulator.init();
    tester.init();
    LINFO("starting the test");
}

void loop() {
    emulator.loop();
    tester.loop();
}

#include <platform/hal/int/instantiate_vectors.h>
