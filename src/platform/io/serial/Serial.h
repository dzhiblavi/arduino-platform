#pragma once

#include "platform/sys/config.h"

#if defined(PLATFORM_NATIVE)

#include "platform/io/serial/frameworks/native.h"  // IWYU pragma: keep

#elif defined(PLATFORM_ARDUINO)

#include "platform/io/serial/frameworks/arduino.h"  // IWYU pragma: keep

#endif
