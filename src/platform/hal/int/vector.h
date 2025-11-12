#pragma once

#include "platform/sys/config.h"

#include <cstddef>
#include <cstdlib>

namespace platform::detail {

template <size_t IntNo>
void _vector();

}  // namespace platform::detail

#define PLATFORM_DEFINE_INT_VECTOR(Pin, ...)                                      \
    template <>                                                                   \
    PLATFORM_RAM void ::platform::detail::_vector<Pin().interrupt().number()>() { \
        __VA_ARGS__();                                                            \
    }                                                                             \
    static_assert(true)
