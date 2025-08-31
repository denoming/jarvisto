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

#include <optional>
#include <string>
#include <vector>
#include <unordered_map>
#include <variant>

namespace jar {

// clang-format off
using ConfigValue = std::variant<
    std::nullptr_t,
    bool,
    int64_t,
    u_int64_t,
    double,
    std::string
>;
// clang-format on
using ConfigValues = std::unordered_map<std::string, ConfigValue>;

class JARVISTO_CORE_EXPORT Config {
public:
    Config() = default;

    void
    load();

    template<typename T>
    [[nodiscard]] T
    get(const std::string& key) const;

    [[nodiscard]] const ConfigValue&
    get(const std::string& key) const;

    template<typename T>
    [[nodiscard]] std::vector<T>
    getMany(const std::string& prefix) const;

    [[nodiscard]] std::vector<ConfigValue>
    getMany(const std::string& prefix) const;

    template<typename T>
    [[nodiscard]] std::optional<T>
    pick(const std::string& key) const;

    [[nodiscard]] bool
    has(const std::string& key) const;

private:
    ConfigValues _values;
};

} // namespace jar