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

#include "jarvisto/core/Export.hpp"

#include <initializer_list>
#include <functional>
#include <future>

#include <csignal>

namespace jar {

class JARVISTO_CORE_EXPORT SignalHandler {
public:
    SignalHandler(std::initializer_list<int> signals);

    ~SignalHandler();

    SignalHandler(SignalHandler&& other) = delete;
    SignalHandler&
    operator=(SignalHandler&& other)
        = delete;

    [[nodiscard]] int
    wait() const;

    [[nodiscard]] int
    waitFor(std::move_only_function<bool(int signal)> predicate) const;

    auto
    asyncWaitFor(std::move_only_function<bool(int signal)> handler)
    {
        return std::async(std::launch::async, &SignalHandler::waitFor, this, std::move(handler));
    }

private:
    sigset_t _set;
};

} // namespace jar
