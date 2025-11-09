#pragma once

#include "platform/sys/config.h"  // IWYU pragma: keep

#ifdef PLATFORM_ARDUINO

#include "platform/Callback.h"
#include "platform/Singleton.h"
#include "platform/hal/pin/type.h"
#include "platform/io/button/ButtonLevel.h"

// #define EB_NO_CALLBACK // THIS ENABLES BUGS IN THE LIBRARY
#include <EncButton.h>

namespace platform {

template <type::InterruptPin Pin, ButtonLevel Level>
class Button : public Singleton<Button<Pin, Level>> {
 public:
    using ImplType = ::ButtonT<Pin{}.native()>;
    using CallbackArgType = ImplType&;
    using CallbackType = Callback<CallbackArgType>;

    Button() = default;

    bool init() {
        constexpr Pin pin;
        impl_.init(native(pin.mode()), native(Level));
        pin.interrupt().attach(Button::buttonISR, interruptMode(Level));
        return true;
    }

    void setCallback(CallbackType* callback) { callback_ = callback; }

    ImplType& impl() { return impl_; }

    void tick() {
        impl_.tick();

        if (callback_ != nullptr && impl_.action()) {
            callback_->run(impl_);
        }
    }

 private:
    static void PLATFORM_RAM buttonISR() { Button::instance().impl_.pressISR(); }

    ImplType impl_;
    CallbackType* callback_ = nullptr;
};

}  // namespace platform

#endif
