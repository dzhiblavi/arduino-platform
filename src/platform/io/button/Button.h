#pragma once

#include "platform/Singleton.h"
#include "platform/hal/pin/type.h"
#include "platform/io/button/ButtonSettings.h"

#include <supp/inline.h>

namespace platform {

// clang-format off
enum class [[nodiscard]] ButtonEvent : uint8_t {  // 3 bits
    //             0b.XXX....
    None         = 0b00000000,
    Pressed      = 0b00010000,
    Clicked      = 0b00100000,
    HoldStarted  = 0b00110000,
    HoldReleased = 0b01000000,
    Timeout      = 0b01010000,
};
// clang-format on

constexpr const char* toString(ButtonEvent event);

namespace sm {

struct ButtonSM {
 public:
    void poll(bool engaged, const ButtonSettings& settings);

    // The number of clicks preceding the event
    uint8_t clicks() const { return clicks_; }

    // The event that happened (reset on tick())
    SUPP_INLINE ButtonEvent event() { return static_cast<ButtonEvent>(state_ & EventMask); }

    bool busy() const;
    bool pressing() const;
    bool holding() const;
    bool waiting() const;

 protected:
    SUPP_INLINE void isr() { state_ = State(state_ | ISRMask); }

 private:
    // clang-format off
    static constexpr uint8_t ISRMask   = 0b10000000;
    static constexpr uint8_t EventMask = 0b01110000;
    static constexpr uint8_t StateMask = 0b00001110;
    static constexpr uint8_t PinMask   = 0b00000001;

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

    SUPP_INLINE void clearEvent();
    SUPP_INLINE void set(State state, ButtonEvent event);
    SUPP_INLINE uint8_t updateStateClearEvent(bool engaged);

    uint16_t timer_ = 0;
    uint8_t clicks_ = 0;
    volatile State state_ = Idle;
};

}  // namespace sm

template <type::DigitalInputPin Pin, ButtonSettings S = {}>
class Button : public sm::ButtonSM, public Singleton<Button<Pin, S>> {
 public:
    Button() = default;

    void init() {
        pin.init();

        if constexpr (type::InterruptPin<Pin>) {
            pin.interrupt().attach(Button::buttonISR, interruptMode(S.level));
        }
    }

    void tick() {
        if constexpr (S.level == ButtonLevel::High) {
            poll(pin.read(), S);
        } else {
            poll(!pin.read(), S);
        }
    }

 private:
    static void buttonISR() { Button::instance().isr(); }

    [[no_unique_address]] const Pin pin{};
};

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

}  // namespace platform

#define PLATFORM_BUTTON_ISR(Pin, ...) \
    template PLATFORM_ISR void ::platform::Button<Pin, ##__VA_ARGS__>::buttonISR()
