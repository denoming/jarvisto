#pragma once

#include "jarvisto/Types.hpp"
#include "jarvisto/Export.hpp"
#include "jarvisto/MqttClient.hpp"
#include "jarvisto/IAvailabilitySubject.hpp"

#include <sigc++/trackable.h>

#include <string>

namespace jar {

class JARVISTO_EXPORT AvailabilityPublisher : public sigc::trackable {
public:
    explicit AvailabilityPublisher(std::string name, MqttClient& client, IAvailabilitySubject& subject);

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