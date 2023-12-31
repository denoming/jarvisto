#pragma once

#include "jarvisto/Export.hpp"
#include "jarvisto/IWorker.hpp"
#include "jarvisto/Http.hpp"

#include <thread>
#include <vector>

namespace jar {

class JARVISTO_EXPORT Worker final : public IWorker {
public:
    explicit Worker(std::size_t threadsNum = 1);

    ~Worker() final;

    Worker(const Worker&) = delete;
    Worker&
    operator=(const Worker&)
        = delete;

    Worker(Worker&&) noexcept = delete;
    Worker&
    operator=(Worker&&) noexcept
        = delete;

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
    std::size_t _threadsNum;
    std::vector<std::jthread> _threads;
    io::io_context _context;
};

} // namespace jar