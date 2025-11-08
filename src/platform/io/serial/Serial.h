#pragma once

#include "platform/sys/config.h"

#if defined(PLATFORM_NATIVE)

#include "platform/io/serial/platforms/native.h"  // IWYU pragma: keep

#elif defined(PLATFORM_ARDUINO)

#include "platform/io/serial/platforms/arduino.h"  // IWYU pragma: keep

#endif
