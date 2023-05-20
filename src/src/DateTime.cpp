#include "jarvis/DateTime.hpp"

#include <date/tz.h>

#include <sstream>

namespace jar {

std::expected<int64_t, std::error_code>
parseUtcDateTime(std::string_view input)
{
    date::utc_seconds value;
    std::istringstream ss{std::string{input}};
    if (date::from_stream(ss, "%Y-%m-%dT%H:%M:%6SZ", value); ss.fail()) {
        return std::unexpected(std::make_error_code(std::errc::invalid_argument));
    } else {
        return value.time_since_epoch().count();
    }
}

std::string
formatUtcDateTime(int64_t input)
{
    date::utc_seconds seconds{std::chrono::seconds{input}};
    std::ostringstream ss;
    date::to_stream(ss, "%FT%TZ", seconds);
    return ss.str();
}

} // namespace jar