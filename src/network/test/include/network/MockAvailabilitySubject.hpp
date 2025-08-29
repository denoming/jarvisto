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