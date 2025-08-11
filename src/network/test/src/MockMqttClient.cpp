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