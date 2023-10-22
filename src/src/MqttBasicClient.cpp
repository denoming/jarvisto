#include "jarvisto/MqttBasicClient.hpp"

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
toError(int returnCode)
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

class MqttBasicClient::Impl {
public:
    Impl(const char* id, bool cleanSession, bool getLogs)
    {
        static Initializer initializer;

        _handle = mosquitto_new(id, cleanSession, this);
        if (_handle == nullptr) {
            throw std::runtime_error{"Unable to initialise mosquitto client instance"};
        }

        mosquitto_connect_callback_set(_handle, &onMosquittoConnect);
        mosquitto_disconnect_callback_set(_handle, &onMosquittoDisconnect);
        mosquitto_subscribe_callback_set(_handle, &onMosquittoSubscribe);
        mosquitto_unsubscribe_callback_set(_handle, &onMosquittoUnsubscribe);
        mosquitto_publish_callback_set(_handle, &onMosquittoPublish);
        mosquitto_publish_callback_set(_handle, &onMosquittoPublish);
        mosquitto_message_callback_set(_handle, &onMosquittoMessage);
        if (getLogs) {
            mosquitto_log_callback_set(_handle, &onMosquittoLog);
        }

        if (mosquitto_loop_start(_handle) != MOSQ_ERR_SUCCESS) {
            mosquitto_destroy(_handle);
            _handle = nullptr;
            throw std::runtime_error{"Unable to start mosquitto loop"};
        }
    }

    virtual ~Impl()
    {
        std::ignore = mosquitto_loop_stop(_handle, true);

        if (_handle != nullptr) {
            mosquitto_destroy(_handle);
        }
    }

    [[maybe_unused]] sigc::connection
    attachConnectSlot(OnConnectSignal::slot_type&& slot)
    {
        return _onConnectSig.connect(std::move(slot));
    }

    [[maybe_unused]] sigc::connection
    attachDisconnectSlot(OnDisconnectSignal::slot_type&& slot)
    {
        return _onDisconnectSig.connect(std::move(slot));
    }

    [[maybe_unused]] sigc::connection
    attachSubscribeSlot(OnSubscribeSignal::slot_type&& slot)
    {
        return _onSubscribeSig.connect(std::move(slot));
    }

    [[maybe_unused]] sigc::connection
    attachUnsubscribeSlot(OnUnsubscribeSignal::slot_type&& slot)
    {
        return _onUnsubscribeSig.connect(std::move(slot));
    }

    [[maybe_unused]] sigc::connection
    attachPublishSlot(OnPublishSignal::slot_type&& slot)
    {
        return _onPublishSig.connect(std::move(slot));
    }

    [[maybe_unused]] sigc::connection
    attachMessageSlot(OnMessageSignal::slot_type&& slot)
    {
        return _onMessageSig.connect(std::move(slot));
    }

    [[maybe_unused]] sigc::connection
    attachLogSlot(OnLogSignal::slot_type&& slot)
    {
        return _onLogSig.connect(std::move(slot));
    }

    [[nodiscard]] bool
    hasConnection()
    {
        return _hasConnection;
    }

    [[nodiscard]] std::error_code
    credentials(std::string_view user, std::string_view password)
    {
        return toError(mosquitto_username_pw_set(_handle, user.data(), password.data()));
    }

    [[nodiscard]] std::error_code
    connect(std::string_view host, uint16_t port, int keepAlive)
    {
        return toError(mosquitto_connect(_handle, host.data(), port, keepAlive));
    }

    [[nodiscard]] std::error_code
    reconnect()
    {
        return toError(mosquitto_reconnect(_handle));
    }

    [[nodiscard]] std::error_code
    reconnectAsync()
    {
        return toError(mosquitto_reconnect_async(_handle));
    }

    [[nodiscard]] std::error_code
    connectAsync(std::string_view host, uint16_t port, int keepAlive)
    {
        return toError(mosquitto_connect_async(_handle, host.data(), port, keepAlive));
    }

    [[nodiscard]] std::error_code
    disconnect()
    {
        return toError(mosquitto_disconnect(_handle));
    }

