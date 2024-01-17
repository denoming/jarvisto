#include "jarvisto/MqttClient.hpp"

namespace jar {

bool
MqttClient::hasConnection()
{
    return doHasConnection();
}

std::error_code
MqttClient::credentials(std::string_view user, std::string_view password)
{
    return doCredentials(user, password);
}

std::error_code
MqttClient::connect(std::string_view host, uint16_t port, int keepAlive)
{
    return doConnect(host, port, keepAlive);
}

std::error_code
MqttClient::connectAsync(std::string_view host, uint16_t port, int keepAlive)
{
    return doConnectAsync(host, port, keepAlive);
}

std::error_code
MqttClient::reconnect()
{
    return doReconnect();
}

std::error_code
MqttClient::reconnectAsync()
{
    return doReconnectAsync();
}

std::error_code
MqttClient::disconnect()
{
    return doDisconnect();
}

std::expected<int, std::error_code>
MqttClient::subscribe(std::string_view topic, MqttQoS qos)
{
    return doSubscribe(topic, qos);
}

std::expected<int, std::error_code>
MqttClient::unsubscribe(std::string_view topic)
{
    return doUnsubscribe(topic);
}

std::expected<int, std::error_code>
MqttClient::publish(
    std::string_view topic, const void* payload, std::size_t size, MqttQoS qos, bool retain)
{
    return doPublish(topic, payload, size, qos, retain);
}

std::expected<int, std::error_code>
MqttClient::publish(std::string_view topic, std::string_view payload, MqttQoS qos, bool retain)
{
    return doPublish(topic, payload, qos, retain);
}

} // namespace jar