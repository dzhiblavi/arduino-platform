#pragma once

#include "platform/Callback.h"
#include "platform/Singleton.h"
#include "platform/hal/pin/type.h"
#include "platform/io/button/ButtonLevel.h"

// #define EB_NO_CALLBACK // THIS ENABLES BUGS IN THE LIBRARY
#include <core/VirtButton.h>

namespace platform {

template <type::InterruptPin Pin, ButtonLevel Level>
class Button : public VirtButton, public Singleton<Button<Pin, Level>> {
 public:
    using CallbackArgType = Button&;
    using CallbackType = Callback<CallbackArgType>;

    Button() = default;

    void init() {
        setBtnLevel(Level == ButtonLevel::High);
        pin.init();
        pin.interrupt().attach(Button::buttonISR, interruptMode(Level));
    }

    void setCallback(CallbackType* callback) { callback_ = callback; }

    void tick() {
        if (VirtButton::tick(pin.read()) && callback_) {
            callback_->run(*this);
        }
    }

 private:
    static void buttonISR() { Button::instance().pressISR(); }

    CallbackType* callback_ = nullptr;
    [[no_unique_address]] const Pin pin{};
};

#define PLATFORM_BUTTON_ISR(Pin, Level) \
    template PLATFORM_ISR void ::platform::Button<Pin, Level>::buttonISR()

}  // namespace platform
