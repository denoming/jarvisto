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

#include <vector>
#include <memory>

namespace jar {

class JARVISTO_NETWORK_EXPORT WorkerGroup final : public IWorker {
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