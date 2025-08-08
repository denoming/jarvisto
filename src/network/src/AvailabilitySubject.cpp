#include "jarvisto/network/AvailabilitySubject.hpp"

namespace jar {

AvailabilitySubject::AvailabilitySubject(std::string name)
    : _name{std::move(name)}
{
}

const std::string&
AvailabilitySubject::name() const
{
    return _name;
}

AvailabilityState
AvailabilitySubject::state() const
{
    return _state;
}

AvailabilitySubject::OnStateUpdateSignal
AvailabilitySubject::onStateUpdate() const
{
    return _signal;
}

void
AvailabilitySubject::availability(AvailabilityState state)
{
    if (_state != state) {
        _signal(_name, _state = state);
    }
}

} // namespace jar