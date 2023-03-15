#include "jarvis/LoggerInitializer.hpp"

#include "jarvis/Options.hpp"
#ifdef JARVIS_ENABLE_DLT
#include "common/LoggerDltSink.hpp"
#endif

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/pattern_formatter.h>
#include <spdlog/fmt/fmt.h>

#include <filesystem>

namespace fs = std::filesystem;

class ShortFilenameAndLine : public spdlog::custom_flag_formatter {
public:
    void
    format(const spdlog::details::log_msg& msg,
           const std::tm&,
           spdlog::memory_buf_t& buffer) override
    {
        static constexpr const char* kFullFormat{"{:.<35}"};
        static constexpr const char* kNullFormat{"{:^5}"};

        if (msg.source.empty()) {
            fmt::format_to(std::back_inserter(buffer), kNullFormat, std::string{"?:?"});
        } else {
            auto fileNameAndLine{fs::path{msg.source.filename}.filename().string()};
            fileNameAndLine += ':';
            fileNameAndLine += std::to_string(msg.source.line);
            fmt::format_to(std::back_inserter(buffer), kFullFormat, fileNameAndLine);
        }
    }

    [[nodiscard]] std::unique_ptr<custom_flag_formatter>
    clone() const override
    {
        return spdlog::details::make_unique<ShortFilenameAndLine>();
    }
};

namespace jar {

namespace {

void
addConsoleSink()
{
    static constexpr const char* kFormat{"[%Y-%m-%d %H:%M:%S.%e] [%P:%t] [%L] %-80!v [%*]"};
    auto formatter = std::make_unique<spdlog::pattern_formatter>();
    formatter->add_flag<ShortFilenameAndLine>('*').set_pattern(kFormat);

    const auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    sink->set_level(spdlog::level::debug);
    sink->set_formatter(std::move(formatter));
    spdlog::default_logger()->sinks().push_back(sink);
}

#ifdef JARVIS_ENABLE_DLT
void
addDltSink(const char* ctxId, const char* ctxDesc)
{
    static constexpr const char* kFormat{"[%P:%t] [%L] [%*] %-80!v"};
    auto formatter = std::make_unique<spdlog::pattern_formatter>();
    formatter->add_flag<ShortFilenameAndLine>('*').set_pattern(kFormat);

    const auto sink = std::make_shared<LoggerDltSink>(ctxId, ctxDesc);
    sink->set_formatter(std::move(formatter));
    spdlog::default_logger()->sinks().push_back(sink);
}
#endif

} // namespace

LoggerInitializer&
LoggerInitializer::instance()
{
    static LoggerInitializer instance;
    return instance;
}

void
LoggerInitializer::initialize()
{
    if (!_initialized) {
        addConsoleSink();
        _initialized = true;
    }
}

void
LoggerInitializer::initialize(const char* ctxId, const char* ctxDesc)
{
    if (!_initialized) {
        addConsoleSink();
#ifdef JARVIS_ENABLE_DLT
        addDltSink(ctxId, ctxDesc);
#endif
        _initialized = true;
    }
}

} // namespace jar