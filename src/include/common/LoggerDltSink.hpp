#pragma once

#include <spdlog/sinks/base_sink.h>
#include <dlt/dlt.h>

#include <mutex>

namespace jar {

class LoggerDltSink final : public spdlog::sinks::base_sink<std::mutex> {
public:
    LoggerDltSink(const char* contextId, const char* contextDesc) noexcept;

    ~LoggerDltSink() final;

    void
    sink_it_(const spdlog::details::log_msg& msg) final;

    void
    flush_() final;

private:
    std::string
    format(const spdlog::details::log_msg& message);

private:
    bool _registerApp{false};
    bool _registerCtx{false};
    DltContext _ctx{};
};

} // namespace jar