#pragma once

#include "platform/sys/config.h"  // IWYU pragma: keep

#if defined(PLATFORM_NATIVE)

#include "platform/hal/emulator/Emulator.h"

namespace platform {

template <uint8_t IntNo>
struct Interrupt {
    constexpr uint8_t number() const { return IntNo; }

    void attach(InterruptMode mode) const { Emulator::instance().attachInterrupt(IntNo, mode); }
    void detach() const { Emulator::instance().detachInterrupt(IntNo); }
};

}  // namespace platform

#endif
