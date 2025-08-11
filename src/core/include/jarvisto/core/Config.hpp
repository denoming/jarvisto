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

    [[nodiscard]]
    bool
    has(const std::string& key) const;

private:
    ConfigValues _values;
};

} // namespace jar