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

template <
    type::InterruptPin S1,
    type::InterruptPin S2,
    type::DigitalInputPin Btn,
    ButtonLevel Level = ButtonLevel::Low>
class EncButton : public exec::Service {
 public:
    static_assert(S1{}.mode() == S2{}.mode(), "both S encoder pins must have the same mode");

    using ImplType = ::EncButton;
    using CallbackArgType = ImplType&;
    using CallbackType = exec::Callback<CallbackArgType>;

    EncButton() { exec::OS::globalAddService(this); }

    bool init() {
        S1 s1;
        S2 s2;
        Btn btn;

        impl_.init(
            s1.native(),
            s2.native(),
            btn.native(),
            native(s1.mode()),
            native(btn.mode()),
            native(Level));
        impl_.setEncISR(true);

        attachInterruptArg(s1.interrupt(), EncButton::encISR, this, InterruptMode::Change);
        attachInterruptArg(s2.interrupt(), EncButton::encISR, this, InterruptMode::Change);
        if constexpr (type::InterruptPin<Btn>) {
            attachInterruptArg(btn.interrupt(), EncButton::btnISR, this, interruptMode(Level));
        }

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
    static void PLATFORM_RAM encISR(void* self) { static_cast<EncButton*>(self)->impl_.tickISR(); }
    static void PLATFORM_RAM btnISR(void* self) { static_cast<EncButton*>(self)->impl_.pressISR(); }

    ImplType impl_;
    CallbackType* callback_ = nullptr;
};

}  // namespace platform

#endif
