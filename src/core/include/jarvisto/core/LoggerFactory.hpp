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

#include "jarvisto/core/Export.hpp"

#include <spdlog/spdlog.h>

namespace jar {

class JARVISTO_CORE_EXPORT LoggerFactory {
public:
    virtual ~LoggerFactory() = default;

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

    std::shared_ptr<spdlog::logger>
    create(const std::string& loggerName, int loggerLevel, bool useByDefault = true);

protected:
    virtual void
    addSinks(std::shared_ptr<spdlog::logger> logger, int logLevel);

private:
    virtual std::shared_ptr<spdlog::logger>
    createLogger(const std::string& loggerName, int loggerLevel, bool useByDefault);
};

} // namespace jar