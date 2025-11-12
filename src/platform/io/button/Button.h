#pragma once

#include "platform/Singleton.h"
#include "platform/hal/int/vector.h"  // IWYU pragma: keep
#include "platform/hal/pin/type.h"
#include "platform/io/button/ButtonSettings.h"
#include "platform/util/isr_template.h"

#include <supp/inline.h>

namespace platform {

// clang-format off
enum class [[nodiscard]] ButtonEvent : uint8_t {  // 3 bits
    //             0b.XXX....
    None         = 0b00000000,
    Pressed      = 0b00010000,
    Released     = 0b00100000,
    Clicked      = 0b00110000,
    HoldStarted  = 0b01000000,
    Timeout      = 0b01010000,
};
// clang-format on

constexpr const char* toString(ButtonEvent event);

namespace sm {

struct ButtonSM {
 public:
    ButtonEvent event() const { return static_cast<ButtonEvent>(state_ & EventMask); }
    uint8_t clicks() const { return clicks_; }
    bool busy() const;
    bool holding() const;
    bool waiting() const;
    bool pressing() const;

 protected:
    void poll(bool engaged, const ButtonSettings& settings);
    SUPP_INLINE void isr() { state_ = State(state_ | ISRMask); }
    void suspendIfPressing();

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
        PressSuspended  = 0b00001110,
    };
    // clang-format on

    void clearEvent();
    void set(State state, ButtonEvent event);
    uint8_t updateStateClearEvent(bool engaged);

    uint16_t timer_ = 0;
    uint8_t clicks_ = 0;
    volatile State state_ = Idle;
};

}  // namespace sm

template <type::DigitalInputPin Pin, ButtonSettings S = ButtonSettings{}>
class Button : public sm::ButtonSM, public Singleton<Button<Pin, S>> {
 public:
    Button() = default;

    void init() {
        pin.init();

        if constexpr (type::InterruptPin<Pin>) {
            pin.interrupt().attach(interruptMode(S.level));
        }
    }

    void tick() {
        if constexpr (S.level == ButtonLevel::High) {
            poll(pin.read(), S);
        } else {
            poll(!pin.read(), S);
        }
    }

    static void buttonISR() { Button::instance().isr(); }

 private:
    [[no_unique_address]] const Pin pin{};
};

constexpr const char* toString(ButtonEvent event) {
    switch (event) {
        case ButtonEvent::None:
            return "None";
        case ButtonEvent::Pressed:
            return "Pressed";
        case ButtonEvent::Released:
            return "Released";
        case ButtonEvent::Clicked:
            return "Clicked";
        case ButtonEvent::HoldStarted:
            return "HoldStarted";
        case ButtonEvent::Timeout:
            return "Timeout";
    }

    __builtin_unreachable();
}

}  // namespace platform

#define PLATFORM_BUTTON_ISR(Pin, ...)                                                            \
    PLATFORM_INSTANTIATE_ISR_TEMPLATE(void ::platform::Button<Pin, ##__VA_ARGS__>::buttonISR()); \
    PLATFORM_DEFINE_INT_VECTOR(Pin, ::platform::Button<Pin, ##__VA_ARGS__>::buttonISR)
