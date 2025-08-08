#pragma once

#include "jarvisto/network/Export.hpp"
#include "jarvisto/network/IAvailabilitySubject.hpp"

namespace jar {

class JARVISTO_NETWORK_EXPORT AvailabilitySubject : public IAvailabilitySubject {
public:
    explicit AvailabilitySubject(std::string name);

    [[nodiscard]] const std::string&
    name() const override;

    [[nodiscard]] AvailabilityState
    state() const override;

    [[nodiscard]] OnStateUpdateSignal
    onStateUpdate() const override;

protected:
    void
    availability(AvailabilityState status);

private:
    std::string _name;
    AvailabilityState _state{AvailabilityState::Offline};
    OnStateUpdateSignal _signal;
};

} // namespace jar