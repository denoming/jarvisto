#include "jarvisto/network/MqttClient.hpp"

#include <mosquitto.h>

#include <boost/assert.hpp>

#include <atomic>

namespace jar {

namespace {

class Initializer {
public:
    Initializer()
    {
        mosquitto_lib_init();
    }

    ~Initializer()
    {
        mosquitto_lib_cleanup();
    }
};

std::error_code
toError(const int returnCode)
{
    switch (returnCode) {
        using enum MqttErrorCode;
    case MOSQ_ERR_SUCCESS:
        return Success;
    case MOSQ_ERR_NO_CONN:
        return NoConnection;
    case MOSQ_ERR_NOMEM:
        return NoMemory;
    case MOSQ_ERR_MALFORMED_UTF8:
        return TopicInvalid;
    case MOSQ_ERR_OVERSIZE_PACKET:
        return PacketOversize;
    case MOSQ_ERR_PAYLOAD_SIZE:
        return PayloadOversize;
    case MOSQ_ERR_INVAL:
        return InvalidArguments;
    case MOSQ_ERR_PROTOCOL:
        return ProtocolError;
    case MOSQ_ERR_ERRNO:
        return SystemError;
    case MOSQ_ERR_QOS_NOT_SUPPORTED:
        return QosNotSupported;
    default:
        return Unknown;
    }
}

} // namespace

class MqttClient::Impl {
public:
    Impl(const char* id, const bool cleanSession, const bool logging)
    {
        static Initializer initializer;

        if (_handle = mosquitto_new(id, cleanSession, this); _handle == nullptr) {
            throw std::runtime_error{"Unable to initialise mosquitto client instance"};
        }

        mosquitto_connect_callback_set(_handle, &Impl::onConnect);
        mosquitto_disconnect_callback_set(_handle, &Impl::onDisconnect);
        mosquitto_subscribe_callback_set(_handle, &Impl::onSubscribe);
        mosquitto_unsubscribe_callback_set(_handle, &Impl::onUnsubscribe);
        mosquitto_publish_callback_set(_handle, &Impl::onPublish);
        mosquitto_message_callback_set(_handle, &Impl::onMessage);
        if (logging) {
            mosquitto_log_callback_set(_handle, &Impl::onLog);
        }

        if (mosquitto_loop_start(_handle) != MOSQ_ERR_SUCCESS) {
            mosquitto_destroy(_handle);
            _handle = nullptr;
            throw std::runtime_error{"Unable to start mosquitto loop"};
        }
    }

    ~Impl()
    {
        std::ignore = mosquitto_loop_stop(_handle, true);

        if (_handle != nullptr) {
            mosquitto_destroy(_handle);
        }
    }

    [[nodiscard]] OnConnectSignal
    onConnect()
    {
        return _onConnectSig;
    }

    [[nodiscard]] OnDisconnectSignal
    onDisconnect()
    {
        return _onDisconnectSig;
    }

    [[nodiscard]] OnSubscribeSignal
    onSubscribe()
    {
        return _onSubscribeSig;
    }

    [[nodiscard]] OnUnsubscribeSignal
    onUnsubscribe()
    {
        return _onUnsubscribeSig;
    }

    [[nodiscard]] OnPublishSignal
    onPublish()
    {
        return _onPublishSig;
    }

    [[nodiscard]] OnMessageSignal
    onMessage()
    {
        return _onMessageSig;
    }

    [[nodiscard]] OnLogSignal
    onLog()
    {
        return _onLogSig;
    }

    [[nodiscard]] bool
    hasConnection()
    {
        return _hasConnection;
    }

    [[nodiscard]] std::error_code
    credentials(const std::string_view user, const std::string_view password) const
    {
        return toError(mosquitto_username_pw_set(_handle, user.data(), password.data()));
    }

    [[nodiscard]] std::error_code
    connect(const std::string_view host, const int port, const int keepAlive) const
    {
        return toError(mosquitto_connect(_handle, host.data(), port, keepAlive));
    }

    [[nodiscard]] std::error_code
    connectAsync(const std::string_view host, const int port, const int keepAlive) const
    {
        return toError(mosquitto_connect_async(_handle, host.data(), port, keepAlive));
    }

