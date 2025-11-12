#pragma once

#if defined(ARDUINO_AVR_NANO)

#include "platform/hal/pin/frameworks/arduino/make.h"

namespace platform {

using D0 = Pin<0, {.dio = true}>;
using D1 = Pin<1, {.dio = true}>;
using D2 = Pin<2, {.dio = true, .intr = true}>;
using D3 = Pin<3, {.dio = true, .intr = true, .pwm = true}>;
using D4 = Pin<4, {.dio = true}>;
using D5 = Pin<5, {.dio = true, .pwm = true}>;
using D6 = Pin<6, {.dio = true, .pwm = true}>;
using D7 = Pin<7, {.dio = true}>;
using D8 = Pin<8, {.dio = true}>;
using D9 = Pin<9, {.dio = true, .pwm = true}>;
using D10 = Pin<10, {.dio = true, .pwm = true, .cs = true}>;
using D11 = Pin<11, {.dio = true, .pwm = true, .copi = true}>;
using D12 = Pin<12, {.dio = true, .cipo = true}>;
using D13 = Pin<13, {.dio = true, .sck = true}>;
using D14 = Pin<14, {.dio = true, .adc = true}>;
using D15 = Pin<15, {.dio = true, .adc = true}>;
using D16 = Pin<16, {.dio = true, .adc = true}>;
using D17 = Pin<17, {.dio = true, .adc = true}>;
using D18 = Pin<18, {.dio = true, .adc = true, .sda = true}>;
using D19 = Pin<19, {.dio = true, .adc = true, .scl = true}>;
using A6 = Pin<20, {.adc = true}>;
using A7 = Pin<21, {.adc = true}>;

// aliases
using A0 = D14;
using A1 = D15;
using A2 = D16;
using A3 = D17;
using A4 = D18;
using A5 = D19;

}  // namespace platform

#else

#error "another board detected"

#endif
