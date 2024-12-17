#include "jarvisto/core/LoggerInitializer.hpp"

#include "jarvisto/Options.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <fmt/format.h>

#include <filesystem>

namespace fs = std::filesystem;

class ShortFilenameAndLine final : public spdlog::custom_flag_formatter {
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
setupDefaultLogger()
{
    auto logger = std::make_shared<spdlog::logger>("MAIN");
    logger->set_level(spdlog::level::debug);
    spdlog::set_default_logger(logger);
}

void
addConsoleSink()
{
    static constexpr const char* kFormat{"[%Y-%m-%d %H:%M:%S.%e] [%P:%t] [%L] [%*] %v"};
    auto formatter = std::make_unique<spdlog::pattern_formatter>();
    formatter->add_flag<ShortFilenameAndLine>('*').set_pattern(kFormat);

    const auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    sink->set_level(spdlog::level::debug);
    sink->set_formatter(std::move(formatter));
    spdlog::default_logger()->sinks().push_back(sink);
}

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
        setupDefaultLogger();
        addConsoleSink();
        _initialized = true;
    }
}

void
LoggerInitializer::initialize(const char* appId,
                              const char* appDesc,
                              const char* ctxId,
                              const char* ctxDesc)
{
    if (!_initialized) {
        setupDefaultLogger();
        addConsoleSink();
        _initialized = true;
    }
}

} // namespace jar