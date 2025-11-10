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

    struct Consumer : Callback<Button<Pin>&> {
        ~Consumer() { TEST_ASSERT_EQUAL(0, calls); }

        void run(Button<Pin>& b) override {
            TEST_ASSERT_NOT_EQUAL(0, calls);
            TEST_ASSERT_EQUAL(clicks, b.clicks());
            TEST_ASSERT_EQUAL(event, b.event());

            --calls;
            clicks = 0;
            event = ButtonEvent::None;
        }

        void expect(int clicks, ButtonEvent event) {
            ++calls;
            this->clicks = clicks;
            this->event = event;
        }

        int calls = 0;
        int clicks = 0;
        ButtonEvent event = ButtonEvent::None;
    };

    t_button() {
        b.init();
        b.setCallback(&c);
        ttime::mono::set(ttime::Time(0));
    }

    void skipMillis(int millis) {
        ttime::mono::advance(ttime::Duration(millis) * ttime::Millisecond);
    }

    void tickAfter(int millis) {
        skipMillis(millis);
        b.tick();
    }

    void setBounce(uint8_t value) {
        e.writePin(pin, value);  // engage button
        b.tick();

        for (int i = 0; i < 3; ++i) {  // 40ms bounce
            skipMillis(10);
            e.writePin(pin, i & 0b1);
            b.tick();
        }

        skipMillis(20);  // proceed to waiting
        e.writePin(pin, value);
        b.tick();
    }

    void pressBounce() { setBounce(1); }
    void releaseBounce() { setBounce(0); }

    void skipToTimeout() {
        TEST_ASSERT_EQUAL(0, Pin().read());  // should not be engaged
        skipMillis(1000);
        b.tick();
    }

    const int pin = 1;
    Emulator& e = Emulator::instance();
    Consumer c;
    Button<Pin> b;
};

TEST_F(t_button, press_bounce) {
    c.expect(0, ButtonEvent::Pressed);
    pressBounce();
}

TEST_F(t_button, single_click) {
    c.expect(0, ButtonEvent::Pressed);
    pressBounce();

    c.expect(1, ButtonEvent::Clicked);
    skipMillis(100);
    releaseBounce();

    c.expect(0, ButtonEvent::Timeout);
    skipToTimeout();
}

TEST_F(t_button, multi_click) {
    c.expect(0, ButtonEvent::Pressed);
    pressBounce();

    c.expect(1, ButtonEvent::Clicked);
    skipMillis(100);
    releaseBounce();

    c.expect(1, ButtonEvent::Pressed);
    skipMillis(100);
    pressBounce();

    c.expect(2, ButtonEvent::Clicked);
    skipMillis(100);
    releaseBounce();

    c.expect(0, ButtonEvent::Timeout);
    skipToTimeout();
}

TEST_F(t_button, hold) {
    c.expect(0, ButtonEvent::Pressed);
    pressBounce();

    c.expect(0, ButtonEvent::HoldStarted);
    tickAfter(2000);

    c.expect(0, ButtonEvent::HoldReleased);
    skipMillis(1000);
    releaseBounce();

    c.expect(0, ButtonEvent::Timeout);
    skipToTimeout();
}

TEST_F(t_button, clicks_then_hold) {
    c.expect(0, ButtonEvent::Pressed);
    pressBounce();

    c.expect(1, ButtonEvent::Clicked);
    skipMillis(100);
    releaseBounce();

    c.expect(1, ButtonEvent::Pressed);
    skipMillis(100);
    pressBounce();

    c.expect(2, ButtonEvent::Clicked);
    skipMillis(100);
    releaseBounce();

    c.expect(2, ButtonEvent::Pressed);
    skipMillis(100);
    pressBounce();

    c.expect(2, ButtonEvent::HoldStarted);
    tickAfter(1000);

    c.expect(0, ButtonEvent::HoldReleased);
    skipMillis(100);
    releaseBounce();

    c.expect(0, ButtonEvent::Timeout);
    skipToTimeout();
}

TEST_F(t_button, hold_then_clicks) {
    c.expect(0, ButtonEvent::Pressed);
    pressBounce();

    c.expect(0, ButtonEvent::HoldStarted);
    tickAfter(2000);

    c.expect(0, ButtonEvent::HoldReleased);
    skipMillis(1000);
    releaseBounce();

    c.expect(0, ButtonEvent::Pressed);
    skipMillis(100);
    pressBounce();

    c.expect(1, ButtonEvent::Clicked);
    skipMillis(100);
    releaseBounce();

    c.expect(1, ButtonEvent::Pressed);
    skipMillis(100);
    pressBounce();

    c.expect(2, ButtonEvent::Clicked);
    skipMillis(100);
    releaseBounce();

    c.expect(0, ButtonEvent::Timeout);
    skipToTimeout();
}

}  // namespace platform

#endif

TESTS_MAIN
