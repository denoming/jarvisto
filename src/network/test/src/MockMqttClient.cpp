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

#include "network/MockMqttClient.hpp"

using namespace testing;

namespace jar {

MockMqttClient::MockMqttClient()
{
    ON_CALL(*this, doHasConnection).WillByDefault(Return(false));

    ON_CALL(*this, doCredentials)
        .WillByDefault(Return(std::make_error_code(std::errc::not_supported)));

    ON_CALL(*this, doConnect).WillByDefault(Return(std::make_error_code(std::errc::not_supported)));

    ON_CALL(*this, doConnectAsync)
        .WillByDefault(Return(std::make_error_code(std::errc::not_supported)));

    ON_CALL(*this, doReconnect)
        .WillByDefault(Return(std::make_error_code(std::errc::not_supported)));

    ON_CALL(*this, doReconnectAsync)
        .WillByDefault(Return(std::make_error_code(std::errc::not_supported)));

    ON_CALL(*this, doDisconnect)
        .WillByDefault(Return(std::make_error_code(std::errc::not_supported)));

    ON_CALL(*this, doSubscribe)
        .WillByDefault(Return(std::unexpected(std::make_error_code(std::errc::not_supported))));

    ON_CALL(*this, doUnsubscribe)
        .WillByDefault(Return(std::unexpected(std::make_error_code(std::errc::not_supported))));

    ON_CALL(*this, doPublish(_, _, _, _, _))
        .WillByDefault(Return(std::unexpected(std::make_error_code(std::errc::not_supported))));

    ON_CALL(*this, doPublish(_, _, _, _))
        .WillByDefault(Return(std::unexpected(std::make_error_code(std::errc::not_supported))));
}

} // namespace jar