#pragma once

#if defined(ARDUINO_ESP8266_WEMOS_D1MINI)

// nothing

#elif defined(ARDUINO_AVR_NANO)

#include "platform/hal/int/frameworks/arduino/boards/nano/instantiate_vectors.h"  // IWYU pragma: keep

#else

#error "unsupported board"

#endif
