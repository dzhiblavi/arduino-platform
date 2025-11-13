#include <platform/sys/config.h>  // IWYU pragma: keep

#include <utest/utest.h>

#if defined(PLATFORM_NATIVE)

#include <platform/hal/emulator/Emulator.h>
#include <platform/hal/int/make.h>
#include <platform/hal/pin/make.h>

#include <platform/io/button/Encoder.h>

#include <time/mono.h>

namespace platform {

struct t_encoder {
    using S1 = Pin<1, GPIOMode::Input>;
    using S2 = Pin<2, GPIOMode::Input>;

    t_encoder() {
        b.init();
        ttime::mono::set(ttime::Time(0));
    }

    ~t_encoder() { TEST_ASSERT_EQUAL(EncoderEvent::None, event); }

    void expect(int counter, EncoderEvent event) {
        this->counter = counter;
        this->event = event;
    }

    void tick() {
        b.tick();
        TEST_ASSERT_EQUAL(event, b.event());
        TEST_ASSERT_EQUAL(counter, b.counter());
        event = EncoderEvent::None;
    }

    void toggle1() {
        e.togglePin(1);
        e.raiseInterrupt<1>(InterruptMode::Change);
    }

    void toggle2() {
        e.togglePin(2);
        e.raiseInterrupt<2>(InterruptMode::Change);
    }

    void right() {
        toggle1();
        toggle2();
        toggle1();
        toggle2();
    }

    void left() {
        toggle2();
        toggle1();
        toggle2();
        toggle1();
    }

    const int pin = 1;
    Emulator& e = Emulator::instance();
    Encoder<S1, S2> b;

    int counter = 0;
    EncoderEvent event = EncoderEvent::None;
};

PLATFORM_ENCODER_ISR(t_encoder::S1, t_encoder::S2);

TEST_F(t_encoder, turn_right_once) {
    expect(1, EncoderEvent::TurnRight);
    right();
    tick();
}

TEST_F(t_encoder, turn_left_once) {
    expect(-1, EncoderEvent::TurnLeft);
    left();
    tick();
}

TEST_F(t_encoder, turn_right_multiple) {
    expect(5, EncoderEvent::TurnRight);
    right();
    right();
    right();
    right();
    right();
    tick();
}

TEST_F(t_encoder, turn_left_multiple) {
    expect(-5, EncoderEvent::TurnLeft);
    left();
    left();
    left();
    left();
    left();
    tick();
}

TEST_F(t_encoder, turn_mixed_no_ticks) {
    expect(-2, EncoderEvent::TurnLeft);
    right();
    right();
    left();
    right();
    left();
    left();
    left();
    left();
    tick();
}

TEST_F(t_encoder, turn_mixed_with_ticks) {
    expect(1, EncoderEvent::TurnRight);
    right();
    tick();

    expect(2, EncoderEvent::TurnRight);
    right();
    tick();

    expect(1, EncoderEvent::TurnLeft);
    left();
    tick();

    expect(2, EncoderEvent::TurnRight);
    right();
    tick();

    expect(1, EncoderEvent::TurnLeft);
    left();
    tick();

    expect(0, EncoderEvent::TurnLeft);
    left();
    tick();

    expect(-1, EncoderEvent::TurnLeft);
    left();
    tick();

    expect(-2, EncoderEvent::TurnLeft);
    left();
    tick();
}

}  // namespace platform

#endif

TESTS_MAIN
