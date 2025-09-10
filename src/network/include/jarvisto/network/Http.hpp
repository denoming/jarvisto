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
#include "jarvisto/network/Asio.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>

#include <chrono>

namespace beast = boost::beast;
namespace http = beast::http;

namespace jar {

/* HTTP version codes */
static constexpr auto kHttpVersion10 = uint32_t{10u};
static constexpr auto kHttpVersion11 = uint32_t{11u};

/* HTTP default requests timeout */
static constexpr auto kHttpDefaultTimeout = std::chrono::seconds{30};

/**
 * Sets the expected server hostname, which will be checked during the verification process
 * @param stream - the TCP stream
 * @param hostname - the hostname
 * @param error - the error code in case of unsuccessful
 */
JARVISTO_NETWORK_EXPORT void
setServerHostname(beast::ssl_stream<beast::tcp_stream>& stream,
                  std::string_view hostname,
                  std::error_code& error);

/**
 * Sets the provided SNI hostname on the provided stream
 * @param stream - the TCP stream
 * @param hostname - the hostname
 * @param error - the error code in case of unsuccessful
 */
JARVISTO_NETWORK_EXPORT void
setSniHostname(beast::ssl_stream<beast::tcp_stream>& stream,
               std::string_view hostname,
               std::error_code& error);

/**
 * Reset timeout of any operation
 * @param stream - the TCP stream
 * @param timeout - the timeout of operation
 */
JARVISTO_NETWORK_EXPORT void
resetTimeout(beast::ssl_stream<beast::tcp_stream>& stream,
             std::chrono::seconds timeout = std::chrono::seconds{kHttpDefaultTimeout});

} // namespace jar