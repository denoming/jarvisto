#include "jarvisto/network/MqttClient.hpp"

#include <mosquitto.h>

#include <mosquittopp.h>

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

#define MOSQ_CAST(handle) (static_cast<mosquitto*>(handle))

MqttClient::MqttClient(const char* id, const bool cleanSession, const bool logging)
{
    static Initializer initializer;

    if (_handle = mosquitto_new(id, cleanSession, this); _handle == nullptr) {
        throw std::runtime_error{"Unable to initialise mosquitto client instance"};
    }

    mosquitto_connect_callback_set(MOSQ_CAST(_handle), [](mosquitto*, void* data, int rc) {
        if (auto* self = static_cast<MqttClient*>(data); self != nullptr) {
            const auto returnCode = static_cast<MqttReturnCode>(rc);
            self->handleConnect(returnCode);
            self->notifyConnect(returnCode);
        }
    });
    mosquitto_disconnect_callback_set(MOSQ_CAST(_handle), [](mosquitto*, void* data, int rc) {
        if (auto* self = static_cast<MqttClient*>(data); self != nullptr) {
            const auto returnCode = static_cast<MqttReturnCode>(rc);
            self->handleDisconnect(returnCode);
            self->notifyDisconnect(returnCode);
        }
    });
    mosquitto_subscribe_callback_set(
        MOSQ_CAST(_handle),
        [](mosquitto*, void* data, const int mid, int /*size*/, const int* /*qos*/) {
            if (const auto* self = static_cast<MqttClient*>(data); self != nullptr) {
                self->notifySubscribe(mid);
            }
        });
    mosquitto_unsubscribe_callback_set(
        MOSQ_CAST(_handle), [](mosquitto*, void* data, const int mid) {
            if (const auto* self = static_cast<MqttClient*>(data); self != nullptr) {
                self->notifyUnsubscribe(mid);
            }
        });
    mosquitto_publish_callback_set(MOSQ_CAST(_handle), [](mosquitto*, void* data, const int mid) {
        if (const auto* self = static_cast<MqttClient*>(data); self != nullptr) {
            self->notifyPublish(mid);
        }
    });
    mosquitto_message_callback_set(
        MOSQ_CAST(_handle), [](mosquitto*, void* data, const mosquitto_message* msg) {
            if (const auto* self = static_cast<MqttClient*>(data); self != nullptr) {
                self->notifyMessage(msg->mid, msg->topic, msg->payload, msg->payloadlen);
            }
        });
    if (logging) {
        mosquitto_log_callback_set(
            MOSQ_CAST(_handle), [](mosquitto*, void* data, const int logLevel, const char* msg) {
                if (const auto* self = static_cast<MqttClient*>(data); self != nullptr) {
                    self->notifyLog(logLevel, msg);
                }
            });
    }

    if (mosquitto_loop_start(MOSQ_CAST(_handle)) != MOSQ_ERR_SUCCESS) {
        mosquitto_destroy(MOSQ_CAST(_handle));
        _handle = nullptr;
        throw std::runtime_error{"Unable to start mosquitto loop"};
    }
}

MqttClient::~MqttClient()
{
    std::ignore = mosquitto_loop_stop(MOSQ_CAST(_handle), true);
    mosquitto_destroy(MOSQ_CAST(_handle));
}

bool
MqttClient::hasConnection() const
{
    return doHasConnection();
}

std::error_code
MqttClient::credentials(const std::string_view user, const std::string_view password) const
{
    return doCredentials(user, password);
}

std::error_code
MqttClient::connect(const std::string_view host, const int port, const int keepAlive) const
{
    return doConnect(host, port, keepAlive);
}

std::error_code
MqttClient::connectAsync(const std::string_view host, const int port, const int keepAlive) const
{
    return doConnectAsync(host, port, keepAlive);
}

std::error_code
MqttClient::reconnect() const
{
    return doReconnect();
}

std::error_code
MqttClient::reconnectAsync() const
{
    return doReconnectAsync();
}

std::error_code
MqttClient::disconnect() const
{
    return doDisconnect();
}

std::expected<int, std::error_code>
MqttClient::subscribe(const std::string_view topic, const MqttQoS qos) const
{
    return doSubscribe(topic, qos);
}

std::expected<int, std::error_code>
MqttClient::unsubscribe(const std::string_view topic) const
{
    return doUnsubscribe(topic);
}

std::expected<int, std::error_code>
MqttClient::publish(const std::string_view topic,
                    const void* payload,
                    const std::size_t size,
                    const MqttQoS qos,
                    const bool retain) const
{
    return doPublish(topic, payload, size, qos, retain);
}

