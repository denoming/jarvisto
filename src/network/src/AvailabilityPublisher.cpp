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

#include "jarvisto/network/AvailabilityPublisher.hpp"

#include "jarvisto/network/AvailabilityObserver.hpp"
#include "jarvisto/network/Formatters.hpp"
#include "jarvisto/core/Logger.hpp"

#include <fmt/format.h>
#include <sigc++/adaptors/track_obj.h>

namespace jar {

AvailabilityPublisher::AvailabilityPublisher(std::string name,
                                             MqttClient& client,
                                             IAvailabilitySubject& subject)
    : _name{std::move(name)}
    , _client{client}
    , _state{subject.state()}
{

    std::ignore = subject.onStateUpdate().connect(sigc::track_obj(
        [this](const std::string& n, AvailabilityState s) { onStateUpdate(n, s); }, this));
    std::ignore = _client.onConnect().connect(sigc::track_obj(
        [this](const MqttReturnCode c) { onPublisherConnect(c == MqttReturnCode::Accepted); },
        this));

    if (_client.hasConnection()) {
        publish();
    }
}

void
AvailabilityPublisher::publish()
{
    static const std::string kPublishTopic = fmt::format("{}/state", _name);
    if (auto rv = _client.publish(kPublishTopic, fmt::to_string(_state)); not rv) {
        LOGE("Unable to publish <{}> state: {}", _state, rv.error().message());
    }
}

void
AvailabilityPublisher::onStateUpdate(const std::string& name, const AvailabilityState state)
{
    LOGD("State of <{}> subject update: old<{}>, new<{}>", name, _state, state);

    _state = state;

    if (_client.hasConnection()) {
        publish();
    }
}

void
AvailabilityPublisher::onPublisherConnect(const bool hasConnection)
{
    LOGD("Connection state update: hasConnection<{}>", hasConnection);

    if (hasConnection) {
        publish();
    }
}

} // namespace jar