#pragma once

#include "platform/sys/config.h"  // IWYU pragma: keep

#if defined(PLATFORM_ARDUINO)

#include "platform/hal/int/frameworks/arduino/make.h"
#include "platform/hal/pin/frameworks/arduino/mode.h"

#include "platform/hal/pin/capability.h"

#include <supp/verify.h>

#include <Arduino.h>
#include <gio/gio.h>

namespace platform {

template <uint8_t P, IOCaps C>
struct Pin {
    struct Base {
        constexpr uint8_t native() const { return P; }
        constexpr IOCaps capabilities() const { return C; }
    };

    template <GPIOMode Mode>
    struct DIOBase : Base {
        void init() const { gio::init(P, platform::native(Mode)); }
        constexpr platform::GPIOMode mode() const { return Mode; }

        template <IOCaps X = C>
        requires(X.intr)
        constexpr type::Interrupt auto interrupt() const {
            return platform::Interrupt<digitalPinToInterrupt(P)>{};
        }
    };

    template <GPIOMode Mode>
    requires(C.dio && isDigitalOutput(Mode))
    struct Output : DIOBase<Mode> {
        void set() const { gio::high(P); }
        void clear() const { gio::low(P); }

        template <IOCaps X = C>
        requires(X.pwm)
        void pwm(uint8_t val) const {
            analogWrite(P, val);
        }
    };

    template <GPIOMode Mode>
    requires(C.dio && isDigitalInput(Mode))
    struct Input : DIOBase<Mode> {
        uint8_t read() const { return gio::read(P); }
    };

    template <IOCaps X = C>
    requires(X.adc)
    struct Adc : Base {
        void init() {}
        int adcRead() const { return analogRead(P); }
    };
};

}  // namespace platform

#endif