    [[nodiscard]] std::expected<int, std::error_code>
    subscribe(std::string_view topic, MqttQoS qos)
    {
        int mid{};
        const int rc = mosquitto_subscribe(_handle, &mid, topic.data(), int(qos));
        if (rc != MOSQ_ERR_SUCCESS) {
            return std::unexpected(toError(rc));
        }
        return mid;
    }

    [[nodiscard]] std::expected<int, std::error_code>
    unsubscribe(std::string_view topic)
    {
        int mid{};
        const int rc = mosquitto_unsubscribe(_handle, &mid, topic.data());
        if (rc != MOSQ_ERR_SUCCESS) {
            return std::unexpected(toError(rc));
        }
        return mid;
    }

    [[nodiscard]] std::expected<int, std::error_code>
    publish(std::string_view topic, const void* payload, std::size_t size, MqttQoS qos, bool retain)
    {
        int mid{};
        const int rc
            = mosquitto_publish(_handle, &mid, topic.data(), int(size), payload, int(qos), retain);
        if (rc != MOSQ_ERR_SUCCESS) {
            return std::unexpected(toError(rc));
        }
        return mid;
    }

    [[nodiscard]] std::expected<int, std::error_code>
    publish(std::string_view topic, std::string_view payload, MqttQoS qos, bool retain)
    {
        return publish(topic, payload.data(), payload.size(), qos, retain);
    }

private:
    void
    onConnect(MqttReturnCode rc)
    {
        if (rc == MqttReturnCode::Accepted) {
            _hasConnection = true;
        }

        _onConnectSig(rc);
    }

    void
    onDisconnect(MqttReturnCode rc)
    {
        _hasConnection = false;

        _onDisconnectSig(rc);
    }

    void
    onSubscribe(int mid, int /*size*/, const int* /*qos*/)
    {
        _onSubscribeSig(mid);
    }

    void
    onUnsubscribe(int mid)
    {
        _onUnsubscribeSig(mid);
    }

    void
    onPublish(int mid)
    {
        _onPublishSig(mid);
    }

    void
    onMessage(int mid, std::string_view topic, void* payload, size_t size)
    {
        _onMessageSig(mid, topic, payload, size);
    }

    void
    onLog(int logLevel, std::string_view message)
    {
        _onLogSig(logLevel, message);
    }

private:
    static void
    onMosquittoConnect(mosquitto* /*handle*/, void* data, int rc)
    {
        Impl* self = static_cast<Impl*>(data);
        BOOST_ASSERT(self != nullptr);
        self->onConnect(static_cast<MqttReturnCode>(rc));
    }

    static void
    onMosquittoDisconnect(mosquitto* /*handle*/, void* data, int rc)
    {
        Impl* self = static_cast<Impl*>(data);
        BOOST_ASSERT(self != nullptr);
        self->onDisconnect(static_cast<MqttReturnCode>(rc));
    }

    static void
    onMosquittoSubscribe(mosquitto* /*handle*/, void* data, int mid, int size, const int* qos)
    {
        Impl* self = static_cast<Impl*>(data);
        BOOST_ASSERT(self != nullptr);
        self->onSubscribe(mid, size, qos);
    }

    static void
    onMosquittoUnsubscribe(mosquitto* /*handle*/, void* data, int mid)
    {
        Impl* self = static_cast<Impl*>(data);
        BOOST_ASSERT(self != nullptr);
        self->onUnsubscribe(mid);
    }

    static void
    onMosquittoPublish(mosquitto* /*handle*/, void* data, int mid)
    {
        Impl* self = static_cast<Impl*>(data);
        BOOST_ASSERT(self != nullptr);
        self->onPublish(mid);
    }

    static void
    onMosquittoMessage(mosquitto* /*handle*/, void* data, const mosquitto_message* msg)
    {
        Impl* self = static_cast<Impl*>(data);
        BOOST_ASSERT(self != nullptr && msg != nullptr);
        self->onMessage(msg->mid, msg->topic, msg->payload, msg->payloadlen);
    }

