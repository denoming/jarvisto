#include "jarvisto/network/WorkerGroup.hpp"

#include "jarvisto/network/Worker.hpp"

namespace jar {

WorkerGroup::WorkerGroup(std::size_t workersNum, std::size_t threadsPerWorker)
    : _index{0}
{
    assert(workersNum > 0);
    assert(threadsPerWorker > 0);

    for (std::size_t n{0}; n < workersNum; ++n) {
        _workers.emplace_back(std::make_unique<Worker>(threadsPerWorker));
    }
}

bool
WorkerGroup::active() const
{
    assert(!_workers.empty());
    return _workers[_index]->active();
}

void
WorkerGroup::start()
{
    for (auto& worker : _workers) {
        worker->start();
    }
}

void
WorkerGroup::stop()
{
    for (auto& worker : _workers) {
        worker->stop();
    }
}

io::any_io_executor
WorkerGroup::executor()
{
    assert(!_workers.empty());
    return _workers[_index]->executor();
}

IWorker&
WorkerGroup::current()
{
    assert(!_workers.empty());
    return *_workers[_index];
}

IWorker&
WorkerGroup::next()
{
    assert(!_workers.empty());
    return *_workers[_index++ % _workers.size()];
}

} // namespace jar