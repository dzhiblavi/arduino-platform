#include "platform/io/button/Encoder.h"

namespace platform {

namespace sm {

namespace {

static constexpr int8_t lookupTable[16] = {
    /* idx = old<<2 | new  (old = previous AB, new = current AB) */
    /* 0b0000 */ +0,  // 00 → 00   (no movement)
    /* 0b0001 */ +1,  // 00 → 01   (clockwise)
    /* 0b0010 */ -1,  // 00 → 10   (counter‑clockwise)
    /* 0b0011 */ +0,  // 00 → 11   (illegal jump)

    /* 0b0100 */ -1,  // 01 → 00
    /* 0b0101 */ +0,  // 01 → 01
    /* 0b0110 */ +0,  // 01 → 10   (illegal – both change)
    /* 0b0111 */ +1,  // 01 → 11

    /* 0b1000 */ +1,  // 10 → 00
    /* 0b1001 */ +0,  // 10 → 01   (illegal)
    /* 0b1010 */ +0,  // 10 → 10
    /* 0b1011 */ -1,  // 10 → 11

    /* 0b1100 */ +0,  // 11 → 00   (illegal)
    /* 0b1101 */ -1,  // 11 → 01
    /* 0b1110 */ +1,  // 11 → 10
    /* 0b1111 */ +0   // 11 → 11   (no movement)
};

SUPP_INLINE int8_t lookupDirection(uint8_t t) {
    return lookupTable[t & 0b1111];
}

}  // namespace

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
    uint8_t s_old = s & (P0 | P1);
    uint8_t s_new = (e1 << 1) | e0;
    if (s_old == s_new) {
        return 0;
    }

    s = (s & ~(P0 | P1)) | s_new;

    uint8_t transition = (s_old << 2) | s_new;
    int8_t direction = lookupDirection(transition);
    if (direction == 0) {
        return 0;
    }

    switch (S.type) {
        case Step4Low:
            if (s_new != 0b11) {
                return 0;
            }
            break;

        case Step4High:
            if (s_new != 0b00) {
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

    counter_ = counter_ + direction;  // NOLINT
    return direction;
}

}  // namespace sm

}  // namespace platform
