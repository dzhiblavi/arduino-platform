#pragma once

#include <supp/verify.h>

namespace platform {

template <typename Self>
class Singleton {
 public:
    Singleton() {
        DASSERT(self_ == nullptr);
        self_ = static_cast<Self*>(this);
    }

    static Self& instance() {
        DASSERT(self_ != nullptr);
        return *self_;
    }

 private:
    inline static Self* self_ = nullptr;
};

}  // namespace platform
