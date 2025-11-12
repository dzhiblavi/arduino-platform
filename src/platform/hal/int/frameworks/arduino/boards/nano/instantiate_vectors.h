#pragma once

#include "platform/hal/int/vector.h"

#include <Arduino.h>

namespace platform::detail {

template <size_t>
void _vector() {
    // undefined pin
    abort();
}

}  // namespace platform::detail

ISR(INT0_vect) {
    ::platform::detail::_vector<0>();
}

ISR(INT1_vect) {
    ::platform::detail::_vector<1>();
}
