#pragma once

#include "jarvis/CommonExport.hpp"
#include "jarvis/IWorker.hpp"
#include "jarvis/Network.hpp"

#include <vector>
#include <memory>

namespace jar {

class JARC_EXPORT WorkerGroup final : public IWorker {
public:
    explicit WorkerGroup(std::size_t workersNum, std::size_t threadsPerWorker = 1);

    [[nodiscard]] bool
    active() const final;

    void
    start() final;

    void
    stop() final;

    io::any_io_executor
    executor() final;

    IWorker&
    current() final;

    IWorker&
    next() final;

private:
    std::size_t _index;
    std::vector<std::unique_ptr<IWorker>> _workers;
};

} // namespace jar