    [[nodiscard]] std::error_code
    reconnect() const
    {
        return toError(mosquitto_reconnect(_handle));
    }

    [[nodiscard]] std::error_code
    reconnectAsync() const
    {
        return toError(mosquitto_reconnect_async(_handle));
    }

    [[nodiscard]] std::error_code
    disconnect() const
    {
        return toError(mosquitto_disconnect(_handle));
    }

    [[nodiscard]] std::expected<int, std::error_code>
    subscribe(const std::string_view topic, MqttQoS qos) const
    {
        int mid{};
        const int rc = mosquitto_subscribe(_handle, &mid, topic.data(), int(qos));
        if (rc != MOSQ_ERR_SUCCESS) {
            return std::unexpected(toError(rc));
        }
        return mid;
    }

    [[nodiscard]] std::expected<int, std::error_code>
    unsubscribe(const std::string_view topic) const
    {
        int mid{};
        const int rc = mosquitto_unsubscribe(_handle, &mid, topic.data());
        if (rc != MOSQ_ERR_SUCCESS) {
            return std::unexpected(toError(rc));
        }
        return mid;
    }

    [[nodiscard]] std::expected<int, std::error_code>
    publish(const std::string_view topic,
            const void* payload,
            const std::size_t size,
            const MqttQoS qos,
            const bool retain) const
    {
        int mid{};
        const int rc = mosquitto_publish(_handle,
                                         &mid,
                                         topic.data(),
                                         static_cast<int>(size),
                                         payload,
                                         static_cast<int>(qos),
                                         retain);
        if (rc != MOSQ_ERR_SUCCESS) {
            return std::unexpected(toError(rc));
        }
        return mid;
    }

    [[nodiscard]] std::expected<int, std::error_code>
    publish(std::string_view const topic,
            std::string_view const payload,
            const MqttQoS qos,
            const bool retain) const
    {
        return publish(topic, payload.data(), payload.size(), qos, retain);
    }

private:
    void
    handleConnect(const MqttReturnCode rc)
    {
        if (rc == MqttReturnCode::Accepted) {
            _hasConnection = true;
        }
        _onConnectSig(rc);
    }

    void
    handleDisconnect(const MqttReturnCode rc)
    {
        _hasConnection = false;
        _onDisconnectSig(rc);
    }

    void
    onSubscribe(const int mid, const int /*size*/, const int* /*qos*/) const
    {
        _onSubscribeSig(mid);
    }

    void
    onUnsubscribe(const int mid) const
    {
        _onUnsubscribeSig(mid);
    }

    void
    onPublish(const int mid) const
    {
        _onPublishSig(mid);
    }

    void
    onMessage(const int mid, const std::string_view topic, void* payload, const size_t size) const
    {
        _onMessageSig(mid, topic, payload, size);
    }

    void
    onLog(const int logLevel, const std::string_view message) const
    {
        _onLogSig(logLevel, message);
    }

private:
    static void
    onConnect(mosquitto* /*handle*/, void* data, int rc)
    {
        if (auto* self = static_cast<Impl*>(data); self != nullptr) {
            self->handleConnect(static_cast<MqttReturnCode>(rc));
        }
    }

    static void
    onDisconnect(mosquitto* /*handle*/, void* data, int rc)
    {
        if (auto* self = static_cast<Impl*>(data); self != nullptr) {
            self->handleDisconnect(static_cast<MqttReturnCode>(rc));
        }
    }

    static void
    onSubscribe(mosquitto* /*handle*/, void* data, const int mid, const int size, const int* qos)
    {
        if (const auto* self = static_cast<Impl*>(data); self != nullptr) {
            self->onSubscribe(mid, size, qos);
        }
    }

    static void
    onUnsubscribe(mosquitto* /*handle*/, void* data, const int mid)
    {
        if (const auto* self = static_cast<Impl*>(data); self != nullptr) {
            self->onUnsubscribe(mid);
        }
    }

    static void
    onPublish(mosquitto* /*handle*/, void* data, const int mid)
    {
        if (const auto* self = static_cast<Impl*>(data); self != nullptr) {
            self->onPublish(mid);
        }
    }

    static void
    onMessage(mosquitto* /*handle*/, void* data, const mosquitto_message* msg)
    {
        if (const auto* self = static_cast<Impl*>(data); self != nullptr) {
            self->onMessage(msg->mid, msg->topic, msg->payload, msg->payloadlen);
        }
    }

