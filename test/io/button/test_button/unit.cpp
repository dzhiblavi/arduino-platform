#include <platform/sys/config.h>  // IWYU pragma: keep

#include <utest/utest.h>

#if defined(PLATFORM_NATIVE)

#include <platform/hal/emulator/Emulator.h>
#include <platform/hal/int/make.h>
#include <platform/hal/pin/make.h>

#include <platform/io/button/Button.h>

#include <time/mono.h>

namespace platform {

struct t_button {
    using Pin = Pin<1, GPIOMode::Input>;

    t_button() {
        b.init();
        ttime::mono::set(ttime::Time(0));
    }

    void expect(int clicks, ButtonEvent event) {
        this->clicks = clicks;
        this->event = event;
    }

    void tick() {
        b.tick();
        TEST_ASSERT_EQUAL(event, b.event());
        TEST_ASSERT_EQUAL(clicks, b.clicks());
        event = ButtonEvent::None;
    }

    void skipMillis(int millis) {
        ttime::mono::advance(ttime::Duration(millis) * ttime::Millisecond);
    }

    void tickAfter(int millis) {
        skipMillis(millis);
        tick();
    }

    void setBounce(uint8_t value) {
        e.writePin(pin, value);  // engage button
        tick();

        for (int i = 0; i < 3; ++i) {  // 30ms bounce
            skipMillis(10);
            e.writePin(pin, i & 0b1);
            tick();
        }

        skipMillis(20);  // proceed to waiting
        e.writePin(pin, value);
        tick();
    }

    void pressBounce() { setBounce(1); }
    void releaseBounce() { setBounce(0); }

    void skipToTimeout() {
        TEST_ASSERT_EQUAL(0, Pin().read());  // should not be engaged
        skipMillis(1000);
        tick();
    }

    const int pin = 1;
    Emulator& e = Emulator::instance();
    Button<Pin> b;

    int clicks = 0;
    ButtonEvent event = ButtonEvent::None;
};

PLATFORM_BUTTON_ISR(t_button::Pin);

TEST_F(t_button, press_bounce) {
    expect(0, ButtonEvent::Pressed);
    pressBounce();
}

TEST_F(t_button, press_from_isr) {
    expect(0, ButtonEvent::Pressed);
    e.raiseInterrupt<Pin().interrupt().number()>(InterruptMode::Rising);
}

TEST_F(t_button, single_click) {
    expect(0, ButtonEvent::Pressed);
    pressBounce();

    expect(1, ButtonEvent::Clicked);
    skipMillis(100);
    releaseBounce();

    expect(0, ButtonEvent::Timeout);
    skipToTimeout();
}

TEST_F(t_button, multi_click) {
    expect(0, ButtonEvent::Pressed);
    pressBounce();

    expect(1, ButtonEvent::Clicked);
    skipMillis(100);
    releaseBounce();

    expect(1, ButtonEvent::Pressed);
    skipMillis(100);
    pressBounce();

    expect(2, ButtonEvent::Clicked);
    skipMillis(100);
    releaseBounce();

    expect(0, ButtonEvent::Timeout);
    skipToTimeout();
}

TEST_F(t_button, hold) {
    expect(0, ButtonEvent::Pressed);
    pressBounce();

    expect(0, ButtonEvent::HoldStarted);
    tickAfter(2000);

    expect(0, ButtonEvent::Released);
    skipMillis(1000);
    releaseBounce();

    expect(0, ButtonEvent::Timeout);
    skipToTimeout();
}

TEST_F(t_button, clicks_then_hold) {
    expect(0, ButtonEvent::Pressed);
    pressBounce();

    expect(1, ButtonEvent::Clicked);
    skipMillis(100);
    releaseBounce();

    expect(1, ButtonEvent::Pressed);
    skipMillis(100);
    pressBounce();

    expect(2, ButtonEvent::Clicked);
    skipMillis(100);
    releaseBounce();

    expect(2, ButtonEvent::Pressed);
    skipMillis(100);
    pressBounce();

    expect(2, ButtonEvent::HoldStarted);
    tickAfter(1000);

    expect(0, ButtonEvent::Released);
    skipMillis(100);
    releaseBounce();

    expect(0, ButtonEvent::Timeout);
    skipToTimeout();
}

TEST_F(t_button, hold_then_clicks) {
    expect(0, ButtonEvent::Pressed);
    pressBounce();

    expect(0, ButtonEvent::HoldStarted);
    tickAfter(2000);

    expect(0, ButtonEvent::Released);
    skipMillis(1000);
    releaseBounce();

    expect(0, ButtonEvent::Pressed);
    skipMillis(100);
    pressBounce();

    expect(1, ButtonEvent::Clicked);
    skipMillis(100);
    releaseBounce();

    expect(1, ButtonEvent::Pressed);
    skipMillis(100);
    pressBounce();

    expect(2, ButtonEvent::Clicked);
    skipMillis(100);
    releaseBounce();

    expect(0, ButtonEvent::Timeout);
    skipToTimeout();
}

}  // namespace platform

#endif

TESTS_MAIN
