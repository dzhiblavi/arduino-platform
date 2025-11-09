#pragma once

namespace platform {

template <typename T>
class Callback {
 public:
    virtual ~Callback() = default;
    virtual void run(T) = 0;
};

}  // namespace platform
