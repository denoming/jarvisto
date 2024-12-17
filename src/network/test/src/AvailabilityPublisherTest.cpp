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
    client.triggerConnect(MqttReturnCode::Accepted);
}