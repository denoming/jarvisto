#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "jarvis/Worker.hpp"

#include <future>
#include <iostream>

using namespace jar;
using namespace testing;

TEST(WorkerTest, StartStop)
{
    Worker worker;
    EXPECT_FALSE(worker.active());
    worker.start();
    EXPECT_TRUE(worker.active());
    worker.stop();
    EXPECT_FALSE(worker.active());
}

TEST(WorkerTest, Post)
{
    Worker worker{2};
    worker.start();

    MockFunction<void()> callback;
    EXPECT_CALL(callback, Call());

    std::promise<void> waiter;
    io::post(worker.executor(), [&]() {
        callback.Call();
        waiter.set_value();
    });

    auto future = waiter.get_future();
    EXPECT_EQ(future.wait_for(std::chrono::seconds{1}), std::future_status::ready);
}