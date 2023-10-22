#pragma once

#include "jarvisto/Export.hpp"

#include <system_error>

namespace jar {

enum class MqttReturnCode : int {
    Accepted,
    UnacceptableProtocolVersion,
    IdentifierRejected,
    ServerUnavailable,
    BadCredentials,
    NotAuthorized,
    Unknown
};

enum class MqttQoS : int { Level0, Level1, Level2 };

enum class MqttErrorCode {
    Success = 0,
    NoConnection,
    NoMemory,
    TopicInvalid,
    PacketOversize,
    PayloadOversize,
    InvalidArguments,
    ProtocolError,
    SystemError,
    QosNotSupported,
    Unknown
};

JARVISTO_EXPORT std::error_code
make_error_code(MqttErrorCode error);

} // namespace jar

namespace std {

template<>
struct is_error_code_enum<jar::MqttErrorCode> : true_type { };

} // namespace std