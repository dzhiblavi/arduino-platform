#include "actions.h"

#include <platform/hal/pin/frameworks/arduino/boards/current.h>

#include <logging/log.h>

using namespace platform;

using ControlPin = D8::Output<GPIOMode::Output>;

struct Emulator {
    Emulator(t::Step* steps, size_t num_steps) : steps{steps}, num_steps{num_steps} {}

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
        bounces = 2;
        timer = millis();
        state = Bouncing;
        LINFO("[emulator] bounce (pin toggle)");
        pin.toggle();
    }

    void bounceResume() {
        if (millis() - timer < 2) {
            return;
        }

        if (--bounces == 0) {
            LINFO("[emulator] bounce done");
            state = Ready;
        } else {
            state = Bouncing;
            LINFO("[emulator] bounce (pin toggle)");
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
        LINFO("[emulator] press (pin high)");
        pin.set();
        doBounce();
    }

    void release() {
        LINFO("[emulator] release (pin low)");
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

            case Bouncing:
                bounceResume();
                return true;

            case Waiting:
                waitResume();
                return true;
        }

        return true;
    }

    t::Step* steps;
    size_t index = 0;
    size_t num_steps;

    State state = Ready;
    uint32_t timer = 0;
    ControlPin pin;

    uint8_t bounces = 0;
    uint32_t sleep_for = 0;
};
