#pragma once

#include "jarvis/Network.hpp"

#include <memory>

namespace jar {

class IWorker {
public:
    using Ptr = std::unique_ptr<IWorker>;

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