#pragma once

#include "jarvisto/Export.hpp"

namespace jar {

class JARVISTO_EXPORT LoggerInitializer {
public:
    static LoggerInitializer&
    instance();

    LoggerInitializer() = default;

    void
    initialize();

    void
    initialize(const char* ctxId, const char* ctxDesc);

private:
    bool _initialized{false};
};

} // namespace jar
