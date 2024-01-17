#pragma once

#include "jarvisto/MqttClient.hpp"

#include <gmock/gmock.h>

namespace jar {

class MockMqttClient : public MqttClient {
public:
    MockMqttClient();

    void
    triggerConnect(MqttReturnCode returnCode);

    void
    triggerDisconnect(MqttReturnCode returnCode);

    void
    triggerSubscribe(int mid);

    void
    triggerUnsubscribe(int mid);

    void
    triggerPublish(int mid);

    void
    triggerMessage(int mid, std::string_view topic, void* payload, size_t size);

    void
    triggerLog(int logLevel, std::string_view message);

    sigc::connection
    onConnect(OnConnectSignal::slot_type&& slot) override;

    sigc::connection
    onDisconnect(OnDisconnectSignal::slot_type&& slot) override;

    sigc::connection
    onSubscribe(OnSubscribeSignal::slot_type&& slot) override;

    sigc::connection
    onUnsubscribe(OnUnsubscribeSignal::slot_type&& slot) override;

    sigc::connection
    onPublish(OnPublishSignal::slot_type&& slot) override;

    sigc::connection
    onMessage(OnMessageSignal::slot_type&& slot) override;

    sigc::connection
    onLog(OnLogSignal::slot_type&& slot) override;

public:
    MOCK_METHOD(bool, doHasConnection, (), (override));

    MOCK_METHOD(std::error_code,
                doCredentials,
                (std::string_view user, std::string_view password),
                (override));

    MOCK_METHOD(std::error_code,
                doConnect,
                (std::string_view host, uint16_t port, int keepAlive),
                (override));

    MOCK_METHOD(std::error_code,
                doConnectAsync,
                (std::string_view host, uint16_t port, int keepAlive),
                (override));

    MOCK_METHOD(std::error_code, doReconnect, (), (override));

    MOCK_METHOD(std::error_code, doReconnectAsync, (), (override));

    MOCK_METHOD(std::error_code, doDisconnect, (), (override));

    MOCK_METHOD((std::expected<int, std::error_code>),
                doSubscribe,
                (std::string_view topic, MqttQoS qos),
                (override));

    MOCK_METHOD((std::expected<int, std::error_code>),
                doUnsubscribe,
                (std::string_view topic),
                (override));

    MOCK_METHOD(
        (std::expected<int, std::error_code>),
        doPublish,
        (std::string_view topic, const void* payload, std::size_t size, MqttQoS qos, bool retain),
        (override));

    MOCK_METHOD((std::expected<int, std::error_code>),
                doPublish,
                (std::string_view topic, std::string_view payload, MqttQoS qos, bool retain),
                (override));

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