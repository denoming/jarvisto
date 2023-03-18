#pragma once

#include "jarvis/CommonExport.hpp"
#include "jarvis/AsyncResult.hpp"
#include "jarvis/Cancellable.hpp"
#include "jarvis/Network.hpp"

#include <concepts>
#include <memory>
#include <string_view>

namespace jar {

class JARC_EXPORT HttpRequest final : public std::enable_shared_from_this<HttpRequest>,
                                      public Cancellable {
public:
    using UnderlyingType = std::string;
    using SetterType = AsyncResultSetter<UnderlyingType>;
    using ResultType = AsyncResult<UnderlyingType>;
    using Ptr = std::shared_ptr<HttpRequest>;

    [[nodiscard]] static Ptr
    create(io::any_io_executor executor, ssl::context& context);

    [[nodiscard]] bool
    pending() const;

    [[nodiscard]] ResultType
    GET(const urls::url& url, const http::fields& fields = {})
    {
        return doGET(url, fields, makeResultSetter<std::string>());
    }

    [[maybe_unused]] ResultType
    GET(const urls::url& url,
        std::invocable<const UnderlyingType&> auto&& onReady,
        std::invocable<std::error_code> auto&& onError,
        const http::fields& fields = {})
    {
        auto&& setter = makeResultSetter<UnderlyingType>(std::forward<decltype(onReady)>(onReady),
                                                         std::forward<decltype(onError)>(onError));
        return doGET(url, fields, std::move(setter));
    }

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