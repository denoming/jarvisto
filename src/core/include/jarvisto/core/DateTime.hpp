#pragma once

#include "jarvisto/core/Export.hpp"
#include "jarvisto/core/Timestamp.hpp"

#include <string_view>
#include <expected>
#include <system_error>

namespace jar {

/**
 * Parses date and time  (UTC)
 * @param input the date and time (ex. 2023-05-20T14:34:50Z)
 * @return the UTC timestamp
 */
JARVISTO_CORE_EXPORT std::expected<Timestamp, std::error_code>
parseUtcDateTime(std::string_view input);

/**
 * Formats timestamp to string in ISO8601 format (UTC)
 * @param input the timestamp
 * @return the formatted string with date and time
 */
JARVISTO_CORE_EXPORT std::string
formatUtcDateTime(Timestamp input);

/**
 * Parses date and time in ISO8601 format with offset (not UTC)
 * @param input the zoned date time (with offset)
 * @return the UTC timestamp
 */
JARVISTO_CORE_EXPORT std::expected<Timestamp, std::error_code>
parseZonedDateTime(std::string_view input,
                   std::string* abbrev = nullptr,
                   std::chrono::minutes* offset = nullptr);

/**
 * Formats timestamp to string in ISO8601 format with given offset
 * @param input the timestamp
 * @return the formatted string with date and time
 */
JARVISTO_CORE_EXPORT std::string
formatZonedDateTime(Timestamp input, std::chrono::minutes* offset = nullptr);

} // namespace jar
