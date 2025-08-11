#pragma once

#include "jarvisto/network/Http.hpp"

namespace jar {

class IWorker {
public:
    virtual ~IWorker() = default;

    [[nodiscard]] virtual bool
    active() const
        = 0;

    virtual void
    start()
        = 0;

    virtual void
    stop()
        = 0;

    virtual io::any_io_executor
    executor()
        = 0;

    virtual IWorker&
    current()
        = 0;

    virtual IWorker&
    next()
        = 0;
};

} // namespace jar