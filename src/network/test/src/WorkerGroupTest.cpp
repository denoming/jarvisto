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

#include "jarvisto/network/WorkerGroup.hpp"

#include <future>

using namespace jar;
using namespace testing;

TEST(WorkerGroupTest, StartStop)
{
    WorkerGroup group{2};
    EXPECT_FALSE(group.active());
    group.start();
    EXPECT_TRUE(group.active());
    group.stop();
    EXPECT_FALSE(group.active());
}

TEST(WorkerGroupTest, Post)
{
    WorkerGroup group{2};
    group.start();

    MockFunction<void()> callback;
    EXPECT_CALL(callback, Call()).Times(2);

    std::promise<void> w1;
    io::post(group.executor(), [&]() {
        callback.Call();
        w1.set_value();
    });
    auto f1 = w1.get_future();
    EXPECT_EQ(f1.wait_for(std::chrono::seconds{1}), std::future_status::ready);

    group.next();

    std::promise<void> w2;
    io::post(group.executor(), [&]() {
        callback.Call();
        w2.set_value();
    });
    auto f2 = w2.get_future();
    EXPECT_EQ(f2.wait_for(std::chrono::seconds{1}), std::future_status::ready);
}