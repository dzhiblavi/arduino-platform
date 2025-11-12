#pragma once

#include "platform/sys/config.h"  // IWYU pragma: keep

#include "platform/hal/int/frameworks/arduino/mode.h"

#include <supp/no_match.h>

namespace platform {

template <uint8_t IntNo>
struct Interrupt {
    constexpr uint8_t number() const { return IntNo; }

    void attach(InterruptMode mode) const {
        if constexpr (IntNo == 0) {
            EICRA = (EICRA & ~((1 << ISC00) | (1 << ISC01))) | (native(mode) << ISC00);
            EIMSK |= (1 << INT0);
        } else if constexpr (IntNo == 1) {
            EICRA = (EICRA & ~((1 << ISC10) | (1 << ISC11))) | (native(mode) << ISC10);
            EIMSK |= (1 << INT1);
        } else {
            supp::noMatch();
        }
    }

    void detach() const {
        if constexpr (IntNo == 0) {
            EIMSK &= ~(1 << INT0);
        } else if constexpr (IntNo == 1) {
            EIMSK &= ~(1 << INT1);
        } else {
            supp::noMatch();
        }
    }
};

}  // namespace platform
