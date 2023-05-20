#pragma once

#include "jarvis/CommonExport.hpp"

#include <string_view>
#include <expected>
#include <system_error>

namespace jar {

/**
 * Parse date and time in ISO8601 format (UTC)
 * Example: 2023-05-20T14:34:50Z
 */
JARC_EXPORT std::expected<int64_t, std::error_code>
parseUtcDateTime(std::string_view input);

/**
 * Format date and time in ISO8601 format (UTC)
 * @param input the date and time in UTC
 * @return the formatted date time string
 */
JARC_EXPORT std::string
formatUtcDateTime(int64_t input);

} // namespace jar
