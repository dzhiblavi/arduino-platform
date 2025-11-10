#pragma once

#include "platform/Callback.h"
#include "platform/Singleton.h"
#include "platform/hal/pin/type.h"
#include "platform/io/button/ButtonLevel.h"

// #define EB_NO_CALLBACK // THIS ENABLES BUGS IN THE LIBRARY
#include <core/VirtEncoder.h>

namespace platform {

template <type::InterruptPin S1, type::InterruptPin S2, ButtonLevel Level = ButtonLevel::Low>
class GyverEncoder : public VirtEncoder, public Singleton<GyverEncoder<S1, S2, Level>> {
 public:
    using CallbackArgType = GyverEncoder&;
    using CallbackType = Callback<CallbackArgType>;

    GyverEncoder() = default;

    void init() {
        setEncISR(true);

        s1.init();
        s2.init();
        s1.interrupt().attach(GyverEncoder::encISR, InterruptMode::Change);
        s2.interrupt().attach(GyverEncoder::encISR, InterruptMode::Change);
    }

    void setCallback(CallbackType* callback) { callback_ = callback; }

    void tick() {
        if (VirtEncoder::tick(s1.read(), s2.read()) && callback_) {
            callback_->run(*this);
        }
    }

 private:
    static void encISR() { GyverEncoder::instance().tickISR(S1().read(), S2().read()); }

    CallbackType* callback_ = nullptr;
    [[no_unique_address]] const S1 s1{};
    [[no_unique_address]] const S2 s2{};
};

}  // namespace platform

#define PLATFORM_ENCODER_ISR(S1, S2, ...) \
    template PLATFORM_ISR void ::platform::GyverEncoder<S1, S2, ##__VA_ARGS__>::encISR()
