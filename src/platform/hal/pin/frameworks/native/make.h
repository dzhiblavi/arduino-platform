#pragma once

#include "platform/sys/config.h"  // IWYU pragma: keep

#if defined(PLATFORM_NATIVE)

#include "platform/hal/emulator/Emulator.h"
#include "platform/hal/int/frameworks/native/make.h"
#include "platform/hal/int/type.h"
#include "platform/hal/pin/capability.h"

namespace platform {

template <uint8_t P, platform::GPIOMode Mode>
struct Pin {
    void init() const { Emulator::instance().init(P, Mode); }
    int adcRead() const { return Emulator::instance().analogRead(P); }
    void set() const { Emulator::instance().write(P, true); }
    void clear() const { Emulator::instance().write(P, false); }
    void toggle() const { Emulator::instance().toggle(P); }
    uint8_t read() const { return Emulator::instance().read(P); }
    void pwm(uint8_t val) const { Emulator::instance().pwm(P, val); }

    constexpr platform::GPIOMode mode() const { return Mode; }
    constexpr uint8_t native() const { return P; }
    constexpr type::Interrupt auto interrupt() const { return platform::Interrupt<P>{}; }

    constexpr IOCaps capabilities() const {
        return IOCaps{
            .dio = true,
            .intr = true,
            .adc = true,
            .pwm = true,
            .sda = true,
            .scl = true,
            .cs = true,
            .sck = true,
            .copi = true,
            .cipo = true,
        };
    }
};

}  // namespace platform

#endif
