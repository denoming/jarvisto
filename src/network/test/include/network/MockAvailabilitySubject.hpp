#pragma once

#include "jarvisto/network/IAvailabilitySubject.hpp"

#include <gmock/gmock.h>

namespace jar {

class MockAvailabilitySubject : public IAvailabilitySubject {
public:
    MockAvailabilitySubject();

    void
    triggerStateUpdate(const std::string& name, AvailabilityState state);

    OnStateUpdateSignal
    onStateUpdate() const override;

public:
    MOCK_METHOD(const std::string&, name, (), (const, override));

    MOCK_METHOD(AvailabilityState, state, (), (const, override));

private:
    OnStateUpdateSignal _onStateUpdateSig;
};

} // namespace jar