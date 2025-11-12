#pragma once

#if defined(ARDUINO_AVR_NANO)

#include "platform/hal/pin/frameworks/arduino/boards/nano.h"  // IWYU pragma: keep

#elif defined(ARDUINO_ESP8266_WEMOS_D1MINI)

#include "platform/hal/pin/frameworks/arduino/boards/d1_mini.h"  // IWYU pragma: keep

#else

#error "the board is not supported"

#endif
