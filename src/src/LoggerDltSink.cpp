#include "common/LoggerDltSink.hpp"

#include "jarvis/Options.hpp"

#include <boost/assert.hpp>

namespace {

DltLogLevelType
toLogLevel(const spdlog::level::level_enum in) noexcept
{
    DltLogLevelType out{DLT_LOG_DEFAULT};
    switch (in) {
    case spdlog::level::trace:
        out = DLT_LOG_VERBOSE;
        break;
    case spdlog::level::debug:
        out = DLT_LOG_DEBUG;
        break;
    case spdlog::level::info:
        out = DLT_LOG_INFO;
        break;
    case spdlog::level::warn:
        out = DLT_LOG_WARN;
        break;
    case spdlog::level::err:
        out = DLT_LOG_ERROR;
        break;
    case spdlog::level::critical:
        out = DLT_LOG_FATAL;
        break;
    default:
        /* Use default value */
        break;
    }
    return out;
}

} // namespace

namespace jar {

LoggerDltSink::LoggerDltSink(const char* contextId, const char* contextDesc) noexcept
{
    if (dlt_init() == DLT_RETURN_OK) {
        if (dlt_register_app(JARVIS_DLT_APPID, JARVIS_DLT_APPID_DESC) == DLT_RETURN_OK) {
            _registerApp = true;
            _registerCtx = (dlt_register_context(&_ctx, contextId, contextDesc) == DLT_RETURN_OK);
        }
    }
}

LoggerDltSink::~LoggerDltSink()
{
    if (_registerCtx) {
        dlt_unregister_context(&_ctx);
    }
    if (_registerApp) {
        dlt_unregister_app_flush_buffered_logs();
    }
    dlt_free();
}

void
LoggerDltSink::sink_it_(const spdlog::details::log_msg& msg)
{
    const std::string formatted{format(msg)};
    DLT_LOG_STRING(_ctx, toLogLevel(msg.level), formatted.data());
}

void
LoggerDltSink::flush_()
{
    /* Do nothing */
}

std::string
LoggerDltSink::format(const spdlog::details::log_msg& message)
{
    spdlog::memory_buf_t buffer;
    BOOST_ASSERT(formatter_);
    formatter_->format(message, buffer);
    return fmt::to_string(buffer);
}

} // namespace jar