#include <platform/sys/config.h>  // IWYU pragma: keep

#include <utest/utest.h>

#if defined(PLATFORM_NATIVE)

#include <platform/hal/emulator/Emulator.h>
#include <platform/hal/int/make.h>
#include <platform/hal/pin/make.h>

#include <platform/io/button/Button.h>

namespace platform {

struct t_emulator {
    Emulator& e = Emulator::instance();
};

TEST_F(t_emulator, example) {
    Button<Pin<1, GPIOMode::InputPullup>, ButtonLevel::High> button;

    button.init();
    button.tick();
}

}  // namespace platform

#endif

TESTS_MAIN