    static void
    onLog(mosquitto* /*handle*/, void* data, const int logLevel, const char* message)
    {
        if (const auto* self = static_cast<Impl*>(data); self != nullptr) {
            self->onLog(logLevel, message);
        }
    }

private:
    std::atomic<bool> _hasConnection{false};
    OnConnectSignal _onConnectSig;
    OnDisconnectSignal _onDisconnectSig;
    OnSubscribeSignal _onSubscribeSig;
    OnUnsubscribeSignal _onUnsubscribeSig;
    OnPublishSignal _onPublishSig;
    OnMessageSignal _onMessageSig;
    OnLogSignal _onLogSig;
    mosquitto* _handle{};
};

MqttClient::MqttClient(const char* id, bool cleanSession, bool logging)
    : _impl{std::make_unique<Impl>(id, cleanSession, logging)}
{
}

MqttClient::~MqttClient() = default;

bool
MqttClient::hasConnection() const
{
    BOOST_ASSERT(_impl);
    return _impl->hasConnection();
}

std::error_code
MqttClient::credentials(const std::string_view user, const std::string_view password) const
{
    BOOST_ASSERT(_impl);
    return _impl->credentials(user, password);
}

std::error_code
MqttClient::connect(const std::string_view host, const int port, const int keepAlive) const
{
    BOOST_ASSERT(_impl);
    return _impl->connect(host, port, keepAlive);
}

std::error_code
MqttClient::connectAsync(const std::string_view host, const int port, const int keepAlive) const
{
    BOOST_ASSERT(_impl);
    return _impl->connectAsync(host, port, keepAlive);
}

std::error_code
MqttClient::reconnect() const
{
    BOOST_ASSERT(_impl);
    return _impl->reconnect();
}

std::error_code
MqttClient::reconnectAsync() const
{
    BOOST_ASSERT(_impl);
    return _impl->reconnectAsync();
}

std::error_code
MqttClient::disconnect() const
{
    BOOST_ASSERT(_impl);
    return _impl->disconnect();
}

std::expected<int, std::error_code>
MqttClient::subscribe(const std::string_view topic, const MqttQoS qos) const
{
    BOOST_ASSERT(_impl);
    return _impl->subscribe(topic, qos);
}

std::expected<int, std::error_code>
MqttClient::unsubscribe(const std::string_view topic) const
{
    BOOST_ASSERT(_impl);
    return _impl->unsubscribe(topic);
}

std::expected<int, std::error_code>
MqttClient::publish(const std::string_view topic,
                    const void* payload,
                    const std::size_t size,
                    const MqttQoS qos,
                    const bool retain) const
{
    BOOST_ASSERT(_impl);
    return _impl->publish(topic, payload, size, qos, retain);
}

std::expected<int, std::error_code>
MqttClient::publish(const std::string_view topic,
                    const std::string_view payload,
                    const MqttQoS qos,
                    const bool retain) const
{
    BOOST_ASSERT(_impl);
    return _impl->publish(topic, payload, qos, retain);
}

MqttClient::OnConnectSignal
MqttClient::onConnect() const
{
    BOOST_ASSERT(_impl);
    return _impl->onConnect();
}

MqttClient::OnDisconnectSignal
MqttClient::onDisconnect() const
{
    BOOST_ASSERT(_impl);
    return _impl->onDisconnect();
}

MqttClient::OnSubscribeSignal
MqttClient::onSubscribe() const
{
    BOOST_ASSERT(_impl);
    return _impl->onSubscribe();
}

MqttClient::OnUnsubscribeSignal
MqttClient::onUnsubscribe() const
{
    BOOST_ASSERT(_impl);
    return _impl->onUnsubscribe();
}

MqttClient::OnPublishSignal
MqttClient::onPublish() const
{
    BOOST_ASSERT(_impl);
    return _impl->onPublish();
}

MqttClient::OnMessageSignal
MqttClient::onMessage() const
{
    BOOST_ASSERT(_impl);
    return _impl->onMessage();
}

MqttClient::OnLogSignal
MqttClient::onLog() const
{
    BOOST_ASSERT(_impl);
    return _impl->onLog();
}

} // namespace jar