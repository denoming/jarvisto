#pragma once

#include "jarvis/Export.hpp"

#include <optional>
#include <string>

namespace jar {

JARC_EXPORT std::optional<std::string>
getEnvVar(std::string_view name);

} // namespace jar