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

#include "jarvisto/core/SignalHandler.hpp"

#include <stdexcept>

#include <cstring>

namespace jar {

SignalHandler::SignalHandler(const std::initializer_list<int> signals)
    : _set{}
{
    if (sigemptyset(&_set) != 0) {
        throw std::runtime_error("Unable to initialize signal set");
    }
    for (const int signal : signals) {
        if (sigaddset(&_set, signal) != 0) {
            throw std::runtime_error("Unable to add signal into set: " + std::to_string(signal));
        }
    }
    if (const int retVal = pthread_sigmask(SIG_BLOCK, &_set, nullptr); retVal != 0) {
        throw std::runtime_error(std::string{"Unable to block signal set: "}
                                 + std::strerror(retVal));
    }
}

SignalHandler::~SignalHandler()
{
    std::ignore = pthread_sigmask(SIG_UNBLOCK, &_set, nullptr);
}

int
SignalHandler::wait() const
{
    int signum{};
    if (const int retVal = sigwait(&_set, &signum); retVal != 0) {
        throw std::runtime_error(std::string{"Unable to wait signal: "} + std::strerror(retVal));
    }
    return signum;
}

int
SignalHandler::waitFor(std::move_only_function<bool(int signal)> handler) const
{
    while (true) {
        if (const int signum = wait(); handler(signum)) {
            return signum;
        }
    }
}

} // namespace jar