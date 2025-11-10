#pragma once

#include "platform/Callback.h"
#include "platform/Singleton.h"
#include "platform/hal/pin/type.h"
#include "platform/io/button/ButtonLevel.h"

#include <supp/Flags.h>
#include <time/mono.h>

namespace platform {

struct ButtonSettings {
    uint16_t hold_ms = 500;
    uint16_t timeout_ms = 500;  // after the last release
    uint8_t debounce_ms = 50;
    ButtonLevel level = ButtonLevel::High;
};

// clang-format off
enum class ButtonEvent : uint8_t {  // 3 bits
    //             0b.XXX....
    None         = 0b00000000,
    Pressed      = 0b00010000,
    Clicked      = 0b00100000,
    HoldStarted  = 0b00110000,
    HoldReleased = 0b01000000,
    Timeout      = 0b01010000,
};
// clang-format on

constexpr const char* toString(ButtonEvent event) {
    switch (event) {
        case ButtonEvent::None:
            return "None";
        case ButtonEvent::Pressed:
            return "Pressed";
        case ButtonEvent::Clicked:
            return "Clicked";
        case ButtonEvent::HoldStarted:
            return "HoldStarted";
        case ButtonEvent::HoldReleased:
            return "HoldReleased";
        case ButtonEvent::Timeout:
            return "Timeout";
    }

    __builtin_unreachable();
}

namespace sm {

struct ButtonSM {
    // The last bit is used for the pin digital value (0/1)
    // clang-format off
    enum State : uint8_t {
        //                0b....XXX.
        Idle            = 0b00000010,
        PressDebounce   = 0b00000100,
        ReleaseDebounce = 0b00000110,
        Pressing        = 0b00001000,
        Holding         = 0b00001010,
        Waiting         = 0b00001100,
    };
    // clang-format on

    void poll(bool engaged, const ButtonSettings& settings) {
        const auto now = static_cast<uint16_t>(ttime::mono::now().millis());
        const auto elapsed = now - timer_;

        switch (updateStateClearEvent(engaged)) {
            case Idle | 0b0:
                break;

            case Idle | 0b1:
                timer_ = now;
                set(PressDebounce, ButtonEvent::Pressed);
                break;

            case PressDebounce | 0b0:
            case PressDebounce | 0b1:
                if (elapsed >= settings.debounce_ms) {
                    set(Pressing, ButtonEvent::None);
                }
                break;

            case Pressing | 0b0:
                ++clicks_;
                timer_ = now;
                set(ReleaseDebounce, ButtonEvent::Clicked);
                break;

            case Pressing | 0b1:
                if (elapsed >= settings.hold_ms) {
                    set(Holding, ButtonEvent::HoldStarted);
                }
                break;

            case Holding | 0b0:
                clicks_ = 0;
                timer_ = now;
                set(ReleaseDebounce, ButtonEvent::HoldReleased);
                break;

            case Holding | 0b1:
                break;

            case ReleaseDebounce | 0b0:
                if (elapsed >= settings.debounce_ms) {
                    timer_ = now;
                    set(Waiting, ButtonEvent::None);
                }
                break;

            case ReleaseDebounce | 0b1:
                if (elapsed >= settings.debounce_ms) {
                    timer_ = now;
                    set(PressDebounce, ButtonEvent::Pressed);
                }
                break;

            case Waiting | 0b0:
                if (elapsed >= settings.timeout_ms) {
                    clicks_ = 0;
                    set(Idle, ButtonEvent::Timeout);
                }
                break;

            case Waiting | 0b1:
                timer_ = now;
                set(PressDebounce, ButtonEvent::Pressed);
                break;

            default:
                break;
        }
    }

    // clang-format off
    static constexpr uint8_t ISRMask   = 0b10000000;
    static constexpr uint8_t EventMask = 0b01110000;
    static constexpr uint8_t StateMask = 0b00001110;
    static constexpr uint8_t PinMask   = 0b00000001;
    // clang-format on

    SUPP_INLINE void clearEvent() { state_ = State(state_ & ~EventMask); }
    SUPP_INLINE void set(State state, ButtonEvent event) {
        state_ = State(static_cast<uint8_t>(event) | state);
    }

    SUPP_INLINE uint8_t updateStateClearEvent(bool engaged) {
        state_ = State((state_ & StateMask) | engaged | ((state_ & ISRMask) == ISRMask));
        return state_;
    }

    // The number of clicks preceding the event
    uint8_t clicks() const { return clicks_; }

    // The event that happened (reset on tick())
    ButtonEvent event() { return static_cast<ButtonEvent>(state_ & EventMask); }

    uint16_t timer_ = 0;
    uint8_t clicks_ = 0;
    volatile State state_ = Idle;
};

}  // namespace sm

template <type::InterruptPin Pin, ButtonSettings S = {}>
class Button : sm::ButtonSM, public Singleton<Button<Pin, S>> {
 public:
    using CallbackArgType = Button&;
    using CallbackType = Callback<CallbackArgType>;

    Button() = default;

    void init() {
        pin.init();
        pin.interrupt().attach(Button::buttonISR, interruptMode(S.level));
    }

    void tick() {
        if constexpr (S.level == ButtonLevel::High) {
            poll(pin.read(), S);
        } else {
            poll(!pin.read(), S);
        }

        if (event() != ButtonEvent::None && callback_) {
            callback_->run(*this);
        }
    }

    void setCallback(CallbackType* callback) { callback_ = callback; }

    using sm::ButtonSM::clicks;
    using sm::ButtonSM::event;

 private:
    static void buttonISR() {
        auto& s = Button::instance().state_;
        s = State(s | ISRMask);
    }

    CallbackType* callback_ = nullptr;
    [[no_unique_address]] const Pin pin{};
};

}  // namespace platform

#define PLATFORM_BUTTON_ISR(Pin, ...) \
    template PLATFORM_ISR void ::platform::Button<Pin, ##__VA_ARGS__>::buttonISR()
