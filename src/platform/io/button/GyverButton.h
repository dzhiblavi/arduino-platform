#pragma once

#include "platform/Singleton.h"
#include "platform/hal/pin/type.h"
#include "platform/io/button/ButtonSettings.h"

// #define EB_NO_CALLBACK // THIS ENABLES BUGS IN THE LIBRARY
#include <core/VirtButton.h>

namespace platform {

template <type::InterruptPin Pin, ButtonSettings S = {}>
class GyverButton : public VirtButton, public Singleton<GyverButton<Pin, S>> {
 public:
    GyverButton() = default;

    void init() {
        setBtnLevel(S.level == ButtonLevel::High);
        setDebTimeout(S.debounce_ms);
        setHoldTimeout(S.hold_ms);
        setTimeout(S.timeout_ms);
        pin.init();
        pin.interrupt().attach(GyverButton::buttonISR, interruptMode(S.level));
    }

    void tick() { VirtButton::tick(pin.read()); }

 private:
    static void buttonISR() { GyverButton::instance().pressISR(); }

    [[no_unique_address]] const Pin pin{};
};

#define PLATFORM_BUTTON_ISR(Pin, ...) \
    template PLATFORM_ISR void ::platform::GyverButton<Pin, ##__VA_ARGS__>::buttonISR()

}  // namespace platform
