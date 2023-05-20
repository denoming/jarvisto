#include "jarvis/DateTime.hpp"

#include <date/date.h>
#include <date/tz.h>

#include <sstream>

namespace jar {

UtcTimestamp
getUtcTimestamp()
{
    const auto now = date::utc_clock::now();
    return std::chrono::time_point_cast<std::chrono::seconds>(now);
}

std::expected<UtcTimestamp, std::error_code>
parseUtcDateTime(std::string_view input)
{
    UtcTimestamp value;
    std::istringstream ss{std::string{input}};
    if (date::from_stream(ss, "%Y-%m-%dT%H:%M:%6SZ", value); ss.fail()) {
        return std::unexpected(std::make_error_code(std::errc::invalid_argument));
    } else {
        return value;
    }
}

std::expected<uint64_t, std::error_code>
parseUtcDateTimeRaw(std::string_view input)
{
    if (const auto result = parseUtcDateTime(input); result) {
        return result.value().time_since_epoch().count();
    } else {
        return std::unexpected(result.error());
    }
}

std::string
formatUtcDateTime(UtcTimestamp input)
{
    std::ostringstream ss;
    date::to_stream(ss, "%FT%TZ", input);
    return ss.str();
}

std::string
formatUtcDateTimeRaw(uint16_t input)
{
    UtcTimestamp ts{std::chrono::seconds{input}};
    return formatUtcDateTime(ts);
}

} // namespace jar