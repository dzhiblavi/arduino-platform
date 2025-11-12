#pragma once

#if defined(ARDUINO_ESP8266_WEMOS_D1MINI)

#include "platform/hal/int/frameworks/arduino/boards/d1_mini/make.h"  // IWYU pragma: keep

#elif defined(ARDUINO_AVR_NANO)

#include "platform/hal/int/frameworks/arduino/boards/nano/make.h"  // IWYU pragma: keep

#else

#error "unsupported board"

#endif
