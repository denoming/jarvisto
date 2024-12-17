#include "jarvisto/network/Mqtt.hpp"

#include <mosquitto.h>

namespace jar {

namespace {

struct ErrorCategory final : std::error_category {
    [[nodiscard]] const char*
    name() const noexcept final
    {
        return "mqtt";
    }

    [[nodiscard]] std::string
    message(int ev) const final
    {
        switch (static_cast<MqttErrorCode>(ev)) {
            using enum MqttErrorCode;
        case Success:
            return "operation succeeded";
        case NoConnection:
            return "no connection to a broker";
        case NoMemory:
            return "out of memory condition occurred";
        case TopicInvalid:
            return "topic is not valid UTF-8 character sequence";
        case PacketOversize:
            return "resulting packet is too large";
        case PayloadOversize:
            return "message payload is too large";
        case InvalidArguments:
            return "input arguments are invalid";
        case ProtocolError:
            return "communication protocol error occurred";
        case SystemError:
            return "system error occurred";
        case QosNotSupported:
            return "invalid QoS value";
        case Unknown:
            return "unknown error occurred";
        default:
            return "unrecognized error";
        }
    }
};

const ErrorCategory kCategory{};

} // namespace

std::error_code
make_error_code(const MqttErrorCode ec)
{
    return {static_cast<int>(ec), kCategory};
}

} // namespace jar