#pragma once

#if defined(ARDUINO_ARCH_AVR)

#define PLATFORM_ARDUINO
#define PLATFORM_ARDUINO_AVR

#define PLATFORM_RAM
#define PLATFORM_ISR

#elif defined(ARDUINO_ARCH_ESP8266)

#include <c_types.h>  // IRAM_ATTR

#define PLATFORM_ARDUINO
#define PLATFORM_ARDUINO_ESP8266

#define PLATFORM_RAM IRAM_ATTR
#define PLATFORM_ISR PLATFORM_RAM

#else

#define PLATFORM_NATIVE
#define PLATFORM_RAM
#define PLATFORM_ISR

#endif
