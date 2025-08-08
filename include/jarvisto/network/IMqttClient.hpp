#pragma once

#include "jarvisto/network/Mqtt.hpp"

#include <sigc++/signal.h>

#include <expected>
#include <string_view>

namespace jar {

class IMqttClient {
public:
    static inline int kDefaultPort{1883};
    static inline int kDefaultKeepAlive{60};
    static inline MqttQoS kDefaultQos{MqttQoS::Level2};
    static inline bool kDefaultRetain{false};

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

    virtual ~IMqttClient() = default;

    [[nodiscard]] virtual bool
    hasConnection() const
        = 0;

    [[nodiscard]] virtual std::error_code
    credentials(std::string_view user, std::string_view password) const
        = 0;

    [[nodiscard]] virtual std::error_code
    connect(std::string_view host, int port, int keepAlive) const
        = 0;

    [[nodiscard]] virtual std::error_code
    connectAsync(std::string_view host, int port, int keepAlive) const
        = 0;

    [[nodiscard]] virtual std::error_code
    reconnect() const
        = 0;

    [[nodiscard]] virtual std::error_code
    reconnectAsync() const
        = 0;

    [[nodiscard]] virtual std::error_code
    disconnect() const
        = 0;

    [[nodiscard]] virtual std::expected<int, std::error_code>
    subscribe(std::string_view topic, MqttQoS qos) const = 0;

    [[nodiscard]] virtual std::expected<int, std::error_code>
    unsubscribe(std::string_view topic) const = 0;

    [[nodiscard]] virtual std::expected<int, std::error_code>
    publish(std::string_view topic, const void* payload, std::size_t size, MqttQoS qos, bool retain)
        const
        = 0;

    [[nodiscard]] virtual std::expected<int, std::error_code>
    publish(std::string_view topic, std::string_view payload, MqttQoS qos, bool retain) const = 0;

    [[nodiscard]] virtual OnConnectSignal
    onConnect() const
        = 0;

    [[nodiscard]] virtual OnDisconnectSignal
    onDisconnect() const
        = 0;

    [[nodiscard]] virtual OnSubscribeSignal
    onSubscribe() const
        = 0;

    [[nodiscard]] virtual OnUnsubscribeSignal
    onUnsubscribe() const
        = 0;

    [[nodiscard]] virtual OnPublishSignal
    onPublish() const
        = 0;

    [[nodiscard]] virtual OnMessageSignal
    onMessage() const
        = 0;

    [[nodiscard]] virtual OnLogSignal
    onLog() const
        = 0;
};

} // namespace jar