std::expected<int, std::error_code>
MqttClient::publish(const std::string_view topic,
                    const std::string_view payload,
                    const MqttQoS qos,
                    const bool retain) const
{
    return doPublish(topic, payload, qos, retain);
}

MqttClient::OnConnectSignal
MqttClient::onConnect() const
{
    return _onConnectSig;
}

MqttClient::OnDisconnectSignal
MqttClient::onDisconnect() const
{
    return _onDisconnectSig;
}

MqttClient::OnSubscribeSignal
MqttClient::onSubscribe() const
{
    return _onSubscribeSig;
}

MqttClient::OnUnsubscribeSignal
MqttClient::onUnsubscribe() const
{
    return _onUnsubscribeSig;
}

MqttClient::OnPublishSignal
MqttClient::onPublish() const
{
    return _onPublishSig;
}

MqttClient::OnMessageSignal
MqttClient::onMessage() const
{
    return _onMessageSig;
}

MqttClient::OnLogSignal
MqttClient::onLog() const
{
    return _onLogSig;
}

bool
MqttClient::doHasConnection() const
{
    return _hasConnection;
}

std::error_code
MqttClient::doCredentials(const std::string_view user, const std::string_view password) const
{
    return toError(mosquitto_username_pw_set(MOSQ_CAST(_handle), user.data(), password.data()));
}

std::error_code
MqttClient::doConnect(const std::string_view host, const int port, const int keepAlive) const
{
    return toError(mosquitto_connect(MOSQ_CAST(_handle), host.data(), port, keepAlive));
}

std::error_code
MqttClient::doConnectAsync(const std::string_view host, const int port, const int keepAlive) const
{
    return toError(mosquitto_connect_async(MOSQ_CAST(_handle), host.data(), port, keepAlive));
}

std::error_code
MqttClient::doReconnect() const
{
    return toError(mosquitto_reconnect(MOSQ_CAST(_handle)));
}

std::error_code
MqttClient::doReconnectAsync() const
{
    return toError(mosquitto_reconnect_async(MOSQ_CAST(_handle)));
}

std::error_code
MqttClient::doDisconnect() const
{
    return toError(mosquitto_disconnect(MOSQ_CAST(_handle)));
}

std::expected<int, std::error_code>
MqttClient::doSubscribe(const std::string_view topic, const MqttQoS qos) const
{
    int mid{};
    const int rc
        = mosquitto_subscribe(MOSQ_CAST(_handle), &mid, topic.data(), static_cast<int>(qos));
    if (rc != MOSQ_ERR_SUCCESS) {
        return std::unexpected(toError(rc));
    }
    return mid;
}

std::expected<int, std::error_code>
MqttClient::doUnsubscribe(const std::string_view topic) const
{
    int mid{};
    if (const int rc = mosquitto_unsubscribe(MOSQ_CAST(_handle), &mid, topic.data());
        rc != MOSQ_ERR_SUCCESS) {
        return std::unexpected(toError(rc));
    }
    return mid;
}

std::expected<int, std::error_code>
MqttClient::doPublish(const std::string_view topic,
                      const void* payload,
                      const std::size_t size,
                      const MqttQoS qos,
                      const bool retain) const
{
    int mid{};
    const int rc = mosquitto_publish(MOSQ_CAST(_handle),
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

std::expected<int, std::error_code>
MqttClient::doPublish(const std::string_view topic,
                      const std::string_view payload,
                      const MqttQoS qos,
                      const bool retain) const
{
    return publish(topic, payload.data(), payload.size(), qos, retain);
}

void
MqttClient::notifyConnect(const MqttReturnCode rc) const
{
    _onConnectSig(rc);
}

void
MqttClient::notifyDisconnect(const MqttReturnCode rc) const
{
    _onDisconnectSig(rc);
}

void
MqttClient::notifySubscribe(const int mid) const
{
    _onSubscribeSig(mid);
}

void
MqttClient::notifyUnsubscribe(const int mid) const
{
    _onUnsubscribeSig(mid);
}

void
MqttClient::notifyPublish(const int mid) const
{
    _onPublishSig(mid);
}

void
MqttClient::notifyMessage(const int mid,
                          const std::string_view topic,
                          void* payload,
                          const size_t size) const
{
    _onMessageSig(mid, topic, payload, size);
}

void
MqttClient::notifyLog(const int logLevel, const std::string_view message) const
{
    _onLogSig(logLevel, message);
}

void
MqttClient::handleConnect(const MqttReturnCode rc)
{
    if (rc == MqttReturnCode::Accepted) {
        _hasConnection = true;
    }
}

void
MqttClient::handleDisconnect(const MqttReturnCode /*rc*/)
{
    _hasConnection = false;
}

} // namespace jar