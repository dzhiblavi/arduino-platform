#pragma once

#include "platform/hal/int/mode.h"

#include <concepts>
#include <cstdint>

namespace platform::type {

using InterruptFunc = void (*)();

template <typename T>
concept Interrupt = requires(T interrupt, InterruptFunc func, InterruptMode mode) {
    { interrupt.attach(func, mode) } -> std::same_as<void>;
    { interrupt.detach() } -> std::same_as<void>;
};

}  // namespace platform::type
