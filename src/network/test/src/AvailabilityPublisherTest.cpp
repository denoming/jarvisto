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

#include "jarvisto/network/AvailabilityPublisher.hpp"
#include "network/MockMqttClient.hpp"
#include "network/MockAvailabilitySubject.hpp"

using namespace jar;
using namespace testing;

class AvailabilityPublisherTest : public Test {
public:
    MockAvailabilitySubject subject;
    MockMqttClient client;
};

TEST_F(AvailabilityPublisherTest, CheckStateUpdating)
{
    EXPECT_CALL(client, doHasConnection).WillOnce(Return(false));
    EXPECT_CALL(subject, state).WillOnce(Return(AvailabilityState::Offline));

    AvailabilityPublisher publisher{"service", client, subject};

    EXPECT_CALL(client, doHasConnection).WillOnce(Return(true)).RetiresOnSaturation();
    EXPECT_CALL(client, doPublish(_, HasSubstr("Online"), _, _)).RetiresOnSaturation();
    subject.triggerStateUpdate("subject", AvailabilityState::Online);

    EXPECT_CALL(client, doHasConnection).WillOnce(Return(true)).RetiresOnSaturation();
    EXPECT_CALL(client, doPublish(_, HasSubstr("Offline"), _, _)).RetiresOnSaturation();
    subject.triggerStateUpdate("subject", AvailabilityState::Offline);

    EXPECT_CALL(client, doHasConnection).WillOnce(Return(false)).RetiresOnSaturation();
    subject.triggerStateUpdate("subject", AvailabilityState::Online);

    EXPECT_CALL(client, doPublish(_, HasSubstr("Online"), _, _)).RetiresOnSaturation();
    client.notifyConnect(MqttReturnCode::Accepted);
}