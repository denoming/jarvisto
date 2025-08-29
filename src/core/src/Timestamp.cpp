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

#include "jarvisto/core/Timestamp.hpp"

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