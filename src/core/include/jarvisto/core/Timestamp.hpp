// Copyright 2025 Denys Asauliak
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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