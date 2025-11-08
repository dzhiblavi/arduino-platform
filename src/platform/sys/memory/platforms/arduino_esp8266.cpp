#include "platform/sys/config.h"  // IWYU pragma: keep

#if defined(PLATFORM_ARDUINO_ESP8266)

#include "platform/sys/memory/usage.h"  // IWYU pragma: keep

#include <ESP.h>

namespace platform {

MemoryUsage getMemoryUsage() {
    return {
        .free_ram = ESP.getFreeHeap(),
    };
}

}  // namespace platform

#endif
