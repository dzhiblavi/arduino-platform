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

TEST(rotate_right) {
    runTest(t::right(+1));
}

TEST(rotate_left) {
    runTest(t::left(-1));
}

TEST(rotate_right_multiple) {
    runTest(t::right(+1), t::right(+2), t::right(+3), t::right(+4), t::right(+5));
}

TEST(rotate_left_multiple) {
    runTest(t::left(-1), t::left(-2), t::left(-3), t::left(-4), t::left(-5));
}

TEST(rotate_mixed) {
    runTest(
        t::left(-1),
        t::left(-2),
        t::right(-1),
        t::right(+0),
        t::right(+1),
        t::left(+0),
        t::left(-1),
        t::left(-2),
        t::left(-3),
        t::left(-4),
        t::left(-5),
        t::right(-4));
}

TESTS_MAIN

#include <platform/hal/int/instantiate_vectors.h>
