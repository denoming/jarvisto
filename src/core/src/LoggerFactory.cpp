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

#include "jarvisto/core/LoggerFactory.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/systemd_sink.h>

#include <filesystem>

namespace fs = std::filesystem;

using namespace spdlog;

class ShortFilenameAndLine final : public custom_flag_formatter {
public:
    void
    format(const details::log_msg& msg, const std::tm&, memory_buf_t& buffer) override
    {
        static constexpr auto kFullFormat{"{:.<35}"};
        static constexpr auto kNullFormat{"{:^5}"};

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
        return details::make_unique<ShortFilenameAndLine>();
    }
};

static std::unique_ptr<pattern_formatter>
getCustomPatternFormatter()
{
    static constexpr auto kFormat{"[%Y-%m-%d %H:%M:%S.%e] [%P:%t] [%L] [%*] %v"};
    auto formatter = std::make_unique<pattern_formatter>();
    formatter->add_flag<ShortFilenameAndLine>('*').set_pattern(kFormat);
    return formatter;
}

namespace jar {

std::shared_ptr<spdlog::logger>
LoggerFactory::create(const std::string& loggerName, const int loggerLevel, const bool setDefault)
{
    auto logger = createLogger(loggerName, loggerLevel, setDefault);
    if (logger) {
        addSinks(logger, loggerLevel);
    }
    return logger;
}

void
LoggerFactory::addSinks(std::shared_ptr<spdlog::logger> logger, const int logLevel)
{
    addConsoleSink(std::move(logger), logLevel);
}

void
LoggerFactory::addConsoleSink(std::shared_ptr<spdlog::logger> logger /* NOLINT */,
                              const int loggerLevel,
                              const bool color)
{
    sink_ptr sink;
    if (color) {
        sink = std::make_shared<sinks::stdout_color_sink_mt>();
    } else {
        sink = std::make_shared<sinks::stdout_sink_mt>();
    }
    sink->set_level(static_cast<level::level_enum>(loggerLevel));
    sink->set_formatter(getCustomPatternFormatter());
    logger->sinks().push_back(std::move(sink));
}

void
LoggerFactory::addFileSink(std::shared_ptr<spdlog::logger> logger /* NOLINT */,
                           const std::string& fileName,
                           const int loggerLevel,
                           bool truncate,
                           const file_event_handlers& handlers)
{
    auto sink = std::make_shared<sinks::basic_file_sink_mt>(fileName, truncate, handlers);
    sink->set_level(static_cast<level::level_enum>(loggerLevel));
    sink->set_formatter(getCustomPatternFormatter());
    logger->sinks().push_back(std::move(sink));
}

void
LoggerFactory::addSystemdSink(std::shared_ptr<spdlog::logger> logger /* NOLINT */,
                              int loggerLevel,
                              std::string ident,
                              bool formatting)
{
    auto sink = std::make_shared<sinks::systemd_sink_mt>(std::move(ident), formatting);
    sink->set_level(static_cast<level::level_enum>(loggerLevel));
    sink->set_formatter(getCustomPatternFormatter());
    logger->sinks().push_back(std::move(sink));
}

std::shared_ptr<spdlog::logger>
LoggerFactory::createLogger(const std::string& loggerName,
                            const int loggerLevel,
                            const bool useByDefault)
{
    const auto logger = std::make_shared<spdlog::logger>(loggerName);
    logger->set_level(static_cast<level::level_enum>(loggerLevel));
    if (useByDefault) {
        set_default_logger(logger);
    }
    return logger;
}

} // namespace jar