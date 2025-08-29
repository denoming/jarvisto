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
#include "jarvisto/network/Types.hpp"
#include "jarvisto/network/MqttClient.hpp"
#include "jarvisto/network/IAvailabilitySubject.hpp"

#include <sigc++/trackable.h>

#include <string>

namespace jar {

class JARVISTO_NETWORK_EXPORT AvailabilityPublisher : public sigc::trackable {
public:
    explicit AvailabilityPublisher(std::string name,
                                   MqttClient& client,
                                   IAvailabilitySubject& subject);

private:
    void
    publish();
    void
    onStateUpdate(const std::string& name, AvailabilityState state);
    void
    onPublisherConnect(bool hasConnection);

private:
    std::string _name;
    MqttClient& _client;
    AvailabilityState _state{AvailabilityState::Offline};
};

} // namespace jar