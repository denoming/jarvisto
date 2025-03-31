#pragma once

#include "jarvisto/core/Export.hpp"

#include <spdlog/spdlog.h>

namespace jar {

class JARVISTO_CORE_EXPORT LoggerFactory {
public:
    virtual ~LoggerFactory() = default;

    std::shared_ptr<spdlog::logger>
    create(const std::string& loggerName, int loggerLevel, bool useByDefault = true);

protected:
    virtual void
    addSinks(std::shared_ptr<spdlog::logger> logger, int logLevel);

    static void
    addConsoleSink(std::shared_ptr<spdlog::logger> logger,
                   int loggerLevel = SPDLOG_LEVEL_DEBUG,
                   bool color = false);

    static void
    addFileSink(std::shared_ptr<spdlog::logger> logger,
                const std::string& fileName,
                int loggerLevel = SPDLOG_LEVEL_DEBUG,
                bool truncate = false,
                const spdlog::file_event_handlers& handlers = {});

    static void
    addSystemdSink(std::shared_ptr<spdlog::logger> logger,
                   int loggerLevel = SPDLOG_LEVEL_DEBUG,
                   std::string ident = "",
                   bool formatting = false);

private:
    virtual std::shared_ptr<spdlog::logger>
    createLogger(const std::string& loggerName, int loggerLevel, bool useByDefault);
};

} // namespace jar