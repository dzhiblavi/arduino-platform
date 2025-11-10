#pragma once

#include "platform/sys/config.h"  // IWYU pragma: keep

#if defined(PLATFORM_NATIVE)

#include "platform/hal/emulator/Emulator.h"
#include "platform/hal/int/type.h"

namespace platform {

template <uint8_t IntNo>
struct Interrupt {
    void attach(InterruptFunc func, InterruptMode mode) const {
        Emulator::instance().attachInterrupt(IntNo, func, mode);
    }

    void detach() const { Emulator::instance().detachInterrupt(IntNo); }
};

}  // namespace platform

#endif
