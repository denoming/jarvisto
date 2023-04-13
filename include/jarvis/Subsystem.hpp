#pragma once

#include "jarvis/CommonExport.hpp"

#include <memory>

namespace jar {

class Application;

class JARC_EXPORT Subsystem {
public:
    virtual ~Subsystem() = default;

    [[nodiscard]] virtual const char*
    name() const
        = 0;

protected:
    friend class Application;

    virtual void
    initialize(Application& application);

    virtual void
    setUp(Application& application);

    virtual void
    tearDown();

    virtual void
    finalize();
};

} // namespace jar