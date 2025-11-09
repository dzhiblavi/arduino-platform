#pragma once

#include "platform/hal/int/type.h"
#include "platform/hal/pin/capability.h"
#include "platform/hal/pin/mode.h"

#include <concepts>
#include <cstdint>

namespace platform::type {

template <typename T>
concept Pin = requires(T pin) {
    { pin.capabilities() } -> std::same_as<IOCaps>;
    { pin.init() } -> std::same_as<void>;
    { pin.native() } -> std::same_as<uint8_t>;
};

template <typename T>
concept DigitalPin = requires(T pin) {
    requires Pin<T>;
    { pin.mode() } -> std::same_as<GPIOMode>;
};

template <typename T>
concept DigitalOutputPin = requires(T pin) {
    requires DigitalPin<T>;
    { pin.set() } -> std::same_as<void>;
    { pin.clear() } -> std::same_as<void>;
};

template <typename T>
concept DigitalInputPin = requires(T pin) {
    requires DigitalPin<T>;
    { pin.read() } -> std::same_as<int>;
};

template <typename T>
concept InterruptPin = requires(T pin) {
    requires DigitalInputPin<T>;
    { pin.interrupt() } -> Interrupt;
};

template <typename T>
concept ADCPin = requires(T pin) {
    requires Pin<T>;
    { pin.adcRead() } -> std::same_as<int>;
};

template <typename T>
concept PWMPin = requires(T pin, uint8_t val) {
    requires Pin<T>;
    { pin.pwm(val) } -> std::same_as<void>;
};

}  // namespace platform::type
