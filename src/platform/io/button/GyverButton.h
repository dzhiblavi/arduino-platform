#pragma once

#include "platform/Singleton.h"
#include "platform/hal/int/vector.h"  // IWYU pragma: keep
#include "platform/hal/pin/type.h"
#include "platform/io/button/ButtonSettings.h"
#include "platform/util/isr_template.h"

// #define EB_NO_CALLBACK // THIS ENABLES BUGS IN THE LIBRARY
#include <core/VirtButton.h>

namespace platform {

template <type::DigitalInputPin Pin, ButtonSettings S = {}>
class GyverButton : public VirtButton, public Singleton<GyverButton<Pin, S>> {
 public:
    GyverButton() = default;

    void init() {
        setBtnLevel(S.level == ButtonLevel::High);
        setDebTimeout(S.debounce_ms);
        setHoldTimeout(S.hold_ms);
        setTimeout(S.timeout_ms);
        pin.init();

        if constexpr (type::InterruptPin<Pin>) {
            pin.interrupt().attach(interruptMode(S.level));
        }
    }

    void tick() { VirtButton::tick(pin.read()); }
    static void buttonISR() { GyverButton::instance().pressISR(); }

 private:
    [[no_unique_address]] const Pin pin{};
};

#define PLATFORM_GYVER_BUTTON_ISR(Pin, ...)                                                      \
    PLATFORM_INSTANTIATE_ISR_TEMPLATE(::platform::GyverButton<Pin, ##__VA_ARGS__>::buttonISR()); \
    PLATFORM_DEFINE_INT_VECTOR(Pin, ::platform::GyverButton<Pin, ##__VA_ARGS__>::buttonISR)

}  // namespace platform
