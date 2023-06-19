#pragma once

#include "jarvisto/Types.hpp"

#include <fmt/core.h>

template<>
struct fmt::formatter<jar::GeoLocation> {
    constexpr auto
    parse(format_parse_context& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto
    format(const jar::GeoLocation& loc, FormatContext& c) const
    {
        constexpr const std::string_view kFormat{"(lat: <{:.3f}>, lon: <{:.3f}>, alt: <{:.3f}>)"};
        return fmt::format_to(c.out(), kFormat, loc.lat, loc.lon, loc.alt);
    }
};