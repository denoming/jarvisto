#pragma once

#include "jarvisto/Export.hpp"

#include <memory>

namespace jar {

class Application;

class JARVISTO_EXPORT Subsystem {
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