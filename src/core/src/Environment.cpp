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

#include "jarvisto/core/Environment.hpp"

#include <cstdlib>

namespace jar {

std::optional<std::string>
getEnvVar(std::string_view name)
{
    const auto* const value = std::getenv(name.data());
    return (value == nullptr) ? std::nullopt : std::make_optional<std::string>(value);
}

} // namespace jar