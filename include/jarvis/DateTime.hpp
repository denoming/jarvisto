#pragma once

#include "jarvis/CommonExport.hpp"

#include <date/date.h>
#include <date/tz.h>

#include <string_view>
#include <expected>
#include <system_error>

namespace jar {

/* UTC timestamp (seconds from January 1, 1970 at midnight) */
using UtcTimestamp = date::utc_seconds;

/**
 * Returns UTC timestamp
 * @return the UTC timestamp
 */
JARC_EXPORT UtcTimestamp
getUtcTimestamp();

/**
 * Parses date and time in ISO8601 format (UTC)
 * @param input the date and time (ex. 2023-05-20T14:34:50Z)
 * @return
 */
JARC_EXPORT std::expected<UtcTimestamp, std::error_code>
parseUtcDateTime(std::string_view input);

/**
 * Parses date and time in ISO8601 format (UTC)
 * @param input the date and time (ex. 2023-05-20T14:34:50Z)
 * @return the timestamp in seconds from epoch
 */
JARC_EXPORT std::expected<int64_t, std::error_code>
parseUtcDateTimeRaw(std::string_view input);

/**
 * Format date and time to ISO8601 format (UTC)
 * @param input the date and time in UTC
 * @return the formatted date time string
 */
JARC_EXPORT std::string
formatUtcDateTime(UtcTimestamp input);

/**
 * Formats date and time to ISO8601 format (UTC)
 * @param input the date and time in seconds from epoch
 * @return
 */
JARC_EXPORT std::string
formatUtcDateTimeRaw(int64_t input);

} // namespace jar
