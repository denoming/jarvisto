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

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "jarvisto/network/MqttAsyncClient.hpp"
#include "test/Waiter.hpp"

#include <fmt/format.h>

using namespace testing;
using namespace jar;

class MqttAsyncClientTest : public Test {
public:
    static inline const std::string_view kMqttUser{"<username>>"};
    static inline const std::string_view kMqttPassword{"<password>"};
    static inline const std::string_view kMqttServer{"<server-ip-address>"};
    static inline const uint16_t kMqttPort{1883};
};

TEST_F(MqttAsyncClientTest, DISABLED_Connecting)
{
    io::io_context context;

    MqttAsyncClient client{context.get_executor()};
    io::co_spawn(
        context.get_executor(),
        [&]() -> io::awaitable<void> {
            EXPECT_EQ(client.credentials(kMqttUser, kMqttPassword), MqttErrorCode::Success);
            auto [ec, rc] = co_await client.connect(kMqttServer, kMqttPort, 60, io::use_awaitable);
            EXPECT_EQ(ec, MqttErrorCode::Success);
            EXPECT_EQ(rc, MqttReturnCode::Accepted);
            if (ec) {
                co_return;
            }

            std::tie(ec, rc) = co_await client.disconnect(io::use_awaitable);
            EXPECT_EQ(ec, MqttErrorCode::Success);
            EXPECT_EQ(rc, MqttReturnCode::Accepted);
        },
        io::detached);

    context.run();
}

TEST_F(MqttAsyncClientTest, DISABLED_Publishing)
{
    static const std::string_view kPayload{"Hello World!"};
    static const std::string_view kTopic{"jarvisto/test"};

    io::io_context context;
    MqttAsyncClient client{context.get_executor()};

    MockFunction<MqttAsyncClient::OnMessage> callback;
    EXPECT_CALL(callback, Call(Not(0), Eq(kTopic), Not(IsNull()), Gt(0)));

    Waiter<MqttAsyncClient::OnMessage> waiter;
    std::ignore = client.onMessage().connect(
        waiter.enroll([&callback](int mid, std::string_view topic, void* payload, size_t size) {
            if (topic == kTopic) {
                callback.Call(mid, topic, payload, size);
            }
        }));

    io::co_spawn(
        context.get_executor(),
        [&]() -> io::awaitable<void> {
            EXPECT_EQ(client.credentials(kMqttUser, kMqttPassword), MqttErrorCode::Success);
            auto [ec, rc] = co_await client.connect(kMqttServer, kMqttPort, 60, io::use_awaitable);
            EXPECT_EQ(ec, MqttErrorCode::Success);
            EXPECT_EQ(rc, MqttReturnCode::Accepted);
            if (ec) {
                co_return;
            }

            ec = co_await client.subscribe(kTopic, MqttQoS::Level2, io::use_awaitable);
            EXPECT_EQ(ec, MqttErrorCode::Success);
            if (ec) {
                co_return;
            }

            ec = co_await client.publish(
                kTopic, kPayload, MqttQoS::Level2, false, io::use_awaitable);
            EXPECT_EQ(ec, MqttErrorCode::Success);
            if (ec) {
                co_return;
            }

            EXPECT_TRUE(waiter.wait(std::chrono::seconds{1}));

            ec = co_await client.unsubscribe(kTopic, io::use_awaitable);
            EXPECT_EQ(ec, MqttErrorCode::Success);

            std::ignore = co_await client.disconnect(io::use_awaitable);
        },
        io::detached);

    context.run();
}