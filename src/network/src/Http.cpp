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

#include "jarvisto/network/Http.hpp"

namespace jar {

void
setServerHostname(beast::ssl_stream<beast::tcp_stream>& stream,
                  std::string_view hostname,
                  std::error_code& error)
{
    auto* param = ::SSL_get0_param(stream.native_handle());
    ::X509_VERIFY_PARAM_set_hostflags(param, X509_CHECK_FLAG_NO_PARTIAL_WILDCARDS);
    if (X509_VERIFY_PARAM_set1_host(param, hostname.data(), hostname.size())) {
        error = {};
    } else {
        error = sys::error_code{static_cast<int>(::ERR_get_error()), io::error::get_ssl_category()};
    }
}

void
setSniHostname(beast::ssl_stream<beast::tcp_stream>& stream,
               std::string_view hostname,
               std::error_code& error)
{
    if (SSL_set_tlsext_host_name(stream.native_handle(), hostname.data())) {
        error = {};
    } else {
        error = sys::error_code{static_cast<int>(::ERR_get_error()), io::error::get_ssl_category()};
    }
}

void
resetTimeout(beast::ssl_stream<beast::tcp_stream>& stream, std::chrono::seconds timeout)
{
    beast::get_lowest_layer(stream).expires_after(timeout);
}

} // namespace jar