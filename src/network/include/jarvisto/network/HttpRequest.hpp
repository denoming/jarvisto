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
#include "jarvisto/network/AsyncResult.hpp"
#include "jarvisto/network/Cancellable.hpp"
#include "jarvisto/network/Http.hpp"

#include <functional>
#include <memory>
#include <string_view>
#include <system_error>

namespace jar {

class JARVISTO_NETWORK_EXPORT HttpRequest final : public std::enable_shared_from_this<HttpRequest>,
                                                  public Cancellable {
public:
    using UnderlyingType = std::string;
    using SetterType = AsyncResultSetter<UnderlyingType>;
    using ResultType = AsyncResult<UnderlyingType>;

    using OnReady = void(const UnderlyingType&);
    using OnError = void(std::error_code);

    [[nodiscard]] static std::shared_ptr<HttpRequest>
    create(io::any_io_executor executor, ssl::context& context);

    [[nodiscard]] bool
    pending() const;

    [[nodiscard]] ResultType
    GET(const urls::url& url, const http::fields& fields = {});

    [[maybe_unused]] ResultType
    GET(const urls::url& url,
        std::move_only_function<OnReady> onReady,
        std::move_only_function<OnError> onError,
        const http::fields& fields = {});

private:
    HttpRequest(io::any_io_executor executor, ssl::context& context);

    [[nodiscard]] ResultType
    doGET(const urls::url& url, const http::fields& fields, SetterType&& setter);

    void
    resolve(std::string_view host, std::string_view port);

    void
    onResolveDone(std::error_code error, const tcp::resolver::results_type& endpoints);

    void
    connect(const tcp::resolver::results_type& endpoints);

    void
    onConnectDone(std::error_code error,
                  const tcp::resolver::results_type::endpoint_type& endpoint);

    void
    handshake();

    void
    onHandshakeDone(std::error_code error);

    void
    write();

    void
    onWriteDone(std::error_code error, std::size_t bytes);

    void
    read();

    void
    onReadDone(std::error_code error, std::size_t bytes);

    void
    shutdown();

    void
    onShutdownDone(std::error_code error);

private:
    SetterType _setter;
    beast::flat_buffer _buf;
    http::request<http::empty_body> _req;
    http::response<http::string_body> _res;
    io::any_io_executor _executor;
    beast::ssl_stream<beast::tcp_stream> _stream;
    tcp::resolver _resolver;
};

} // namespace jar