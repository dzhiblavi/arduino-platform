#pragma once

#include <Arduino.h>

namespace platform::detail {

template <size_t N>
void _vector() {
    abort();
}

}  // namespace platform::detail

ISR(INT0_vect) {
    ::platform::detail::_vector<0>();
}

ISR(INT1_vect) {
    ::platform::detail::_vector<1>();
}
