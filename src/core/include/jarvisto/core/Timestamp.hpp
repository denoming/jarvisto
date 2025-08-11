#pragma once

#include "jarvisto/core/Export.hpp"

#include <date/date.h>
#include <date/tz.h>

namespace jar {

/**
 * UTC timestamp
 *
 * Contains seconds since January 1, 1970 at midnight.
 */
struct JARVISTO_CORE_EXPORT Timestamp {
    Timestamp() = default;

    explicit Timestamp(int64_t v);

    explicit Timestamp(date::utc_seconds v);

    explicit Timestamp(date::sys_seconds v);

    explicit Timestamp(date::local_seconds v);

    auto
    operator<=>(const Timestamp& other) const
        = default;

    explicit
    operator int64_t() const;

    explicit
    operator date::utc_seconds() const;

    static Timestamp
    zero();

    static Timestamp
    now();

private:
    int64_t _value{};
};

} // namespace jar