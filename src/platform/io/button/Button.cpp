#include "platform/io/button/Button.h"

#include <time/mono.h>

namespace platform {

namespace sm {

void ButtonSM::poll(bool engaged, const ButtonSettings& settings) {
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
            set(ReleaseDebounce, ButtonEvent::Released);
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

        case PressSuspended | 0b0:
            timer_ = now;
            set(ReleaseDebounce, ButtonEvent::Released);
            break;

        case PressSuspended | 0b1:
            break;

        case PressDebounceSuspended | 0b0:
            if (elapsed >= settings.debounce_ms) {
                timer_ = now;
                set(ReleaseDebounce, ButtonEvent::Released);
            }
            break;

        case PressDebounceSuspended | 0b1:
            if (elapsed >= settings.debounce_ms) {
                set(PressSuspended, ButtonEvent::None);
            }
            break;

        default:
            break;
    }
}

bool ButtonSM::busy() const {
    return (state_ & StateMask) != Idle;
}

bool ButtonSM::holding() const {
    return (state_ & StateMask) == Holding;
}

bool ButtonSM::waiting() const {
    return (state_ & StateMask) == Waiting;
}

bool ButtonSM::pressing() const {
    switch (state_ & StateMask) {
        case Pressing:
        case PressDebounce:
        case PressSuspended:
        case Holding:
            return true;

        default:
            return false;
    }
}

void ButtonSM::clearEvent() {
    state_ = State(state_ & ~EventMask);
}

void ButtonSM::set(State state, ButtonEvent event) {
    state_ = State(static_cast<uint8_t>(event) | state);
}

uint8_t ButtonSM::updateStateClearEvent(bool engaged) {
    state_ = State((state_ & StateMask) | engaged | ((state_ & ISRMask) == ISRMask));
    return state_;
}

void ButtonSM::suspendIfPressing() {
    switch (state_ & StateMask) {
        case Pressing:
        case Holding:
            state_ = PressSuspended;
            break;

        case PressDebounce:
            state_ = PressDebounceSuspended;
            break;

        default:
            break;
    }
}

}  // namespace sm

}  // namespace platform
