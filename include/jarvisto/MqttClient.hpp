#pragma once

#include "jarvisto/Mqtt.hpp"
#include "jarvisto/Export.hpp"

#include <sigc++/signal.h>

#include <expected>
#include <string_view>

namespace jar {

class JARVISTO_EXPORT MqttClient {
public:
    /* Signatures */
    using OnConnect = void(MqttReturnCode returnCode);
    using OnDisconnect = void(MqttReturnCode returnCode);
    using OnSubscribe = void(int mid);
    using OnUnsubscribe = void(int mid);
    using OnPublish = void(int mid);
    using OnMessage = void(int mid, std::string_view topic, void* payload, size_t size);
    using OnLog = void(int logLevel, std::string_view message);

    /* Signals */
    using OnConnectSignal = sigc::signal<OnConnect>;
    using OnDisconnectSignal = sigc::signal<OnDisconnect>;
    using OnSubscribeSignal = sigc::signal<OnSubscribe>;
    using OnUnsubscribeSignal = sigc::signal<OnUnsubscribe>;
    using OnPublishSignal = sigc::signal<OnPublish>;
    using OnMessageSignal = sigc::signal<OnMessage>;
    using OnLogSignal = sigc::signal<OnLog>;

    virtual ~MqttClient() = default;

    [[nodiscard]] bool
    hasConnection();

    [[nodiscard]] std::error_code
    credentials(std::string_view user, std::string_view password);

    [[nodiscard]] std::error_code
    connect(std::string_view host, uint16_t port = 1883, int keepAlive = 60);

    [[nodiscard]] std::error_code
    connectAsync(std::string_view host, uint16_t port = 1883, int keepAlive = 60);

    [[nodiscard]] std::error_code
    reconnect();

    [[nodiscard]] std::error_code
    reconnectAsync();

    [[nodiscard]] std::error_code
    disconnect();

    [[nodiscard]] std::expected<int, std::error_code>
    subscribe(std::string_view topic, MqttQoS qos = MqttQoS::Level2);

    [[nodiscard]] std::expected<int, std::error_code>
    unsubscribe(std::string_view topic);

    [[nodiscard]] std::expected<int, std::error_code>
    publish(std::string_view topic,
            const void* payload,
            std::size_t size,
            MqttQoS qos = MqttQoS::Level2,
            bool retain = false);

    [[nodiscard]] std::expected<int, std::error_code>
    publish(std::string_view topic,
            std::string_view payload,
            MqttQoS qos = MqttQoS::Level2,
            bool retain = false);

    [[maybe_unused]] virtual sigc::connection
    onConnect(OnConnectSignal::slot_type&& slot)
        = 0;

    [[maybe_unused]] virtual sigc::connection
    onDisconnect(OnDisconnectSignal::slot_type&& slot)
        = 0;

    [[maybe_unused]] virtual sigc::connection
    onSubscribe(OnSubscribeSignal::slot_type&& slot)
        = 0;

    [[maybe_unused]] virtual sigc::connection
    onUnsubscribe(OnUnsubscribeSignal::slot_type&& slot)
        = 0;

    [[maybe_unused]] virtual sigc::connection
    onPublish(OnPublishSignal::slot_type&& slot)
        = 0;

    [[maybe_unused]] virtual sigc::connection
    onMessage(OnMessageSignal::slot_type&& slot)
        = 0;

    [[maybe_unused]] virtual sigc::connection
    onLog(OnLogSignal::slot_type&& slot)
        = 0;

protected:
    virtual bool
    doHasConnection()
        = 0;

    virtual std::error_code
    doCredentials(std::string_view user, std::string_view password)
        = 0;

    virtual std::error_code
    doConnect(std::string_view host, uint16_t port, int keepAlive)
        = 0;

    virtual std::error_code
    doConnectAsync(std::string_view host, uint16_t port, int keepAlive)
        = 0;

    virtual std::error_code
    doReconnect()
        = 0;

    virtual std::error_code
    doReconnectAsync()
        = 0;

    virtual std::error_code
    doDisconnect()
        = 0;

    virtual std::expected<int, std::error_code>
    doSubscribe(std::string_view topic, MqttQoS qos) = 0;

    virtual std::expected<int, std::error_code>
    doUnsubscribe(std::string_view topic) = 0;

    virtual std::expected<int, std::error_code>
    doPublish(
        std::string_view topic, const void* payload, std::size_t size, MqttQoS qos, bool retain)
        = 0;

    virtual std::expected<int, std::error_code>
    doPublish(std::string_view topic, std::string_view payload, MqttQoS qos, bool retain) = 0;
};

} // namespace jar
