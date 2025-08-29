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