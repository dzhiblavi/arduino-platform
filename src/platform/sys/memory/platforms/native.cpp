#include "platform/sys/config.h"  // IWYU pragma: keep

#if defined(PLATFORM_NATIVE)

#include "platform/sys/memory/usage.h"  // IWYU pragma: keep

namespace platform {

MemoryUsage getMemoryUsage() {
    return {.free_ram = static_cast<size_t>(-1)};
}

}  // namespace platform

#endif
