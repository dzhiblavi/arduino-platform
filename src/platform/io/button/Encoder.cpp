#include "platform/io/button/Encoder.h"

namespace platform {

namespace sm {

void EncoderSM::tick() {
    if (s & Isr) {
        s &= ~Isr;
        s |= Turn;
    } else if (s & Turn) {
        s &= ~Turn;
    }
}

EncoderEvent EncoderSM::event() const {
    switch (s & (Turn | Dir)) {
        case 0b0 | 0b0:
            return EncoderEvent::None;

        case Turn | 0b0:
            return EncoderEvent::TurnLeft;

        case Turn | Dir:
            return EncoderEvent::TurnRight;
    }

    return EncoderEvent::None;
}

PLATFORM_ISR EncoderSM::tickISR(uint8_t e0, uint8_t e1, const EncoderSettings& e) {
    const int8_t res = poll(e0, e1, e);
    if (res == 0) {
        return;
    }

    s |= Isr;
    (res > 0) ? s |= Dir : s &= ~Dir;
}

PLATFORM_RAM int8_t EncoderSM::poll(uint8_t e0, uint8_t e1, const EncoderSettings& S) {
    if ((p0 ^ p1 ^ e0 ^ e1) == 0) {
        return 0;
    }

    if (p1 ^ e0) {
        epos += 1;
    } else {
        epos -= 1;
    }

    p0 = e0;  // NOLINT
    p1 = e1;  // NOLINT

    if (epos == 0) {
        return 0;
    }

    switch (S.type) {
        case Step4Low:
            if (!(e0 & e1)) {
                return 0;
            }
            break;
        case Step4High:
            if (e0 | e1) {
                return 0;
            }
            break;
        case Step2:
            if (e0 ^ e1) {
                return 0;
            }
            break;
        case Step1:
            break;
    }

    int8_t direction = epos > 0 ? -1 : +1;
    epos = 0;
    counter_ += direction;  // NOLINT
    return direction;
}

}  // namespace sm

}  // namespace platform
