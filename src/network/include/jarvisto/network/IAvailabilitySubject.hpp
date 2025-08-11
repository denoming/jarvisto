#pragma once

#include "jarvisto/network/Types.hpp"

#include <sigc++/signal.h>

#include <string>

namespace jar {

class IAvailabilitySubject {
public:
    /* Signatures */
    using OnStateUpdate = void(const std::string& name, AvailabilityState state);
    /* Signals */
    using OnStateUpdateSignal = sigc::signal<OnStateUpdate>;

    virtual ~IAvailabilitySubject() = default;

    [[nodiscard]] virtual const std::string&
    name() const
        = 0;

    [[nodiscard]] virtual AvailabilityState
    state() const
        = 0;

    [[nodiscard]] virtual OnStateUpdateSignal
    onStateUpdate() const
        = 0;
};

} // namespace jar