    static void
    onMosquittoLog(mosquitto* /*handle*/, void* data, int logLevel, const char* message)
    {
        Impl* self = static_cast<Impl*>(data);
        BOOST_ASSERT(self != nullptr);
        self->onLog(logLevel, message);
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

    mosquitto* _handle{nullptr};
};

MqttBasicClient::MqttBasicClient(const char* id, bool cleanSession, bool getLogs)
    : _impl{std::make_unique<Impl>(id, cleanSession, getLogs)}
{
}

MqttBasicClient::~MqttBasicClient() = default;

bool
MqttBasicClient::hasConnection()
{
    BOOST_ASSERT(_impl);
    return _impl->hasConnection();
}

std::error_code
MqttBasicClient::credentials(std::string_view user, std::string_view password)
{
    BOOST_ASSERT(_impl);
    return _impl->credentials(user, password);
}

std::error_code
MqttBasicClient::connect(std::string_view host, uint16_t port, int keepAlive)
{
    BOOST_ASSERT(_impl);
    return _impl->connect(host, port, keepAlive);
}

std::error_code
MqttBasicClient::connectAsync(std::string_view host, uint16_t port, int keepAlive)
{
    BOOST_ASSERT(_impl);
    return _impl->connectAsync(host, port, keepAlive);
}

std::error_code
MqttBasicClient::reconnect()
{
    BOOST_ASSERT(_impl);
    return _impl->reconnect();
}

std::error_code
MqttBasicClient::reconnectAsync()
{
    BOOST_ASSERT(_impl);
    return _impl->reconnectAsync();
}

std::error_code
MqttBasicClient::disconnect()
{
    BOOST_ASSERT(_impl);
    return _impl->disconnect();
}

std::expected<int, std::error_code>
MqttBasicClient::subscribe(std::string_view topic, MqttQoS qos)
{
    BOOST_ASSERT(_impl);
    return _impl->subscribe(topic, qos);
}

std::expected<int, std::error_code>
MqttBasicClient::unsubscribe(std::string_view topic)
{
    BOOST_ASSERT(_impl);
    return _impl->unsubscribe(topic);
}

std::expected<int, std::error_code>
MqttBasicClient::publish(
    std::string_view topic, const void* payload, std::size_t size, MqttQoS qos, bool retain)
{
    BOOST_ASSERT(_impl);
    return _impl->publish(topic, payload, size, qos, retain);
}

std::expected<int, std::error_code>
MqttBasicClient::publish(std::string_view topic, std::string_view payload, MqttQoS qos, bool retain)
{
    BOOST_ASSERT(_impl);
    return _impl->publish(topic, payload, qos, retain);
}

sigc::connection
MqttBasicClient::onConnect(OnConnectSignal::slot_type&& slot)
{
    BOOST_ASSERT(_impl);
    return _impl->attachConnectSlot(std::move(slot));
}

sigc::connection
MqttBasicClient::onDisconnect(OnDisconnectSignal::slot_type&& slot)
{
    BOOST_ASSERT(_impl);
    return _impl->attachDisconnectSlot(std::move(slot));
}

sigc::connection
MqttBasicClient::onSubscribe(OnSubscribeSignal::slot_type&& slot)
{
    BOOST_ASSERT(_impl);
    return _impl->attachSubscribeSlot(std::move(slot));
}

sigc::connection
MqttBasicClient::onUnsubscribe(OnUnsubscribeSignal::slot_type&& slot)
{
    BOOST_ASSERT(_impl);
    return _impl->attachUnsubscribeSlot(std::move(slot));
}

sigc::connection
MqttBasicClient::onPublish(OnPublishSignal::slot_type&& slot)
{
    BOOST_ASSERT(_impl);
    return _impl->attachPublishSlot(std::move(slot));
}

sigc::connection
MqttBasicClient::onMessage(OnMessageSignal::slot_type&& slot)
{
    BOOST_ASSERT(_impl);
    return _impl->attachMessageSlot(std::move(slot));
}

sigc::connection
MqttBasicClient::onLog(OnLogSignal::slot_type&& slot)
{
    BOOST_ASSERT(_impl);
    return _impl->attachLogSlot(std::move(slot));
}

} // namespace jar