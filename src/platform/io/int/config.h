#include "platform/sys/config.h"  // IWYU pragma: keep

#if !defined(PLATFORM_INT_MANUAL)

#if defined(PLATFORM_NATIVE)

#define PLATFORM_INT_MANUAL

#elif defined(PLATFORM_ARDUINO_AVR)

#define PLATFORM_INT_ARDUINO_AVR

#elif defined(PLATFORM_ARDUINO_ESP8266)

#define PLATFORM_INT_ARDUINO_ESP8266

#endif

#endif
