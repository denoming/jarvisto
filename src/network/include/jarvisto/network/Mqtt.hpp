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

#include "jarvisto/network/Export.hpp"

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

JARVISTO_NETWORK_EXPORT std::error_code
make_error_code(MqttErrorCode error);

} // namespace jar

namespace std {

template<>
struct is_error_code_enum<jar::MqttErrorCode> : true_type { };

} // namespace std