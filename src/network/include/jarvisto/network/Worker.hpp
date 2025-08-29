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

#pragma once

#include "jarvisto/network/Export.hpp"
#include "jarvisto/network/IWorker.hpp"
#include "jarvisto/network/Http.hpp"

#include <thread>
#include <vector>

namespace jar {

class JARVISTO_NETWORK_EXPORT Worker final : public IWorker {
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