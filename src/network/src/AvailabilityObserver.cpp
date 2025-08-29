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

#include "jarvisto/network/AvailabilityObserver.hpp"

#include <sigc++/adaptors/adaptors.h>

namespace jar {

AvailabilityObserver::AvailabilityObserver(std::string name)
    : AvailabilitySubject{std::move(name)}
{
}

void
AvailabilityObserver::add(AvailabilitySubject& subject)
{
    const auto [entry, ok] = _states.insert({subject.name(), subject.state()});

    std::ignore = subject.onStateUpdate().connect(sigc::track_obj(
        [entry, this](const std::string& name, const AvailabilityState state) {
            std::get<AvailabilityState>(*entry) = state;
            if (state == AvailabilityState::Online) {
                updateAvailability();
            } else {
                availability(AvailabilityState::Offline);
            }
        },
        *this));

    updateAvailability();
}

void
AvailabilityObserver::remove(AvailabilitySubject& subject)
{
    if (_states.erase(subject.name()) > 0) {
        updateAvailability();
    }
}

void
AvailabilityObserver::updateAvailability()
{
    if (_states.empty()) {
        availability(AvailabilityState::Offline);
    } else {
        auto ownStatus = AvailabilityState::Online;
        for (const auto& [name, status] : _states) {
            if (status != AvailabilityState::Online) {
                ownStatus = AvailabilityState::Offline;
                break;
            }
        }
        availability(ownStatus);
    }
}

} // namespace jar