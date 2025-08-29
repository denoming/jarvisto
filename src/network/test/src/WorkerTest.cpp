// Copyright 2025 Denys Asauliak
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "jarvisto/network/Worker.hpp"

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