#pragma once

#include "platform/sys/config.h"  // IWYU pragma: keep

#if defined(PLATFORM_ARDUINO)

#include "platform/hal/int/platforms/arduino/make.h"
#include "platform/hal/pin/platforms/arduino/mode.h"

#include "platform/hal/pin/capability.h"

#include <supp/verify.h>

#include <Arduino.h>
#include <gio/gio_avr.h>

#include <concepts>

namespace platform {

template <uint8_t P, IOCaps C>
struct Pin {
    struct Base {
        constexpr uint8_t native() const { return P; }
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
        int read() const { return gio::read(P); }
    };

    template <IOCaps X = C>
    requires(X.adc)
    struct Adc : Base {
        void init() {}
        int adcRead() const { return analogRead(P); }
    };
};

#if defined(ARDUINO_AVR_NANO)

using D0 = Pin<0, {.dio = true}>;
using D1 = Pin<1, {.dio = true}>;
using D2 = Pin<2, {.dio = true, .intr = true}>;
using D3 = Pin<3, {.dio = true, .intr = true, .pwm = true}>;
using D4 = Pin<4, {.dio = true}>;
using D5 = Pin<5, {.dio = true, .pwm = true}>;
using D6 = Pin<6, {.dio = true, .pwm = true}>;
using D7 = Pin<7, {.dio = true}>;
using D8 = Pin<8, {.dio = true}>;
using D9 = Pin<9, {.dio = true, .pwm = true}>;
using D10 = Pin<10, {.dio = true, .pwm = true, .cs = true}>;
using D11 = Pin<11, {.dio = true, .pwm = true, .copi = true}>;
using D12 = Pin<12, {.dio = true, .cipo = true}>;
using D13 = Pin<13, {.dio = true, .sck = true}>;
using D14 = Pin<14, {.dio = true, .adc = true}>;
using D15 = Pin<15, {.dio = true, .adc = true}>;
using D16 = Pin<16, {.dio = true, .adc = true}>;
using D17 = Pin<17, {.dio = true, .adc = true}>;
using D18 = Pin<18, {.dio = true, .adc = true, .sda = true}>;
using D19 = Pin<19, {.dio = true, .adc = true, .scl = true}>;
using A6 = Pin<20, {.adc = true}>;
using A7 = Pin<21, {.adc = true}>;

// aliases
using A0 = D14;
using A1 = D15;
using A2 = D16;
using A3 = D17;
using A4 = D18;
using A5 = D19;

#endif

}  // namespace platform

#endif
