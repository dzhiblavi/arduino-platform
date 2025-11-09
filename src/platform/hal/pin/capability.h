#pragma once

namespace platform {

struct IOCaps {
    bool dio = false;
    bool intr = false;
    bool adc = false;
    bool pwm = false;
    bool sda = false;
    bool scl = false;
    bool cs = false;
    bool sck = false;
    bool copi = false;
    bool cipo = false;
};

}  // namespace platform
