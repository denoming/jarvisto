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