#pragma once

#include "jarvisto/network/MqttClient.hpp"

#include <gmock/gmock.h>

namespace jar {

class MockMqttClient : public MqttClient {
public:
    MockMqttClient();

    using MqttClient::notifyConnect;
    using MqttClient::notifyDisconnect;
    using MqttClient::notifySubscribe;
    using MqttClient::notifyUnsubscribe;
    using MqttClient::notifyPublish;
    using MqttClient::notifyMessage;
    using MqttClient::notifyLog;

public:
    MOCK_METHOD(bool, doHasConnection, (), (override, const));

    MOCK_METHOD(std::error_code,
                doCredentials,
                (std::string_view user, std::string_view password),
                (override, const));

    MOCK_METHOD(std::error_code,
                doConnect,
                (std::string_view host, int port, int keepAlive),
                (override, const));

    MOCK_METHOD(std::error_code,
                doConnectAsync,
                (std::string_view host, int port, int keepAlive),
                (override, const));

    MOCK_METHOD(std::error_code, doReconnect, (), (override, const));

    MOCK_METHOD(std::error_code, doReconnectAsync, (), (override, const));

    MOCK_METHOD(std::error_code, doDisconnect, (), (override, const));

    MOCK_METHOD((std::expected<int, std::error_code>),
                doSubscribe,
                (std::string_view topic, MqttQoS qos),
                (override, const));

    MOCK_METHOD((std::expected<int, std::error_code>),
                doUnsubscribe,
                (std::string_view topic),
                (override, const));

    MOCK_METHOD(
        (std::expected<int, std::error_code>),
        doPublish,
        (std::string_view topic, const void* payload, std::size_t size, MqttQoS qos, bool retain),
        (override, const));

    MOCK_METHOD((std::expected<int, std::error_code>),
                doPublish,
                (std::string_view topic, std::string_view payload, MqttQoS qos, bool retain),
                (override, const));
};

} // namespace jar