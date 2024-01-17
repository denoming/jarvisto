#pragma once

#include "jarvisto/IAvailabilitySubject.hpp"

#include <gmock/gmock.h>

namespace jar {

class MockAvailabilitySubject : public IAvailabilitySubject {
public:
    MockAvailabilitySubject();

    void
    triggerStateUpdate(const std::string& name, AvailabilityState state);

    sigc::connection
    onStateUpdate(OnStateUpdateSignal::slot_type&& slot) override;

public:
    MOCK_METHOD(const std::string&, name, (), (const, override));

    MOCK_METHOD(AvailabilityState, state, (), (const, override));

private:
    OnStateUpdateSignal _onStateUpdateSig;
};

} // namespace jar