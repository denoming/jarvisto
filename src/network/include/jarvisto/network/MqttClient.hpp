// Copyright 2025 Denys Asauliak
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "jarvisto/network/Mqtt.hpp"
#include "jarvisto/network/Export.hpp"

#include <sigc++/signal.h>

#include <memory>
#include <atomic>
#include <expected>
#include <string_view>

namespace jar {

class JARVISTO_NETWORK_EXPORT MqttClient {
public:
    static inline int kDefaultPort{1883};
    static inline int kDefaultKeepAlive{60};

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

    explicit MqttClient(const char* id = nullptr, bool cleanSession = true, bool logging = false);

    virtual ~MqttClient();

    [[nodiscard]] bool
    hasConnection() const;

    [[nodiscard]] std::error_code
    credentials(std::string_view user, std::string_view password) const;

    [[nodiscard]] std::error_code
    connect(std::string_view host,
            int port = kDefaultPort,
            int keepAlive = kDefaultKeepAlive) const;

    [[nodiscard]] std::error_code
    connectAsync(std::string_view host,
                 int port = kDefaultPort,
                 int keepAlive = kDefaultKeepAlive) const;

    [[nodiscard]] std::error_code
    reconnect() const;

    [[nodiscard]] std::error_code
    reconnectAsync() const;

    [[nodiscard]] std::error_code
    disconnect() const;

    [[nodiscard]] std::expected<int, std::error_code>
    subscribe(std::string_view topic, MqttQoS qos = MqttQoS::Level2) const;

    [[nodiscard]] std::expected<int, std::error_code>
    unsubscribe(std::string_view topic) const;

    [[nodiscard]] std::expected<int, std::error_code>
    publish(std::string_view topic,
            const void* payload,
            std::size_t size,
            MqttQoS qos = MqttQoS::Level2,
            bool retain = false) const;

    [[nodiscard]] std::expected<int, std::error_code>
    publish(std::string_view topic,
            std::string_view payload,
            MqttQoS qos = MqttQoS::Level2,
            bool retain = false) const;

    [[nodiscard]] OnConnectSignal
    onConnect() const;

    [[nodiscard]] OnDisconnectSignal
    onDisconnect() const;

    [[nodiscard]] OnSubscribeSignal
    onSubscribe() const;

    [[nodiscard]] OnUnsubscribeSignal
    onUnsubscribe() const;

    [[nodiscard]] OnPublishSignal
    onPublish() const;

    [[nodiscard]] OnMessageSignal
    onMessage() const;

    [[nodiscard]] OnLogSignal
    onLog() const;

protected:
    [[nodiscard]] virtual bool
    doHasConnection() const;

    [[nodiscard]] virtual std::error_code
    doCredentials(std::string_view user, std::string_view password) const;

    [[nodiscard]] virtual std::error_code
    doConnect(std::string_view host, int port, int keepAlive) const;

    [[nodiscard]] virtual std::error_code
    doConnectAsync(std::string_view host, int port, int keepAlive) const;

    [[nodiscard]] virtual std::error_code
    doReconnect() const;

    [[nodiscard]] virtual std::error_code
    doReconnectAsync() const;

    [[nodiscard]] virtual std::error_code
    doDisconnect() const;

    [[nodiscard]] virtual std::expected<int, std::error_code>
    doSubscribe(std::string_view topic, MqttQoS qos) const;

    [[nodiscard]] virtual std::expected<int, std::error_code>
    doUnsubscribe(std::string_view topic) const;

    [[nodiscard]] virtual std::expected<int, std::error_code>
    doPublish(std::string_view topic,
              const void* payload,
              std::size_t size,
              MqttQoS qos,
              bool retain) const;

    [[nodiscard]] virtual std::expected<int, std::error_code>
    doPublish(std::string_view topic, std::string_view payload, MqttQoS qos, bool retain) const;

    void
    notifyConnect(MqttReturnCode rc) const;

    void
    notifyDisconnect(MqttReturnCode rc) const;

    void
    notifySubscribe(int mid) const;

    void
    notifyUnsubscribe(int mid) const;

    void
    notifyPublish(int mid) const;

    void
    notifyMessage(int mid, std::string_view topic, void* payload, size_t size) const;

    void
    notifyLog(int logLevel, std::string_view message) const;

private:
    void
    handleConnect(MqttReturnCode rc);

    void
    handleDisconnect(MqttReturnCode rc);

private:
    void* _handle;

    OnConnectSignal _onConnectSig;
    OnDisconnectSignal _onDisconnectSig;
    OnSubscribeSignal _onSubscribeSig;
    OnUnsubscribeSignal _onUnsubscribeSig;
    OnPublishSignal _onPublishSig;
    OnMessageSignal _onMessageSig;
    OnLogSignal _onLogSig;

    std::atomic<bool> _hasConnection{false};
};

} // namespace jar