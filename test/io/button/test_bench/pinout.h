#include <platform/hal/pin/frameworks/arduino/boards/current.h>
#include <platform/io/button/EncButtonSettings.h>

using namespace platform;

inline constexpr auto buttonSettings = ButtonSettings{
    .hold_ms = 50,
    .timeout_ms = 50,
    .debounce_ms = 8,
};

#if defined(ARDUINO_AVR_NANO)

using P1 = D2::Input<GPIOMode::Input>;

#elif defined(ARDUINO_ESP8266_WEMOS_D1MINI)

using P1 = D5::Input<GPIOMode::Input>;

#endif
