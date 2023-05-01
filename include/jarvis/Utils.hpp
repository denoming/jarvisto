#pragma once

#include "jarvis/CommonExport.hpp"

#include <optional>
#include <string>
#include <expected>
#include <system_error>

namespace jar {

JARC_EXPORT std::optional<std::string>
getEnvVar(std::string_view name);

/**
 * Parse date and time in ISO8601 format
 */
JARC_EXPORT std::expected<int64_t, std::error_code>
parseDateTime(std::string_view input);

} // namespace jar