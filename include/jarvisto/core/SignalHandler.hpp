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
