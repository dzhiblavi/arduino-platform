#pragma once

#include "platform/sys/config.h"  // IWYU pragma: keep

#if !defined(PLATFORM_NATIVE)

#error "emulator can only be used on native platform"

#endif

#include "platform/hal/int/mode.h"
#include "platform/hal/int/vector.h"
#include "platform/hal/pin/mode.h"

#include <supp/verify.h>

#include <unordered_map>

namespace platform {

class Emulator {
 public:
    using Pin = uint8_t;

    // Pins
    void init(Pin pin, GPIOMode mode) {
        LTRACE("[emulator] init pin=", pin, ", mode=", toString(mode));
        pins_[pin] = {mode, 0};
    }

    int analogRead(Pin pin) {
        const auto& p = pins_[pin];
        LTRACE("[emulator] analogRead pin=", pin, ", value=", p.value);

        DASSERT(p.mode == std::nullopt);
        return p.value;
    }

    void write(Pin pin, bool value) {
        auto& p = pins_[pin];
        LTRACE("[emulator] write pin=", pin, ", value=", value);

        DASSERT(p.mode && isOutput(*p.mode));
        p.pwm = false;
        p.value = static_cast<int>(value);
    }

    uint8_t read(Pin pin) {
        auto& p = pins_[pin];
        LTRACE("[emulator] read pin=", pin, ", value=", p.value);

        DASSERT(p.mode && isInput(*p.mode));
        return p.value;
    }

    void pwm(Pin pin, uint8_t value) {
        auto& p = pins_[pin];
        LTRACE("[emulator] pwm pin=", pin, ", value=", value);

        DASSERT(p.mode && isOutput(*p.mode));
        p.pwm = true;
        p.value = static_cast<int>(value);
    }

    // Interrupts
    void attachInterrupt(Pin pin, InterruptMode mode) {
        LTRACE("[emulator] attach pin=", pin, ", mode=", toString(mode));
        interrupts_[pin] = mode;
    }

    void detachInterrupt(Pin pin) {
        LTRACE("[emulator] detach pin=", pin);
        DASSERT(interrupts_.contains(pin));
        interrupts_.erase(pin);
    }

    // Control emulator
    void writePin(Pin pin, int value) {
        auto& p = pins_[pin];
        LTRACE("[emulator] emulate write pin=", pin, ", value=", value);
        p.value = value;
    }

    void raiseInterrupt(Pin pin, InterruptMode mode) {
        auto it = interrupts_.find(pin);
        if (it == interrupts_.end()) {
            LTRACE("[emulator] raise (noint) pin=", pin, ", mode=", toString(mode));
            return;
        }

        auto&& intmode = it->second;
        if (intmode != mode) {
            LTRACE("[emulator] raise (nomode) pin=", pin, ", mode=", toString(mode));
            return;
        }

        LTRACE("[emulator] raise (call) pin=", pin, ", mode=", toString(mode));
        switch (pin) {
            case 0:
                ::platform::detail::_vector<0>();
                break;
            case 1:
                ::platform::detail::_vector<0>();
                break;
            case 2:
                ::platform::detail::_vector<0>();
                break;
            default:
                DASSERT(false, "emulator supports 0,1,2 interrupts only");
                break;
        }
    }

    static Emulator& instance() {
        static Emulator instance;
        return instance;
    }

 private:
    Emulator() = default;

    struct PinState {
        std::optional<GPIOMode> mode = std::nullopt;  // None for ADC
        int value = 0;                                // ADC or digital
        bool pwm = false;
    };

    std::unordered_map<Pin, InterruptMode> interrupts_;
    std::unordered_map<Pin, PinState> pins_;
};

}  // namespace platform
