#pragma once

#include "platform/sys/config.h"  // IWYU pragma: keep

#if defined(PLATFORM_ARDUINO)

#include "platform/io/pin/mode.h"

#include <supp/verify.h>

#include <Arduino.h>

#include <concepts>

namespace platform {

namespace cap {

struct Dio;
struct Int;
struct Adc;
struct Pwm;

template <typename Cap, typename... Caps>
consteval bool has() {
    return (std::same_as<Cap, Caps> || ...);
}

};  // namespace cap

template <uint8_t P, typename... Caps>
struct Pin {
    template <GPIOMode Mode>
    struct Base {
        void init() {
            if constexpr (Mode != GPIOMode::Adc) {
                pinMode(P, platform::native(Mode));
            }
        }

        template <typename R = cap::Int>
        requires(cap::has<R, Caps...>())
        constexpr uint8_t interrupt() {
            return digitalPinToInterrupt(P);
        }

        constexpr uint8_t native() { return P; }
        constexpr platform::GPIOMode mode() { return Mode; }
    };

    template <GPIOMode Mode>
    requires(cap::has<cap::Dio, Caps...>() && isDigitalOutput(Mode))
    struct Output : Base<Mode> {
        void set() { digitalWrite(P, HIGH); }
        void clear() { digitalWrite(P, LOW); }

        template <typename R1 = cap::Pwm>
        requires(cap::has<R1, Caps...>())
        void pwm(uint8_t val) {
            analogWrite(P, val);
        }
    };

    template <GPIOMode Mode>
    requires(cap::has<cap::Dio, Caps...>() && isDigitalInput(Mode))
    struct Input : Base<Mode> {
        int read() { return digitalRead(P); }
    };

    template <typename R = cap::Adc>
    requires(cap::has<R, Caps...>())
    struct Adc : Base<GPIOMode::Adc> {
        int adc() { return analogRead(P); }
    };
};

#if defined(ARDUINO_AVR_NANO)

using D0 = Pin<0, cap::Dio>;
using D1 = Pin<1, cap::Dio>;
using D2 = Pin<2, cap::Dio, cap::Int>;
using D3 = Pin<3, cap::Dio, cap::Int, cap::Pwm>;
using D4 = Pin<4, cap::Dio>;
using D5 = Pin<5, cap::Dio, cap::Pwm>;
using D6 = Pin<6, cap::Dio, cap::Pwm>;
using D7 = Pin<7, cap::Dio>;
using D8 = Pin<8, cap::Dio>;
using D9 = Pin<9, cap::Dio, cap::Pwm>;
using D10 = Pin<10, cap::Dio, cap::Pwm>;  // +SPI(CS)
using D11 = Pin<11, cap::Dio, cap::Pwm>;  // +SPI(COPI)
using D12 = Pin<12, cap::Dio>;            // +SPI(CIPO)
using D13 = Pin<13, cap::Dio>;            // +SPI(SCK)
using D14 = Pin<14, cap::Dio, cap::Adc>;
using D15 = Pin<15, cap::Dio, cap::Adc>;
using D16 = Pin<16, cap::Dio, cap::Adc>;
using D17 = Pin<17, cap::Dio, cap::Adc>;
using D18 = Pin<18, cap::Dio, cap::Adc>;  // +I2C(SDA)
using D19 = Pin<19, cap::Dio, cap::Adc>;  // +I2C(SCL)
using A6 = Pin<20, cap::Adc>;
using A7 = Pin<21, cap::Adc>;

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
