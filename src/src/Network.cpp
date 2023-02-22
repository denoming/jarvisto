#include "jarvis/Network.hpp"

namespace jar::net {

bool
setTlsHostName(beast::ssl_stream<beast::tcp_stream>& stream,
               std::string_view hostname,
               sys::error_code& error)
{
    // Set SNI Hostname (many hosts need this to handshake successfully)
    if (SSL_set_tlsext_host_name(stream.native_handle(), hostname.data())) {
        error = {};
    } else {
        error
            = sys::error_code{static_cast<int>(::ERR_get_error()), io::error::get_ssl_category()};
    }
    return !error;
}

void
resetTimeout(beast::ssl_stream<beast::tcp_stream>& stream, std::chrono::seconds timeout)
{
    beast::get_lowest_layer(stream).expires_after(timeout);
}

} // namespace jar