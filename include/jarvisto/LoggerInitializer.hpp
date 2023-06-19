#pragma once

#include "jarvisto/Export.hpp"

#include <spdlog/sinks/sink.h>

namespace jar {

class JARVISTO_EXPORT LoggerInitializer {
public:
    static LoggerInitializer&
    instance();

    void
    initialize();

    void
    initialize(const char* appId, const char* appDesc, const char* ctxId, const char* ctxDesc);

private:
    LoggerInitializer() = default;

private:
    bool _initialized{false};
};

} // namespace jar
