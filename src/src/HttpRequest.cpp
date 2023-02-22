#include "jarvis/HttpRequest.hpp"

#include "jarvis/Logger.hpp"

namespace jar {

HttpRequest::Ptr
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
HttpRequest::doGET(std::string_view host,
                   std::string_view port,
                   std::string_view path,
                   SetterType&& setter)
{
    assert(!host.empty());
    assert(!port.empty());
    assert(!path.empty());

    _setter = std::move(setter);

    _req.version(net::kHttpVersion11);
    _req.method(http::verb::get);
    _req.target(path);
    _req.set(http::field::host, host);
    _req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    resolve(host, port);

    return _setter.getResult();
}

void
HttpRequest::resolve(std::string_view host, std::string_view port)
{
    _resolver.async_resolve(
        host,
        port,
        io::bind_cancellation_slot(
            onCancel(),
            beast::bind_front_handler(&HttpRequest::onResolveDone, shared_from_this())));
}

void
HttpRequest::onResolveDone(sys::error_code error, const tcp::resolver::results_type& endpoints)
{
    LOGD("Resolve given host address: <{}:{}>", host, port);

    if (error) {
        LOGE("Failed to resolve address: <{}>", error.message());
        _setter.setError(error);
        return;
    }

    LOGD("Resolve address was successful: <{}>", result.size());

    if (cancelled()) {
        LOGD("Operation was interrupted");
        _setter.setError(sys::errc::make_error_code(sys::errc::operation_canceled));
    } else {
        connect(endpoints);
    }
}

void
HttpRequest::connect(const tcp::resolver::results_type& endpoints)
{
    LOGD("Connect to host endpoints");

    net::resetTimeout(_stream);

    beast::get_lowest_layer(_stream).async_connect(
        endpoints,
        io::bind_cancellation_slot(
            onCancel(),
            beast::bind_front_handler(&HttpRequest::onConnectDone, shared_from_this())));
}

void
HttpRequest::onConnectDone(sys::error_code error,
                           const tcp::resolver::results_type::endpoint_type& endpoint)
{
    boost::ignore_unused(endpoint);

    if (error) {
        LOGE("Failed to connect: <{}>", error.message());
        _setter.setError(error);
        return;
    }

    LOGD("Connecting to host <{}> address was done", endpoint.address().to_string());

    if (cancelled()) {
        LOGD("Operation was interrupted");
        _setter.setError(sys::errc::make_error_code(sys::errc::operation_canceled));
    } else {
        handshake();
    }
}

void
HttpRequest::handshake()
{
    LOGD("Handshake with host");

    net::resetTimeout(_stream);

    _stream.async_handshake(
        ssl::stream_base::client,
        io::bind_cancellation_slot(
            onCancel(),
            beast::bind_front_handler(&HttpRequest::onHandshakeDone, shared_from_this())));
}

void
HttpRequest::onHandshakeDone(sys::error_code error)
{
    if (error) {
        LOGE("Failed to handshake: <{}>", error.message());
        _setter.setError(error);
        return;
    }

    LOGD("Handshake was successful");

    if (cancelled()) {
        LOGD("Operation was interrupted");
        _setter.setError(sys::errc::make_error_code(sys::errc::operation_canceled));
    } else {
        write();
    }
}

void
HttpRequest::write()
{
    LOGD("Write request to stream");

    net::resetTimeout(_stream);

    http::async_write(
        _stream,
        _req,
        io::bind_cancellation_slot(
            onCancel(), beast::bind_front_handler(&HttpRequest::onWriteDone, shared_from_this())));
}

void
HttpRequest::onWriteDone(sys::error_code error, std::size_t bytes)
{
    boost::ignore_unused(bytes);

    if (error) {
        LOGE("Failed to write request: <{}>", error.what());
        _setter.setError(error);
        return;
    }

    LOGD("Writing of request was successful: <{}> bytes", bytes);

    if (cancelled()) {
        LOGD("Operation was interrupted");
        _setter.setError(sys::errc::make_error_code(sys::errc::operation_canceled));
    } else {
        read();
    }
}

void
HttpRequest::read()
{
    LOGD("Read response from stream");

    net::resetTimeout(_stream);

    http::async_read(
        _stream,
        _buf,
        _res,
        io::bind_cancellation_slot(
            onCancel(), beast::bind_front_handler(&HttpRequest::onReadDone, shared_from_this())));
}

void
HttpRequest::onReadDone(sys::error_code error, std::size_t bytes)
{
    boost::ignore_unused(bytes);

    if (error) {
        LOGE("Failed to read response: <{}>", error.what());
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

    net::resetTimeout(_stream);

    _stream.async_shutdown(io::bind_cancellation_slot(
        onCancel(), beast::bind_front_handler(&HttpRequest::onShutdownDone, shared_from_this())));
}

void
HttpRequest::onShutdownDone(sys::error_code error)
{
    if (error == io::error::eof || error == io::error::operation_aborted) {
        error = {};
    }

    if (error == ssl::error::stream_truncated) {
        LOGD("Stream was truncated");
        error = {};
    }

    if (error) {
        LOGE("Failed to shutdown connection: <{}>", error.what());
    } else {
        LOGD("Shutdown of connection was successful");
    }

    _req.clear();
    _res.clear();
    _buf.clear();
}

} // namespace jar