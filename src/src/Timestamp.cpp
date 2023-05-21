#include "jarvis/Timestamp.hpp"

namespace krn = std::chrono;

namespace jar {

Timestamp::Timestamp(const int64_t v)
    : _value{v}
{
}

Timestamp::Timestamp(date::utc_seconds v)
    : _value{v.time_since_epoch().count()}
{
}

Timestamp::Timestamp(date::sys_seconds v)
    : Timestamp{date::clock_cast<date::utc_clock>(v)}
{
}

Timestamp::Timestamp(date::local_seconds v)
    : Timestamp{date::clock_cast<date::utc_clock>(v)}
{
}

Timestamp::operator int64_t() const
{
    return _value;
}

Timestamp::operator date::utc_seconds() const
{
    return date::utc_seconds{std::chrono::seconds{_value}};
}

Timestamp
Timestamp::zero()
{
    return Timestamp{};
}

Timestamp
Timestamp::now()
{
    const auto now = date::utc_clock::now();
    return Timestamp{krn::time_point_cast<krn::seconds>(now)};
}

} // namespace jar