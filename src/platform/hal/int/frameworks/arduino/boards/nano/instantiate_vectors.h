#pragma once

#include "platform/hal/int/vector.h"

#include <Arduino.h>

ISR(INT0_vect) {
    ::platform::detail::_vector<0>();
}

ISR(INT1_vect) {
    ::platform::detail::_vector<1>();
}
