#include "jarvisto/AvailabilityPublisher.hpp"

#include "jarvisto/AvailabilityObserver.hpp"
#include "jarvisto/Formatters.hpp"
#include "jarvisto/Logger.hpp"

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

    std::ignore = subject.onStateUpdate(
        sigc::track_obj([this](auto&& name, auto&& state) { onStateUpdate(name, state); }, this));

    std::ignore = _client.onConnect(sigc::track_obj(
        [this](const MqttReturnCode code) { onPublisherConnect(code == MqttReturnCode::Accepted); },
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