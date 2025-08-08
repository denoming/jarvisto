#include "network/MockMqttClient.hpp"

using namespace testing;

namespace jar {

MockMqttClient::MockMqttClient()
{
    ON_CALL(*this, hasConnection).WillByDefault(Return(false));

    ON_CALL(*this, credentials)
        .WillByDefault(Return(std::make_error_code(std::errc::not_supported)));

    ON_CALL(*this, connect).WillByDefault(Return(std::make_error_code(std::errc::not_supported)));

    ON_CALL(*this, connectAsync)
        .WillByDefault(Return(std::make_error_code(std::errc::not_supported)));

    ON_CALL(*this, reconnect).WillByDefault(Return(std::make_error_code(std::errc::not_supported)));

    ON_CALL(*this, reconnectAsync)
        .WillByDefault(Return(std::make_error_code(std::errc::not_supported)));

    ON_CALL(*this, disconnect)
        .WillByDefault(Return(std::make_error_code(std::errc::not_supported)));

    ON_CALL(*this, subscribe)
        .WillByDefault(Return(std::unexpected(std::make_error_code(std::errc::not_supported))));

    ON_CALL(*this, unsubscribe)
        .WillByDefault(Return(std::unexpected(std::make_error_code(std::errc::not_supported))));

    ON_CALL(*this, publish(_, _, _, _, _))
        .WillByDefault(Return(std::unexpected(std::make_error_code(std::errc::not_supported))));

    ON_CALL(*this, publish(_, _, _, _))
        .WillByDefault(Return(std::unexpected(std::make_error_code(std::errc::not_supported))));
}

void
MockMqttClient::triggerConnect(MqttReturnCode returnCode) const
{
    _onConnectSig(returnCode);
}

void
MockMqttClient::triggerDisconnect(MqttReturnCode returnCode) const
{
    _onDisconnectSig(returnCode);
}

void
MockMqttClient::triggerSubscribe(int mid) const
{
    _onSubscribeSig(mid);
}

void
MockMqttClient::triggerUnsubscribe(int mid) const
{
    _onUnsubscribeSig(mid);
}

void
MockMqttClient::triggerPublish(int mid) const
{
    _onPublishSig(mid);
}

void
MockMqttClient::triggerMessage(int mid, std::string_view topic, void* payload, size_t size) const
{
    _onMessageSig(mid, topic, payload, size);
}

void
MockMqttClient::triggerLog(int logLevel, std::string_view message) const
{
    _onLogSig(logLevel, message);
}

MockMqttClient::OnConnectSignal
MockMqttClient::onConnect() const
{
    return _onConnectSig;
}

MockMqttClient::OnDisconnectSignal
MockMqttClient::onDisconnect() const
{
    return _onDisconnectSig;
}

MockMqttClient::OnSubscribeSignal
MockMqttClient::onSubscribe() const
{
    return _onSubscribeSig;
}

MockMqttClient::OnUnsubscribeSignal
MockMqttClient::onUnsubscribe() const
{
    return _onUnsubscribeSig;
}

MockMqttClient::OnPublishSignal
MockMqttClient::onPublish() const
{
    return _onPublishSig;
}

MockMqttClient::OnMessageSignal
MockMqttClient::onMessage() const
{
    return _onMessageSig;
}

MockMqttClient::OnLogSignal
MockMqttClient::onLog() const
{
    return _onLogSig;
}

} // namespace jar