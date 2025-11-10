#pragma once

#include "platform/Singleton.h"
#include "platform/hal/pin/type.h"
#include "platform/io/button/EncoderSettings.h"

#include <supp/inline.h>

namespace platform {

namespace sm {

struct EncoderSM {
 public:
    int8_t tick();
    SUPP_INLINE bool turning() const { return s & Turn; }
    SUPP_INLINE bool direction() const { return s & Dir; }
    SUPP_INLINE int8_t counter() const { return counter_; }

 protected:
    void tickISR(uint8_t e0, uint8_t e1, const EncoderSettings& e);

 private:
    int8_t poll(uint8_t e0, uint8_t e1, const EncoderSettings& s);

    enum : uint8_t {
        P0 = 1 << 0,
        P1 = 1 << 1,
        Turn = 1 << 2,
        Dir = 1 << 3,
        Isr = 1 << 4,
    };

    volatile uint8_t s = 0;
    volatile int8_t counter_ = 0;
    int8_t pos = 0;
};

}  // namespace sm

template <type::InterruptPin S1, type::InterruptPin S2, EncoderSettings S = {}>
class Encoder : public sm::EncoderSM, public Singleton<Encoder<S1, S2, S>> {
 public:
    Encoder() = default;

    void init() {
        s1.init();
        s2.init();
        s1.interrupt().attach(Encoder::encoderISR, InterruptMode::Change);
        s2.interrupt().attach(Encoder::encoderISR, InterruptMode::Change);
    }

 private:
    static void encoderISR() { Encoder::instance().tickISR(S1().read(), S2().read(), S); }

    [[no_unique_address]] const S1 s1{};
    [[no_unique_address]] const S2 s2{};
};

}  // namespace platform

#define PLATFORM_ENCODER_ISR(S1, S2, ...) \
    template PLATFORM_ISR void ::platform::Encoder<S1, S2, ##__VA_ARGS__>::encoderISR()
