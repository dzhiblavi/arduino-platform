#pragma once

#include "platform/hal/pin/type.h"
#include "platform/io/button/Button.h"
#include "platform/io/button/EncButtonSettings.h"
#include "platform/io/button/Encoder.h"

namespace platform {

template <
    type::InterruptPin S1,
    type::InterruptPin S2,
    type::DigitalInputPin Btn,
    EncButtonSettings S = {}>
class EncButton : public Encoder<S1, S2, S.encoder>, public Button<Btn, S.button> {
 public:
    EncButton() = default;

    void init() {
        Encoder<S1, S2, S.encoder>::init();
        Button<Btn, S.button>::init();
    }

    int8_t tick() {
        Button<Btn, S.button>::tick();
        return Encoder<S1, S2, S.encoder>::tick();
    }
};

}  // namespace platform

#define PLATFORM_ENC_BUTTON_ISR(S1, S2, Btn, S) \
    PLATFORM_ENCODER_ISR(S1, S2, S.encoder);  \
    PLATFORM_BUTTON_ISR(Btn, S.button)
