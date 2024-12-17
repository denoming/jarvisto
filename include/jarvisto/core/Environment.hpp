#pragma once

#include "jarvisto/core/Export.hpp"

#include <optional>
#include <string>

namespace jar {

JARVISTO_CORE_EXPORT std::optional<std::string>
getEnvVar(std::string_view name);

} // namespace jar