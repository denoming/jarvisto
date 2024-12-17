#include "network/MockMqttClient.hpp"

using namespace testing;

namespace jar {

MockMqttClient::MockMqttClient()
{
    ON_CALL(*this, doHasConnection).WillByDefault(Return(false));

    ON_CALL(*this, doCredentials)
        .WillByDefault(Return(std::make_error_code(std::errc::not_supported)));

    ON_CALL(*this, doConnect).WillByDefault(Return(std::make_error_code(std::errc::not_supported)));

    ON_CALL(*this, doConnectAsync)
        .WillByDefault(Return(std::make_error_code(std::errc::not_supported)));

    ON_CALL(*this, doReconnect)
        .WillByDefault(Return(std::make_error_code(std::errc::not_supported)));

    ON_CALL(*this, doReconnectAsync)
        .WillByDefault(Return(std::make_error_code(std::errc::not_supported)));

    ON_CALL(*this, doDisconnect)
        .WillByDefault(Return(std::make_error_code(std::errc::not_supported)));

    ON_CALL(*this, doSubscribe)
        .WillByDefault(Return(std::unexpected(std::make_error_code(std::errc::not_supported))));

    ON_CALL(*this, doUnsubscribe)
        .WillByDefault(Return(std::unexpected(std::make_error_code(std::errc::not_supported))));

    ON_CALL(*this, doPublish(_, _, _, _, _))
        .WillByDefault(Return(std::unexpected(std::make_error_code(std::errc::not_supported))));

    ON_CALL(*this, doPublish(_, _, _, _))
        .WillByDefault(Return(std::unexpected(std::make_error_code(std::errc::not_supported))));
}

void
MockMqttClient::triggerConnect(MqttReturnCode returnCode)
{
    _onConnectSig(returnCode);
}

void
MockMqttClient::triggerDisconnect(MqttReturnCode returnCode)
{
    _onDisconnectSig(returnCode);
}

void
MockMqttClient::triggerSubscribe(int mid)
{
    _onSubscribeSig(mid);
}

void
MockMqttClient::triggerUnsubscribe(int mid)
{
    _onUnsubscribeSig(mid);
}

void
MockMqttClient::triggerPublish(int mid)
{
    _onPublishSig(mid);
}

void
MockMqttClient::triggerMessage(int mid, std::string_view topic, void* payload, size_t size)
{
    _onMessageSig(mid, topic, payload, size);
}

void
MockMqttClient::triggerLog(int logLevel, std::string_view message)
{
    _onLogSig(logLevel, message);
}

sigc::connection
MockMqttClient::onConnect(OnConnectSignal::slot_type&& slot)
{
    return _onConnectSig.connect(std::move(slot));
}

sigc::connection
MockMqttClient::onDisconnect(OnDisconnectSignal::slot_type&& slot)
{
    return _onDisconnectSig.connect(std::move(slot));
}

sigc::connection
MockMqttClient::onSubscribe(OnSubscribeSignal::slot_type&& slot)
{
    return _onSubscribeSig.connect(std::move(slot));
}

sigc::connection
MockMqttClient::onUnsubscribe(OnUnsubscribeSignal::slot_type&& slot)
{
    return _onUnsubscribeSig.connect(std::move(slot));
}

sigc::connection
MockMqttClient::onPublish(OnPublishSignal::slot_type&& slot)
{
    return _onPublishSig.connect(std::move(slot));
}

sigc::connection
MockMqttClient::onMessage(OnMessageSignal::slot_type&& slot)
{
    return _onMessageSig.connect(std::move(slot));
}

sigc::connection
MockMqttClient::onLog(OnLogSignal::slot_type&& slot)
{
    return _onLogSig.connect(std::move(slot));
}

} // namespace jar