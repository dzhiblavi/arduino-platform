#include "platform/sys/config.h"  // IWYU pragma: keep

#if defined(PLATFORM_ARDUINO_AVR)

#include "platform/sys/memory/usage.h"  // IWYU pragma: keep

extern int __heap_start, *__brkval;

namespace platform {

MemoryUsage getMemoryUsage() {
    int v;
    int free_ram = (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);

    return {
        .free_ram = static_cast<size_t>(free_ram),
    };
}

}  // namespace platform

#endif
