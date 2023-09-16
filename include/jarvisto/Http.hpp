#pragma once

#include "jarvisto/Export.hpp"
#include "jarvisto/Asio.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/url.hpp>

#include <chrono>

namespace beast = boost::beast;
namespace http = beast::http;
namespace urls = boost::urls;

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
JARVISTO_EXPORT void
setServerHostname(beast::ssl_stream<beast::tcp_stream>& stream,
                  std::string_view hostname,
                  std::error_code& error);

/**
 * Sets the provided SNI hostname on the provided stream
 * @param stream - the TCP stream
 * @param hostname - the hostname
 * @param error - the error code in case of unsuccessful
 */
JARVISTO_EXPORT void
setSniHostname(beast::ssl_stream<beast::tcp_stream>& stream,
               std::string_view hostname,
               std::error_code& error);

/**
 * Reset timeout of any operation
 * @param stream - the TCP stream
 * @param timeout - the timeout of operation
 */
JARVISTO_EXPORT void
resetTimeout(beast::ssl_stream<beast::tcp_stream>& stream,
             std::chrono::seconds timeout = std::chrono::seconds{kHttpDefaultTimeout});

} // namespace jar