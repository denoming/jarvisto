// Copyright 2025 Denys Asauliak
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "jarvisto/network/Worker.hpp"

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