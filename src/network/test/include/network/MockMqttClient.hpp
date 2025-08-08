#pragma once

#include "jarvisto/network/IMqttClient.hpp"

#include <gmock/gmock.h>

namespace jar {

class MockMqttClient : public IMqttClient {
public:
    MockMqttClient();

    void
    triggerConnect(MqttReturnCode returnCode) const;

    void
    triggerDisconnect(MqttReturnCode returnCode) const;

    void
    triggerSubscribe(int mid) const;

    void
    triggerUnsubscribe(int mid) const;

    void
    triggerPublish(int mid) const;

    void
    triggerMessage(int mid, std::string_view topic, void* payload, size_t size) const;

    void
    triggerLog(int logLevel, std::string_view message) const;

    OnConnectSignal
    onConnect() const override;

    OnDisconnectSignal
    onDisconnect() const override;

    OnSubscribeSignal
    onSubscribe() const override;

    OnUnsubscribeSignal
    onUnsubscribe() const override;

    OnPublishSignal
    onPublish() const override;

    OnMessageSignal
    onMessage() const override;

    OnLogSignal
    onLog() const override;

public:
    MOCK_METHOD(bool, hasConnection, (), (override, const));

    MOCK_METHOD(std::error_code,
                credentials,
                (std::string_view user, std::string_view password),
                (override, const));

    MOCK_METHOD(std::error_code,
                connect,
                (std::string_view host, int port, int keepAlive),
                (override, const));

    MOCK_METHOD(std::error_code,
                connectAsync,
                (std::string_view host, int port, int keepAlive),
                (override, const));

    MOCK_METHOD(std::error_code, reconnect, (), (override, const));

    MOCK_METHOD(std::error_code, reconnectAsync, (), (override, const));

    MOCK_METHOD(std::error_code, disconnect, (), (override, const));

    MOCK_METHOD((std::expected<int, std::error_code>),
                subscribe,
                (std::string_view topic, MqttQoS qos),
                (override, const));

    MOCK_METHOD((std::expected<int, std::error_code>),
                unsubscribe,
                (std::string_view topic),
                (override, const));

    MOCK_METHOD(
        (std::expected<int, std::error_code>),
        publish,
        (std::string_view topic, const void* payload, std::size_t size, MqttQoS qos, bool retain),
        (override, const));

    MOCK_METHOD((std::expected<int, std::error_code>),
                publish,
                (std::string_view topic, std::string_view payload, MqttQoS qos, bool retain),
                (override, const));

private:
    OnConnectSignal _onConnectSig;
    OnDisconnectSignal _onDisconnectSig;
    OnSubscribeSignal _onSubscribeSig;
    OnUnsubscribeSignal _onUnsubscribeSig;
    OnPublishSignal _onPublishSig;
    OnMessageSignal _onMessageSig;
    OnLogSignal _onLogSig;
};

} // namespace jar