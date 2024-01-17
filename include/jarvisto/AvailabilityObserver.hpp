#pragma once

#include "jarvisto/AvailabilitySubject.hpp"
#include "jarvisto/Types.hpp"
#include "jarvisto/Export.hpp"

#include <sigc++/trackable.h>

#include <string>
#include <unordered_map>

namespace jar {

class JARVISTO_EXPORT AvailabilityObserver : public AvailabilitySubject, public sigc::trackable {
public:
    explicit AvailabilityObserver(std::string name);

    void
    add(AvailabilitySubject& subject);

    void
    remove(AvailabilitySubject& subject);

private:
    void
    updateAvailability();

private:
    std::unordered_map<std::string, AvailabilityState> _states;
};

} // namespace jar