#pragma once

#include "platform/hal/int/mode.h"

#include <concepts>

namespace platform {

using InterruptFunc = void (*)();

namespace type {

template <typename T>
concept Interrupt = requires(T interrupt, InterruptMode mode) {
    { interrupt.attach(mode) } -> std::same_as<void>;
    { interrupt.detach() } -> std::same_as<void>;
    { interrupt.number() } -> std::same_as<uint8_t>;
};

}  // namespace type

}  // namespace platform
