#include "test.h"

#include <platform/hal/pin/frameworks/arduino/boards/current.h>

#include <logging/log.h>

using namespace platform;

constexpr auto settings = EncoderSettings{};
using S1 = D2::Input<GPIOMode::Input>;
using S2 = D3::Input<GPIOMode::Input>;
PLATFORM_ENCODER_ISR(S1, S2, settings);

struct Tester {
    Tester(t::Step* steps, size_t num_steps) : steps{steps}, num_steps{num_steps} {}

    void init() { encoder.init(); }

    // returns true if should continue
    bool loop() {
        if (index == num_steps) {
            return false;
        }

        Serial.flush();
        encoder.tick();

        const auto& step = steps[index];
        const auto event = encoder.event();
        const auto counter = encoder.counter();

        if (event != EncoderEvent::None) {
            if (event != step.expected) {
                LFATAL("[tester] expected ", toString(step.expected), ", found ", toString(event));
                index = num_steps + 1;
                return ok_ = false;
            }

            if (counter != step.counter) {
                LFATAL("[tester] expected ", step.counter, " counter, found ", counter);
                index = num_steps + 1;
                return ok_ = false;
            }

            LINFO("[tester] OK: ", toString(event), ", counter=", counter);
            ++index;

            LINFO("[tester] wait for ", index);
            return true;
        }

        return true;
    }

    bool ok() const { return ok_; }

    t::Step* steps;
    size_t index = 0;
    size_t num_steps;

    Encoder<S1, S2, settings> encoder;
    bool ok_ = true;
};
