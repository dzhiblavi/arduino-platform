#include "platform/io/button/Encoder.h"

namespace platform {

namespace sm {

int8_t EncoderSM::tick() {
    switch (s & Isr) {
        case 0b0:
            if (s & Turn) {
                s &= ~Turn;
            }
            return 0;
        case Isr:
            s &= ~Isr;
            s |= Turn;
            return (s & Dir) ? +1 : -1;
    }

    return 0;
}

void EncoderSM::tickISR(uint8_t e0, uint8_t e1, const EncoderSettings& e) {
    const int8_t res = poll(e0, e1, e);
    if (res == 0) {
        return;
    }

    s |= Isr;
    (res > 0) ? s |= Dir : s &= ~Dir;
}

int8_t EncoderSM::poll(uint8_t e0, uint8_t e1, const EncoderSettings& S) {
    uint8_t p0 = s & P0;
    uint8_t p1 = (s & P1) >> 1;

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
    counter_ += result;  // NOLINT
    return result;
}

}  // namespace sm

}  // namespace platform
