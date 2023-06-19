#pragma once

#include <compare>

namespace jar {

struct GeoLocation {
    auto
    operator<=>(const GeoLocation& other) const
        = default;

    double lat{};
    double lon{};
    double alt{};
};

} // namespace jar