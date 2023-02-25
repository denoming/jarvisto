#pragma once

#include "jarvis/Export.hpp"

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/url.hpp>

#include <chrono>

namespace beast = boost::beast;
namespace sys = boost::system;
namespace http = beast::http;
namespace io = boost::asio;
namespace ssl = boost::asio::ssl;
namespace urls = boost::urls;

using tcp = boost::asio::ip::tcp;

namespace jar::net {

/* HTTP version codes */
static constexpr auto kHttpVersion10 = uint32_t{10u};
static constexpr auto kHttpVersion11 = uint32_t{11u};

/* HTTP default requests timeout */
static constexpr auto kHttpDefaultTimeout = std::chrono::seconds{30};

JARC_EXPORT bool
setTlsHostName(beast::ssl_stream<beast::tcp_stream>& stream,
               std::string_view hostname,
               sys::error_code& error);

JARC_EXPORT void
resetTimeout(beast::ssl_stream<beast::tcp_stream>& stream,
             std::chrono::seconds timeout = std::chrono::seconds{kHttpDefaultTimeout});

} // namespace jar::net