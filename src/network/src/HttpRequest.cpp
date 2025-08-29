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

#include "jarvisto/network/HttpRequest.hpp"

#include "jarvisto/core/Logger.hpp"

namespace jar {

std::shared_ptr<HttpRequest>
HttpRequest::create(io::any_io_executor executor, ssl::context& context)
{
    return std::shared_ptr<HttpRequest>(new HttpRequest{std::move(executor), context});
}

HttpRequest::HttpRequest(io::any_io_executor executor, ssl::context& context)
    : _executor{std::move(executor)}
    , _resolver{_executor}
    , _stream{_executor, context}
{
}

bool
HttpRequest::pending() const
{
    return !_setter.dirty();
}

HttpRequest::ResultType
HttpRequest::GET(const urls::url& url, const http::fields& fields)
{
    return doGET(url, fields, makeResultSetter<std::string>());
}

HttpRequest::ResultType
HttpRequest::GET(const urls::url& url,
                 std::move_only_function<OnReady> onReady,
                 std::move_only_function<OnError> onError,
                 const http::fields& fields)
{
    auto&& setter = makeResultSetter<UnderlyingType>(std::move(onReady), std::move(onError));
    return doGET(url, fields, std::move(setter));
}

HttpRequest::ResultType
HttpRequest::doGET(const urls::url& url, const http::fields& fields, SetterType&& setter)
{
    assert(!url.empty());

    _setter = std::move(setter);

    std::error_code error;
    setServerHostname(_stream, url.host(), error);
    if (error) {
        LOGW("Unable to set server to use in verification process");
        error = {};
    }
    setSniHostname(_stream, url.host(), error);
    if (error) {
        LOGW("Unable to set SNI hostname");
    }

    _req.version(kHttpVersion11);
    _req.method(http::verb::get);
    _req.target(url.encoded_resource());
    _req.set(http::field::host, url.encoded_host());
    _req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    /* Set all custom fields to HTTP request */
    std::for_each(fields.cbegin(), fields.cend(), [&](const http::fields::value_type& f) {
        _req.set(f.name_string(), f.value());
    });

    resolve(url.encoded_host(), url.has_port() ? url.port() : "https");

    return _setter.getResult();
}

void
HttpRequest::resolve(std::string_view host, std::string_view port)
{
    LOGD("Resolve given host address: <{}:{}>", host, port);

    _resolver.async_resolve(
        host,
        port,
        io::bind_cancellation_slot(
            onCancel(),
            beast::bind_front_handler(&HttpRequest::onResolveDone, shared_from_this())));
}

void
HttpRequest::onResolveDone(std::error_code error, const tcp::resolver::results_type& endpoints)
{
    if (error) {
        LOGE("Failed to resolve address: <{}>", error.message());
        _setter.setError(error);
        return;
    }

    if (cancelled()) {
        LOGD("Operation was interrupted");
        _setter.setError(sys::errc::make_error_code(sys::errc::operation_canceled));
    } else {
        LOGD("Resolve address was successful: <{}>", endpoints.size());
        connect(endpoints);
    }
}

void
HttpRequest::connect(const tcp::resolver::results_type& endpoints)
{
    LOGD("Connect to host endpoints");

    resetTimeout(_stream);

    beast::get_lowest_layer(_stream).async_connect(
        endpoints,
        io::bind_cancellation_slot(
            onCancel(),
            beast::bind_front_handler(&HttpRequest::onConnectDone, shared_from_this())));
}

void
HttpRequest::onConnectDone(std::error_code error,
                           const tcp::resolver::results_type::endpoint_type& endpoint)
{
    if (error) {
        LOGE("Failed to connect: <{}>", error.message());
        _setter.setError(error);
        return;
    }

    if (cancelled()) {
        LOGD("Operation was interrupted");
        _setter.setError(sys::errc::make_error_code(sys::errc::operation_canceled));
    } else {
        LOGD("Connecting to host <{}> address was done", endpoint.address().to_string());
        handshake();
    }
}

void
HttpRequest::handshake()
{
    LOGD("Handshake with host");

    resetTimeout(_stream);

    _stream.async_handshake(
        ssl::stream_base::client,
        io::bind_cancellation_slot(
            onCancel(),
            beast::bind_front_handler(&HttpRequest::onHandshakeDone, shared_from_this())));
}

void
HttpRequest::onHandshakeDone(std::error_code error)
{
    if (error) {
        LOGE(" >>> Failed to handshake <<<: <{}>", error.message());
        _setter.setError(error);
        return;
    }

    if (cancelled()) {
        LOGD("Operation was interrupted");
        _setter.setError(sys::errc::make_error_code(sys::errc::operation_canceled));
    } else {
        LOGD("Handshake was successful");
        write();
    }
}

void
HttpRequest::write()
{
    LOGD("Write request to stream");

    resetTimeout(_stream);

    http::async_write(
        _stream,
        _req,
        io::bind_cancellation_slot(
            onCancel(), beast::bind_front_handler(&HttpRequest::onWriteDone, shared_from_this())));
}

void
HttpRequest::onWriteDone(std::error_code error, std::size_t bytes)
{
    if (error) {
        LOGE("Failed to write request: <{}>", error.message());
        _setter.setError(error);
        return;
    }

    if (cancelled()) {
        LOGD("Operation was interrupted");
        _setter.setError(sys::errc::make_error_code(sys::errc::operation_canceled));
    } else {
        LOGD("Writing of request was successful: <{}> bytes", bytes);
        read();
    }
}

void
HttpRequest::read()
{
    LOGD("Read response from stream");

    resetTimeout(_stream);

    http::async_read(
        _stream,
        _buf,
        _res,
        io::bind_cancellation_slot(
            onCancel(), beast::bind_front_handler(&HttpRequest::onReadDone, shared_from_this())));
}

void
HttpRequest::onReadDone(std::error_code error, std::size_t bytes)
{
    if (error) {
        LOGE("Failed to read response: <{}>", error.message());
        _setter.setError(error);
        return;
    }

    LOGD("Reading of response was successful: <{}> bytes", bytes);
    _setter.setValue(std::move(_res.body()));

    if (cancelled()) {
        LOGD("Operation was interrupted");
        _setter.setError(sys::errc::make_error_code(sys::errc::operation_canceled));
    } else {
        shutdown();
    }
}

void
HttpRequest::shutdown()
{
    LOGD("Shutdown connection with host");

    resetTimeout(_stream);

    _stream.async_shutdown(io::bind_cancellation_slot(
        onCancel(), beast::bind_front_handler(&HttpRequest::onShutdownDone, shared_from_this())));
}

void
HttpRequest::onShutdownDone(std::error_code error)
{
    if (error == io::error::make_error_code(io::error::eof)) {
        error = {};
    }
    if (error == io::error::make_error_code(io::error::operation_aborted)) {
        error = {};
    }
    if (error == ssl::error::make_error_code(ssl::error::stream_truncated)) {
        error = {};
    }

    if (error) {
        LOGE("Failed to shutdown connection: <{}>", error.message());
    } else {
        LOGD("Shutdown of connection was successful");
    }

    _req.clear();
    _res.clear();
    _buf.clear();
}

} // namespace jar