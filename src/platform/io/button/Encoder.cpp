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

PLATFORM_RAM void EncoderSM::tickISR(uint8_t e0, uint8_t e1, const EncoderSettings& e) {
    const int8_t res = poll(e0, e1, e);
    if (res == 0) {
        return;
    }

    s |= Isr;
    (res > 0) ? s |= Dir : s &= ~Dir;
}

PLATFORM_RAM int8_t EncoderSM::poll(uint8_t e0, uint8_t e1, const EncoderSettings& S) {
    const uint8_t p0 = s & P0;
    const uint8_t p1 = (s & P1) >> 1;

    if ((p0 ^ p1 ^ e0 ^ e1) == 0b0) {
        return 0;
    }

    (p1 ^ e0) ? pos += 1 : pos -= 1;
    e0 ? s |= P0 : s &= ~P0;
    e1 ? s |= P1 : s &= ~P1;

    if (pos == 0) {
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
        default:
            break;
    }

    int8_t result = pos > 0 ? -1 : +1;
    pos = 0;
    counter_ = counter_ + result;  // NOLINT
    return result;
}

}  // namespace sm

}  // namespace platform
