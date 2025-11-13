#include "test.h"

#include <platform/hal/pin/frameworks/arduino/boards/current.h>

#include <logging/log.h>

using namespace platform;

using ControlPin1 = D8::Output<GPIOMode::Output>;
using ControlPin2 = D9::Output<GPIOMode::Output>;

struct Emulator {
    Emulator(t::Step* steps, size_t num_steps) : steps{steps}, num_steps{num_steps} {}

    enum State : uint8_t {
        Ready,
        Rotating,
        DelayWaiting,
    };

    void init() {
        s1.init();
        s1.clear();

        s2.init();
        s2.clear();
    }

    void doWait(uint32_t dur) {
        timer = millis();
        sleep_for = dur;
        state = DelayWaiting;
        LINFO("[emulator] wait for ", dur);
    }

    void waitResume() {
        if (millis() - timer < sleep_for) {
            return;
        }

        LINFO("[emulator] wait resume");
        state = Ready;
    }

    void doRotate(bool right) {
        timer = millis();
        rot_steps = right ? +4 : -4;
        state = Rotating;
        LINFO("[emulator] rotate ", right);

        if (rot_steps > 0) {
            s1.toggle();
            --rot_steps;
        } else {
            s2.toggle();
            ++rot_steps;
        }
    }

    void rotateResume() {
        // 5 ms delay between wave transitions
        if (millis() - timer < 5) {
            return;
        }

        if (rot_steps > 0) {
            (rot_steps & 1) ? s2.toggle() : s1.toggle();
            --rot_steps;
        } else {
            ((-rot_steps) & 1) ? s1.toggle() : s2.toggle();
            ++rot_steps;
        }

        if (rot_steps == 0) {
            LINFO("[emulator] rotate done");
            state = Ready;
        }
    }

    void run(const t::Step& step) {
        switch (step.action) {
            case t::None:
                break;

            case t::Left:
                doRotate(false);
                break;

            case t::Right:
                doRotate(true);
                break;
        }

        if (step.delay_ms > 0) {
            doWait(step.delay_ms);
        }
    }

    // returns true if should continue
    bool loop() {
        switch (state) {
            case Ready:
                if (index < num_steps) {
                    run(steps[index++]);
                    return true;
                } else {
                    return false;
                }

            case Rotating:
                rotateResume();
                break;

            case DelayWaiting:
                waitResume();
                break;
        }

        return true;
    }

    t::Step* steps;
    size_t index = 0;
    size_t num_steps;

    State state = Ready;
    ControlPin1 s1;
    ControlPin2 s2;

    uint32_t timer = 0;
    uint32_t sleep_for = 0;
    int8_t rot_steps = 0;
};
