#pragma once

#include "jarvis/CommonExport.hpp"

#include <date/date.h>
#include <date/tz.h>

#include <string_view>
#include <expected>
#include <system_error>

namespace jar {

/* UTC timestamp in seconds */
using UtcTimestamp = date::utc_seconds;

/**
 * Parse date and time in ISO8601 format (UTC)
 * Example: 2023-05-20T14:34:50Z
 */
JARC_EXPORT std::expected<UtcTimestamp, std::error_code>
parseUtcDateTime(std::string_view input);

/**
 *
 * @param input
 * @return
 */
JARC_EXPORT std::expected<uint64_t, std::error_code>
parseUtcDateTimeRaw(std::string_view input);

/**
 * Format date and time in ISO8601 format (UTC)
 * @param input the date and time in UTC
 * @return the formatted date time string
 */
JARC_EXPORT std::string
formatUtcDateTime(UtcTimestamp input);

/**
 *
 * @param input
 * @return
 */
JARC_EXPORT std::string
formatUtcDateTimeRaw(uint16_t input);

} // namespace jar
