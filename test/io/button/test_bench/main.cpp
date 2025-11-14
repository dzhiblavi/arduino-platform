#define LOGGING_DISABLED

#include "io/button/emulator/emulator.h"
#include "pinout.h"

#include <platform/io/button/Button.h>
#include <platform/io/button/GyverButton.h>

#include <bench/Histogram.h>
#include <bench/bench.h>

#include <utest/utest.h>

using namespace platform;

constexpr uint32_t Repeat = 1000;

// total ticks: 113355, ticks/op: 113, max ticks/op: 189, us/op: 7.08
// total ticks: 185473, ticks/op: 185, max ticks/op: 209, us/op: 11.59
using B = Button<P1, buttonSettings>;
PLATFORM_BUTTON_ISR(P1, buttonSettings);

// total ticks: 117851, ticks/op: 117, max ticks/op: 193, us/op: 7.37
// total ticks: 306856, ticks/op: 306, max ticks/op: 310, us/op: 19.18
// using B = GyverButton<P1, buttonSettings>;
// PLATFORM_GYVER_BUTTON_ISR(P1, buttonSettings);

void dumpStats(auto& hist, uint32_t total_ticks, uint32_t max_ticks) {
    float us = static_cast<float>(total_ticks) / (F_CPU / 1000000) / Repeat;

    for (size_t i = 0; i < hist.BucketsCount; ++i) {
        if (hist.buckets[i] != 0) {
            Serial.print(hist.bucketEdge(i));
            Serial.print('\t');
        }
    }

    Serial.println();

    for (auto&& bucket : hist.buckets) {
        if (bucket) {
            Serial.print(bucket);
            Serial.print('\t');
        }
    }

    Serial.println();
    Serial.print("total ticks: ");
    Serial.print(total_ticks);
    Serial.print(", ticks/op: ");
    Serial.print(total_ticks / Repeat);
    Serial.print(", max ticks/op: ");
    Serial.print(max_ticks);
    Serial.print(", us/op: ");
    Serial.println(us, 2);
    Serial.flush();
}

template <std::same_as<t::Step>... Steps>
void runBenchmark(Steps... steps) {
    t::Step arr[] = {steps...};
    Emulator emulator(arr, sizeof...(steps));
    emulator.init();

    uint64_t total_ticks = 0;
    uint32_t max_tick = 0;
    auto hist = bench::histogram<12, 1>();
    B button;
    button.init();

    for (uint32_t i = 0; i < Repeat; ++i) {
        for (;;) {
            bool resume = emulator.loop();

            bench::start();
            button.tick();
            auto ticks = bench::end();

            hist.add(ticks);
            total_ticks += ticks;
            max_tick = max(max_tick, ticks);

            if (!resume) {
                break;
            }
        }
    }

    dumpStats(hist, total_ticks, max_tick);
}

TEST(benchmark_idle) {
    runBenchmark(t::delay(0, 5));
}

TEST(benchmark_click) {
    runBenchmark(t::press(0), t::delay(0, 5), t::release(1, ButtonEvent::Clicked), t::delay(1, 5));
}

TESTS_MAIN

#include <platform/hal/int/instantiate_vectors.h>
