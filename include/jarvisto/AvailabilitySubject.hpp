#pragma once

#include "jarvisto/Export.hpp"
#include "jarvisto/IAvailabilitySubject.hpp"

namespace jar {

class JARVISTO_EXPORT AvailabilitySubject : public IAvailabilitySubject {
public:
    explicit AvailabilitySubject(std::string name);

    [[nodiscard]] const std::string&
    name() const override;

    [[nodiscard]] AvailabilityState
    state() const override;

    [[maybe_unused]] sigc::connection
    onStateUpdate(OnStateUpdateSignal::slot_type&& slot) override;

protected:
    void
    availability(AvailabilityState status);

private:
    std::string _name;
    AvailabilityState _state{AvailabilityState::Offline};
    OnStateUpdateSignal _signal;
};

} // namespace jar