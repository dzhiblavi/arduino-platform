#pragma once

#include "platform/sys/config.h"  // IWYU pragma: keep

#ifdef PLATFORM_ARDUINO

#include "platform/io/button/ButtonLevel.h"
#include "platform/io/int/interrupts.h"
#include "platform/io/pin/type.h"

#include <exec/Callback.h>
#include <exec/os/OS.h>
#include <exec/os/Service.h>

// #define EB_NO_CALLBACK // THIS ENABLES BUGS IN THE LIBRARY
#include <EncButton.h>

namespace platform {

template <type::InterruptPin Pin, ButtonLevel Level>
class Button : public exec::Service {
 public:
    using ImplType = ::Button;
    using CallbackArgType = ImplType&;
    using CallbackType = exec::Callback<CallbackArgType>;

    Button() { exec::OS::globalAddService(this); }

    bool init() {
        Pin pin;
        impl_.init(pin.native(), native(pin.mode()), native(Level));
        attachInterruptArg(pin.interrupt(), Button::buttonISR, this, interruptMode(Level));
        return true;
    }

    void setCallback(CallbackType* callback) { callback_ = callback; }

    ImplType& impl() { return impl_; }

    // Service
    void tick() override {
        impl_.tick();

        if (callback_ != nullptr && impl_.action()) {
            callback_->run(impl_);
        }
    }

 private:
    static void PLATFORM_RAM buttonISR(void* self) { static_cast<Button*>(self)->impl_.pressISR(); }

    ImplType impl_;
    CallbackType* callback_ = nullptr;
};

}  // namespace platform

#endif
