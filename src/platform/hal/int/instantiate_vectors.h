#pragma once

#include "platform/sys/config.h"  // IWYU pragma: keep

#if defined(PLATFORM_ARDUINO)

#include "platform/hal/int/frameworks/arduino/instantiate_vectors.h"  // IWYU pragma: keep

#elif defined(PLATFORM_NATIVE)

#include "platform/hal/int/frameworks/native/instantiate_vectors.h"  // IWYU pragma: keep

#endif
