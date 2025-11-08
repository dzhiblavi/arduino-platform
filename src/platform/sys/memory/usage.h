#pragma once

#include <cstddef>

namespace platform {

struct MemoryUsage {
    size_t free_ram;
};

MemoryUsage getMemoryUsage();

}  // namespace platform
