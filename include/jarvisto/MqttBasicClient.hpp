#pragma once

#include "jarvisto/MqttClient.hpp"
#include "jarvisto/Export.hpp"

#include <sigc++/signal.h>

#include <memory>

namespace jar {

class JARVISTO_EXPORT MqttBasicClient final : public MqttClient {
public:
    explicit MqttBasicClient(const char* id = nullptr,
                             bool cleanSession = true,
                             bool getLogs = false);

    ~MqttBasicClient() final;

    [[maybe_unused]] sigc::connection
    onConnect(OnConnectSignal::slot_type&& slot) final;

    [[maybe_unused]] sigc::connection
    onDisconnect(OnDisconnectSignal::slot_type&& slot) final;

    [[maybe_unused]] sigc::connection
    onSubscribe(OnSubscribeSignal::slot_type&& slot) final;

    [[maybe_unused]] sigc::connection
    onUnsubscribe(OnUnsubscribeSignal::slot_type&& slot) final;

    [[maybe_unused]] sigc::connection
    onPublish(OnPublishSignal::slot_type&& slot) final;

    [[maybe_unused]] sigc::connection
    onMessage(OnMessageSignal::slot_type&& slot) final;

    [[maybe_unused]] sigc::connection
    onLog(OnLogSignal::slot_type&& slot) final;

private:
    bool
    doHasConnection() final;

    std::error_code
    doCredentials(std::string_view user, std::string_view password) final;

    std::error_code
    doConnect(std::string_view host, uint16_t port, int keepAlive) final;

    std::error_code
    doConnectAsync(std::string_view host, uint16_t port, int keepAlive) final;

    std::error_code
    doReconnect() final;

    std::error_code
    doReconnectAsync() final;

    std::error_code
    doDisconnect() final;

    std::expected<int, std::error_code>
    doSubscribe(std::string_view topic, MqttQoS qos) final;

    std::expected<int, std::error_code>
    doUnsubscribe(std::string_view topic) final;

    std::expected<int, std::error_code>
    doPublish(std::string_view topic,
              const void* payload,
              std::size_t size,
              MqttQoS qos,
              bool retain) final;

    std::expected<int, std::error_code>
    doPublish(std::string_view topic, std::string_view payload, MqttQoS qos, bool retain) final;

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace jar