#include "jarvisto/DateTime.hpp"

#include <date/tz.h>

#include <sstream>

namespace krn = std::chrono;

namespace jar {

std::expected<Timestamp, std::error_code>
parseUtcDateTime(std::string_view input)
{
    date::utc_seconds value;
    std::istringstream ss{std::string{input}};
    if (date::from_stream(ss, "%Y-%m-%dT%H:%M:%6SZ", value); ss.fail()) {
        return std::unexpected(std::make_error_code(std::errc::invalid_argument));
    } else {
        return Timestamp{value};
    }
}

std::string
formatUtcDateTime(Timestamp input)
{
    std::ostringstream ss;
    date::to_stream(ss, "%FT%TZ", date::utc_seconds{input});
    return ss.str();
}

std::expected<Timestamp, std::error_code>
parseZonedDateTime(std::string_view input, std::string* abbrev, std::chrono::minutes* offset)
{
    date::utc_seconds value;
    std::istringstream ss{std::string{input}};
    if (date::from_stream(ss, "%Y-%m-%dT%H:%M:%6S%z", value, abbrev, offset); ss.fail()) {
        return std::unexpected(std::make_error_code(std::errc::invalid_argument));
    } else {
        return Timestamp{value};
    }
}

std::string
formatZonedDateTime(Timestamp input, std::chrono::minutes* offset)
{
    krn::seconds d;
    if (offset) {
        d = krn::duration_cast<krn::seconds>(*offset);
    }
    date::local_seconds ls = clock_cast<date::local_t>(date::utc_seconds{input}) + d;
    std::stringstream ss;
    to_stream(ss, "%FT%T%z", ls, nullptr, &d);
    return ss.str();
}

} // namespace jar