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
#include "jarvisto/network/Types.hpp"

#include <fmt/format.h>
#include <fmt/std.h>

template<>
struct fmt::formatter<jar::MqttReturnCode> : fmt::formatter<std::string_view> {
    template<typename FormatContext>
    auto
    format(jar::MqttReturnCode returnCode, FormatContext& context) const
    {
        std::string_view output{"Unknown"};
        switch (returnCode) {
            using enum jar::MqttReturnCode;
        case Accepted:
            output = "Accepted";
            break;
        case UnacceptableProtocolVersion:
            output = "UnacceptableProtocolVersion";
            break;
        case IdentifierRejected:
            output = "IdentifierRejected";
            break;
        case ServerUnavailable:
            output = "ServerUnavailable";
            break;
        case BadCredentials:
            output = "BadCredentials";
            break;
        case NotAuthorized:
            output = "NotAuthorized";
            break;
        default:
            break;
        }
        return formatter<std::string_view>::format(output, context);
    }
};

template<>
struct fmt::formatter<jar::MqttErrorCode> : fmt::formatter<std::string_view> {
    template<typename FormatContext>
    auto
    format(jar::MqttErrorCode errorCode, FormatContext& context) const
    {
        std::string_view output{"Unknown"};
        switch (errorCode) {
            using enum jar::MqttErrorCode;
        case Success:
            output = "Success";
            break;
        case NoConnection:
            output = "NoConnection";
            break;
        case NoMemory:
            output = "NoMemory";
            break;
        case TopicInvalid:
            output = "TopicInvalid";
            break;
        case PacketOversize:
            output = "PacketOversize";
            break;
        case PayloadOversize:
            output = "PayloadOversize";
            break;
        case InvalidArguments:
            output = "InvalidArguments";
            break;
        case ProtocolError:
            output = "ProtocolError";
            break;
        case SystemError:
            output = "SystemError";
            break;
        case QosNotSupported:
            output = "QosNotSupported";
            break;
        default:
            break;
        }
        return formatter<std::string_view>::format(output, context);
    }
};

template<>
struct fmt::formatter<jar::AvailabilityState> : fmt::formatter<std::string_view> {
    template<typename FormatContext>
    auto
    format(jar::AvailabilityState state, FormatContext& context) const
    {
        std::string_view output{"Unknown"};
        switch (state) {
            using enum jar::AvailabilityState;
        case Online:
            output = "Online";
            break;
        case Offline:
            output = "Offline";
            break;
        default:
            break;
        }
        return formatter<std::string_view>::format(output, context);
    }
};