#include "emulator.h"
#include "tester.h"

#include <utest/utest.h>

using namespace platform;

template <std::same_as<t::Step>... Steps>
void runTest(Steps... steps) {
    t::Step arr[] = {steps...};

    Emulator emulator(arr, sizeof...(steps));
    Tester tester(arr, sizeof...(steps));

    emulator.init();
    tester.init();

    for (;;) {
        bool x = emulator.loop();
        bool y = tester.loop();
        TEST_ASSERT_TRUE(tester.ok());

        if (!x && !y) {
            break;
        }
    }
}

TEST(single_click) {
    runTest(
        t::press(0),
        t::delay(0, 100),
        t::release(1, ButtonEvent::Clicked),
        t::delay(0, 600, ButtonEvent::Timeout));
}

TEST(single_hold) {
    runTest(
        t::press(0),
        t::delay(0, 600, ButtonEvent::HoldStarted),
        t::release(0, ButtonEvent::Released),
        t::delay(0, 600, ButtonEvent::Timeout));
}

TEST(multiple_clicks) {
    runTest(
        t::press(0),
        t::delay(0, 100),
        t::release(1, ButtonEvent::Clicked),
        t::delay(1, 100),
        t::press(1),
        t::delay(1, 100),
        t::release(2, ButtonEvent::Clicked),
        t::delay(1, 100),
        t::press(2),
        t::delay(2, 100),
        t::release(3, ButtonEvent::Clicked),
        t::delay(0, 600, ButtonEvent::Timeout));
}

TEST(clicks_hold_clicks) {
    runTest(
        t::press(0),
        t::delay(0, 100),
        t::release(1, ButtonEvent::Clicked),
        t::delay(1, 100),
        t::press(1),
        t::delay(1, 100),
        t::release(2, ButtonEvent::Clicked),
        t::delay(1, 100),
        t::press(2),
        t::delay(2, 600, ButtonEvent::HoldStarted),
        t::release(0, ButtonEvent::Released),
        t::delay(1, 100),
        t::press(0),
        t::delay(0, 100),
        t::release(1, ButtonEvent::Clicked),
        t::delay(1, 100),
        t::press(1),
        t::delay(1, 100),
        t::release(2, ButtonEvent::Clicked),
        t::delay(0, 600, ButtonEvent::Timeout));
}

TEST(suspend_no_click) {
    runTest(
        t::press(0),
        t::suspend(0),
        t::delay(0, 200),
        t::release(0, ButtonEvent::Released),
        t::delay(0, 1000, ButtonEvent::Timeout));
}

TEST(suspend_no_hold) {
    runTest(
        t::press(0),
        t::suspend(0),
        t::delay(0, 700),
        t::release(0, ButtonEvent::Released),
        t::delay(0, 700, ButtonEvent::Timeout));
}

TEST(unsuspend_clicks) {
    runTest(
        t::press(0),
        t::suspend(0),
        t::delay(0, 100),
        t::release(0, ButtonEvent::Released),
        t::delay(1, 100),
        t::press(0),
        t::delay(0, 100),
        t::release(1, ButtonEvent::Clicked),
        t::delay(0, 700, ButtonEvent::Timeout));
}

TEST(unsuspend_hold) {
    runTest(
        t::press(0),
        t::suspend(0),
        t::delay(0, 100),
        t::release(0, ButtonEvent::Released),
        t::delay(1, 100),
        t::press(0),
        t::delay(0, 700, ButtonEvent::HoldStarted),
        t::release(0, ButtonEvent::Released),
        t::delay(0, 700, ButtonEvent::Timeout));
}

TESTS_MAIN

#include <platform/hal/int/instantiate_vectors.h>
