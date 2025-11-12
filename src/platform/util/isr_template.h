#pragma once

#include "platform/sys/config.h"

#define PLATFORM_INSTANTIATE_ISR_TEMPLATE(...) template PLATFORM_ISR __VA_ARGS__
