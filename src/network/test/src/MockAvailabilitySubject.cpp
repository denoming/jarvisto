#include "network/MockAvailabilitySubject.hpp"

using namespace testing;

namespace jar {

MockAvailabilitySubject::MockAvailabilitySubject()
{
    static const std::string kDefaultName{"MockSubject"};

    ON_CALL(*this, name).WillByDefault(ReturnRef(kDefaultName));

    ON_CALL(*this, state).WillByDefault(Return(AvailabilityState::Offline));
}

void
MockAvailabilitySubject::triggerStateUpdate(const std::string& name, AvailabilityState state)
{
    _onStateUpdateSig(name, state);
}

MockAvailabilitySubject::OnStateUpdateSignal
MockAvailabilitySubject::onStateUpdate() const
{
    return _onStateUpdateSig;
}

} // namespace jar