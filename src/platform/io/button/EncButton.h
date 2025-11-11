#pragma once

#include "platform/hal/pin/type.h"
#include "platform/io/button/Button.h"
#include "platform/io/button/EncButtonSettings.h"
#include "platform/io/button/Encoder.h"

namespace platform {

enum class [[nodiscard]] EncButtonEvent : uint8_t {
    None,
    Pressed,
    Released,
    Clicked,
    HoldStarted,
    Timeout,
    TurnRight,
    TurnLeft,
    TurnRightHold,
    TurnLeftHold,
};

template <
    type::InterruptPin S1,
    type::InterruptPin S2,
    type::DigitalInputPin Btn,
    EncButtonSettings S = {}>
class EncButton : Encoder<S1, S2, S.encoder>, Button<Btn, S.button> {
    using E = Encoder<S1, S2, S.encoder>;
    using B = Button<Btn, S.button>;

 public:
    EncButton() = default;

    void init() {
        E::init();
        B::init();
    }

    void tick() {
        B::tick();
        E::tick();

        if (E::turning()) {
            B::suspendIfPressing();
        }
    }

    EncButtonEvent event() const {
        switch (E::event()) {
            case EncoderEvent::TurnLeft:
                return B::pressing() ? EncButtonEvent::TurnLeftHold : EncButtonEvent::TurnLeft;
            case EncoderEvent::TurnRight:
                return B::pressing() ? EncButtonEvent::TurnRightHold : EncButtonEvent::TurnRight;
            default:
                break;
        }
        switch (B::event()) {
            case ButtonEvent::Pressed:
                return EncButtonEvent::Pressed;
            case ButtonEvent::Released:
                return EncButtonEvent::Released;
            case ButtonEvent::Clicked:
                return EncButtonEvent::Clicked;
            case ButtonEvent::HoldStarted:
                return EncButtonEvent::HoldStarted;
            case ButtonEvent::Timeout:
                return EncButtonEvent::Timeout;
            default:
                break;
        }

        return EncButtonEvent::None;
    }

    using B::clicks;
};

}  // namespace platform

#define PLATFORM_ENC_BUTTON_ISR(S1, S2, Btn, S) \
    PLATFORM_ENCODER_ISR(S1, S2, S.encoder);    \
    PLATFORM_BUTTON_ISR(Btn, S.button)
