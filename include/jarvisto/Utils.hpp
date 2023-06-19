#pragma once

#include "jarvisto/Export.hpp"

#include <optional>
#include <string>

namespace jar {

JARVISTO_EXPORT std::optional<std::string>
getEnvVar(std::string_view name);

} // namespace jar