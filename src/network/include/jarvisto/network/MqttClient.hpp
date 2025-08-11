#pragma once

#include "jarvisto/network/IMqttClient.hpp"
#include "jarvisto/network/Export.hpp"

#include <memory>

namespace jar {

class JARVISTO_NETWORK_EXPORT MqttClient final : public IMqttClient {
public:
    explicit MqttClient(const char* id = nullptr, bool cleanSession = true, bool logging = false);

    ~MqttClient() override;

    [[nodiscard]] bool
    hasConnection() const override;

    [[nodiscard]] std::error_code
    credentials(std::string_view user, std::string_view password) const override;

    [[nodiscard]] std::error_code
    connect(std::string_view host, int port, int keepAlive) const override;

    [[nodiscard]] std::error_code
    connectAsync(std::string_view host, int port, int keepAlive) const override;

    [[nodiscard]] std::error_code
    reconnect() const override;

    [[nodiscard]] std::error_code
    reconnectAsync() const override;

    [[nodiscard]] std::error_code
    disconnect() const override;

    [[nodiscard]] std::expected<int, std::error_code>
    subscribe(std::string_view topic, MqttQoS qos) const override;

    [[nodiscard]] std::expected<int, std::error_code>
    unsubscribe(std::string_view topic) const override;

    [[nodiscard]] std::expected<int, std::error_code>
    publish(std::string_view topic, const void* payload, std::size_t size, MqttQoS qos, bool retain)
        const override;

    [[nodiscard]] std::expected<int, std::error_code>
    publish(std::string_view topic,
            std::string_view payload,
            MqttQoS qos,
            bool retain) const override;

    [[nodiscard]] OnConnectSignal
    onConnect() const override;

    [[nodiscard]] OnDisconnectSignal
    onDisconnect() const override;

    [[nodiscard]] OnSubscribeSignal
    onSubscribe() const override;

    [[nodiscard]] OnUnsubscribeSignal
    onUnsubscribe() const override;

    [[nodiscard]] OnPublishSignal
    onPublish() const override;

    [[nodiscard]] OnMessageSignal
    onMessage() const override;

    [[nodiscard]] OnLogSignal
    onLog() const override;

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar