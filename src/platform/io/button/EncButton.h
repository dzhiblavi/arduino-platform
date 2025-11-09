#pragma once

#include "platform/Callback.h"
#include "platform/Singleton.h"
#include "platform/hal/pin/type.h"
#include "platform/io/button/ButtonLevel.h"

// #define EB_NO_CALLBACK // THIS ENABLES BUGS IN THE LIBRARY
#include <core/VirtEncButton.h>

namespace platform {

template <
    type::InterruptPin S1,
    type::InterruptPin S2,
    type::DigitalInputPin Btn,
    ButtonLevel Level = ButtonLevel::Low>
class EncButton : public VirtEncButton, public Singleton<EncButton<S1, S2, Btn, Level>> {
 public:
    using CallbackArgType = EncButton&;
    using CallbackType = Callback<CallbackArgType>;

    EncButton() = default;

    void init() {
        setBtnLevel(Level == ButtonLevel::High);
        setEncISR(true);

        s1.init();
        s2.init();
        btn.init();

        s1.interrupt().attach(EncButton::encISR, InterruptMode::Change);
        s2.interrupt().attach(EncButton::encISR, InterruptMode::Change);
        if constexpr (type::InterruptPin<Btn>) {
            btn.interrupt().attach(EncButton::btnISR, interruptMode(Level));
        }
    }

    void setCallback(CallbackType* callback) { callback_ = callback; }

    void tick() {
        if (VirtEncButton::tick(s1.read(), s2.read(), btn.read()) && callback_) {
            callback_->run(*this);
        }
    }

 private:
    static void encISR() { EncButton::instance().tickISR(S1().read(), S2().read()); }
    static void btnISR() { EncButton::instance().pressISR(); }

    CallbackType* callback_ = nullptr;
    [[no_unique_address]] const S1 s1{};
    [[no_unique_address]] const S2 s2{};
    [[no_unique_address]] const Btn btn{};
};

}  // namespace platform

#define PLATFORM_ENC_BUTTON_ISR(S1, S2, Btn, ...)                                          \
    template PLATFORM_ISR void ::platform::EncButton<S1, S2, Btn, ##__VA_ARGS__>::encISR(); \
    template PLATFORM_ISR void ::platform::EncButton<S1, S2, Btn, ##__VA_ARGS__>::btnISR()
