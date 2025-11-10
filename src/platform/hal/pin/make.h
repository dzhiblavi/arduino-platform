#pragma once

#include "platform/sys/config.h"  // IWYU pragma: keep

#if defined(PLATFORM_ARDUINO)

#include "platform/hal/pin/frameworks/arduino/make.h"  // IWYU pragma: keep

#elif defined(PLATFORM_NATIVE)

#include "platform/hal/pin/frameworks/native/make.h"  // IWYU pragma: keep

#endif
