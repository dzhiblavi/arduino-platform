#pragma once

#if defined(ARDUINO_ESP8266_WEMOS_D1MINI)

#include "platform/hal/pin/platforms/arduino/make.h"

namespace platform {

// clang-format off
using A0 = Pin<A0, IOCaps{.adc = true}>;
using D0 = Pin<D0, IOCaps{.dio = true, .pwm = true, .sda = true, .scl = true}>;
using D1 = Pin<D1, IOCaps{.dio = true, .intr = true, .pwm = true, .sda = true, .scl = true}>;
using D2 = Pin<D2, IOCaps{.dio = true, .intr = true, .pwm = true, .sda = true, .scl = true}>;
using D3 = Pin<D3, IOCaps{.dio = true, .intr = true, .pwm = true, .sda = true, .scl = true}>;
using D4 = Pin<D4, IOCaps{.dio = true, .intr = true, .pwm = true, .sda = true, .scl = true}>;
using D5 = Pin<D5, IOCaps{.dio = true, .intr = true, .pwm = true, .sda = true, .scl = true, .sck = true}>;
using D6 = Pin<D6, IOCaps{.dio = true, .intr = true, .pwm = true, .sda = true, .scl = true, .cipo = true}>;
using D7 = Pin<D7, IOCaps{.dio = true, .intr = true, .pwm = true, .sda = true, .scl = true, .copi = true}>;
using D8 = Pin<D8, IOCaps{.dio = true, .intr = true, .pwm = true, .sda = true, .scl = true, .cs = true}>;
// clang-format on

}  // namespace platform

#else

#error "another board detected"

#endif
