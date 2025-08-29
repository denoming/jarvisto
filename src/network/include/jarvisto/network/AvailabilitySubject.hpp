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