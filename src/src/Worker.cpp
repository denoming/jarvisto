#include "jarvisto/Worker.hpp"

namespace jar {

Worker::Worker(std::size_t threadsNum)
    : _threadsNum{threadsNum}
    , _context{int(threadsNum)}
{
    assert(threadsNum > 0);
}

Worker::~Worker()
{
    stop();
}

bool
Worker::active() const
{
    return !_threads.empty();
}

void
Worker::start()
{
    assert(!active());

    if (_context.stopped()) {
        _context.restart();
    }

    for (std::size_t n{0}; n < _threadsNum; ++n) {
        _threads.emplace_back([this]() {
            auto worker = io::make_work_guard(_context);
            _context.run();
        });
    }
}

void
Worker::stop()
{
    if (active()) {
        _context.stop();
        _threads.clear();
    }
}

io::any_io_executor
Worker::executor()
{
    return _context.get_executor();
}

IWorker&
Worker::current()
{
    return *this;
}

IWorker&
Worker::next()
{
    return *this;
}

} // namespace jar