#pragma once

#include "platform/Singleton.h"
#include "platform/hal/int/vector.h"  // IWYU pragma: keep
#include "platform/hal/pin/type.h"
#include "platform/io/button/EncoderSettings.h"
#include "platform/util/isr_template.h"

// #define EB_NO_CALLBACK // THIS ENABLES BUGS IN THE LIBRARY
#include <core/VirtEncoder.h>

namespace platform {

template <type::InterruptPin S1, type::InterruptPin S2, EncoderSettings S = EncoderSettings{}>
class GyverEncoder : public VirtEncoder, public Singleton<GyverEncoder<S1, S2, S>> {
 public:
    GyverEncoder() = default;

    void init() {
        setEncISR(true);
        setEncType(getType(S.type));

        s1.init();
        s2.init();
        s1.interrupt().attach(InterruptMode::Change);
        s2.interrupt().attach(InterruptMode::Change);
    }

    void tick() { VirtEncoder::tick(); }
    static void encoderISR() { GyverEncoder::instance().tickISR(S1().read(), S2().read()); }

 private:
    static constexpr int getType(EncoderType type) {
        switch (type) {
            case Step4Low:
                return EB_STEP4_LOW;
            case Step4High:
                return EB_STEP4_HIGH;
            case Step2:
                return EB_STEP2;
            case Step1:
                return EB_STEP1;
        }
    }

    [[no_unique_address]] const S1 s1{};
    [[no_unique_address]] const S2 s2{};
};

}  // namespace platform

#define PLATFORM_ENCODER_ISR(S1, S2, ...)                                                        \
    PLATFORM_INSTANTIATE_ISR_TEMPLATE(                                                           \
        ::platform::GyverEncoder<S1, S2, ##__VA_ARGS__>::encoderISR());                          \
    PLATFORM_DEFINE_INT_VECTOR(S1, ::platform::GyverEncoder<S1, S2, ##__VA_ARGS__>::encoderISR); \
    PLATFORM_DEFINE_INT_VECTOR(S2, ::platform::GyverEncoder<S1, S2, ##__VA_ARGS__>::encoderISR)
