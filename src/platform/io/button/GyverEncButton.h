#pragma once

#include "platform/Singleton.h"
#include "platform/hal/pin/type.h"
#include "platform/io/button/EncButtonSettings.h"

// #define EB_NO_CALLBACK // THIS ENABLES BUGS IN THE LIBRARY
#include <core/VirtEncButton.h>

namespace platform {

template <
    type::InterruptPin S1,
    type::InterruptPin S2,
    type::DigitalInputPin Btn,
    EncButtonSettings S = {}>
class GyverEncButton : public VirtEncButton, public Singleton<GyverEncButton<S1, S2, Btn, S>> {
 public:
    GyverEncButton() = default;

    void init() {
        setEncISR(true);
        setEncType(getType(S.encoder.type));

        setBtnLevel(S.button.level == ButtonLevel::High);
        setDebTimeout(S.button.debounce_ms);
        setHoldTimeout(S.button.hold_ms);
        setTimeout(S.button.timeout_ms);

        s1.init();
        s2.init();
        btn.init();

        s1.interrupt().attach(GyverEncButton::encISR, InterruptMode::Change);
        s2.interrupt().attach(GyverEncButton::encISR, InterruptMode::Change);
        if constexpr (type::InterruptPin<Btn>) {
            btn.interrupt().attach(GyverEncButton::btnISR, interruptMode(S.button.level));
        }
    }

    void tick() { VirtEncButton::tick(btn.read()); }

 private:
    static void encISR() { GyverEncButton::instance().tickISR(S1().read(), S2().read()); }
    static void btnISR() { GyverEncButton::instance().pressISR(); }

    static constexpr int getType(EncoderType type) {
        switch (type) {
            case Step4Low:
                return EB_STEP4_LOW;
            case Step4High:
                return EB_STEP4_HIGH;
            case Step2:
                return EB_STEP2;
            case Step1:
                return EB_STEP1;
        }
    }

    [[no_unique_address]] const S1 s1{};
    [[no_unique_address]] const S2 s2{};
    [[no_unique_address]] const Btn btn{};
};

}  // namespace platform

#define PLATFORM_ENC_BUTTON_ISR(S1, S2, Btn, ...)                                                \
    template PLATFORM_ISR void ::platform::GyverEncButton<S1, S2, Btn, ##__VA_ARGS__>::encISR(); \
    template PLATFORM_ISR void ::platform::GyverEncButton<S1, S2, Btn, ##__VA_ARGS__>::btnISR()
