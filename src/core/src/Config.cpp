#include "jarvisto/core/Config.hpp"

#include <nlohmann/json.hpp>

#include <filesystem>
#include <fstream>

#include <unistd.h>

namespace fs = std::filesystem;

static auto kDefaultPathSuffix{"config"};

namespace jar {

namespace env {

std::optional<std::string>
getEnv(const std::string& name)
{
    if (const auto* env = std::getenv(name.data()); env != nullptr) {
        return env;
    }
    return std::nullopt;
}

std::optional<fs::path>
getExecPath()
{
    static constexpr int kMaxPath{4096};
    char result[kMaxPath];
    const ssize_t count = readlink("/proc/self/exe", result, kMaxPath);
    if (count == -1) {
        return std::nullopt;
    }
    return fs::path{result, result + count};
}

fs::path
getSystemRootPath()
{
    return fs::current_path().root_path();
}

std::string
getConfigEnv()
{
    if (const auto configEnv = getEnv("JAR_ENV"); configEnv) {
        return *configEnv;
    }
    return "development";
}

std::optional<fs::path>
getConfigDir()
{
    const auto configDir = getEnv("JAR_CONFIG_DIR");

    fs::path configDirPath;
    if (configDir) {
        configDirPath = *configDir;
    }

    if (not configDirPath.empty()) {
        if (not configDirPath.is_relative()) {
            if (not fs::exists(configDirPath)) {
                throw std::runtime_error{"Config directory not exists"};
            }
            return configDirPath;
        }
    }

    std::string pathSuffix{kDefaultPathSuffix};
    if (not configDirPath.empty()) {
        pathSuffix = configDirPath.lexically_normal().string();
    }

    const auto rootPath = getSystemRootPath();
    const auto execPath = getExecPath();
    if (not execPath) {
        throw std::runtime_error{"Unable to get executable path"};
    }

    fs::path path{*execPath};
    while (path.has_parent_path() and not fs::equivalent(path, rootPath)) {
        if (auto candidate = path / pathSuffix;
            fs::exists(candidate) and fs::is_directory(candidate)) {
            return candidate;
        }
        path = path.parent_path();
    }
    if (path.string().ends_with(pathSuffix)) {
        return path;
    }

    return std::nullopt;
}

std::vector<fs::path>
getAllFiles(const fs::path& path)
{
    std::vector<std::filesystem::path> files;

    for (const auto& entry : std::filesystem::directory_iterator{path}) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path());
        }
    }
    if (files.empty()) {
        /* Directory is empty */
        return {};
    }

    const std::string configEnv = getConfigEnv();
    std::vector<std::string> order = {"default", configEnv, "local", "local-" + configEnv};
    auto comparator = [&order](const fs::path& path1, const fs::path& path2) {
        const auto fileName1 = path1.stem().string();
        const auto fileName2 = path2.stem().string();
        const auto it1 = std::ranges::find(order, fileName1);
        const auto it2 = std::ranges::find(order, fileName2);
        if (it1 == std::end(order) and it2 == std::end(order)) {
            /* If both are not in the order list -> order them alphabetically */
            return path1 < path2;
        }
        if (it1 == std::end(order)) {
            /* If only path1 is not in the order list -> it comes after path2 */
            return false;
        }
        if (it2 == std::end(order)) {
            /* If only path2 is not in the order list -> it comes after path1 */
            return true;
        }
        /* Order path1 and path2 by the position in the order list */
        return std::distance(order.begin(), it1) < std::distance(order.begin(), it2);
    };

    /* Sort the list of the files */
    std::ranges::sort(files, comparator);

    return files;
}

} // namespace env

namespace json {

std::string
normalizeConfigKey(const std::string& str)
{
    auto result = str.substr(1);
    std::ranges::replace(result, '/', '.');
    return result;
}

ConfigValue
normalizeConfigValue(const nlohmann::json& object)
{
    ConfigValue normalizedValue;
    if (object.is_null()) {
        normalizedValue = nullptr;
    } else if (object.is_boolean()) {
        normalizedValue = object.get<bool>();
    } else if (object.is_number_integer()) {
        normalizedValue = object.get<std::int64_t>();
    } else if (object.is_number_unsigned()) {
        normalizedValue = object.get<std::uint64_t>();
    } else if (object.is_number_float()) {
        normalizedValue = object.get<double>();
    } else if (object.is_string()) {
        normalizedValue = object.get<std::string>();
    } else {
        throw std::runtime_error{"Unsupported config value type"};
    }
    return normalizedValue;
}

void
loadConfigFile(const fs::path& path, ConfigValues& values)
{
    try {
        if (std::ifstream is{path}; is.is_open()) {
            const auto config = nlohmann::json::parse(is).flatten();
            for (auto it = config.begin(); it != config.end(); ++it) {
                values[normalizeConfigKey(it.key())] = normalizeConfigValue(it.value());
            }
        }
    } catch (const std::exception& e) {
        throw std::runtime_error{"Unable to parse file: " + std::string{e.what()}};
    }
}

} // namespace json

void
Config::load()
{
    const auto configDir = env::getConfigDir();
    if (not configDir) {
        return;
    }

    for (const auto& file : env::getAllFiles(*configDir)) {
        if (file.extension() == ".json") {
            json::loadConfigFile(file, _values);
        }
    }
}

template<typename T>
T
Config::get(const std::string& key) const
{
    if (const auto it = _values.find(key); it != std::cend(_values)) {
        const ConfigValue& value = std::get<1>(*it);
        if (not std::holds_alternative<T>(value)) {
            throw std::runtime_error{"Config key " + key + " has value of wrong type"};
        }
        return std::get<T>(value);
    }
    throw std::runtime_error{"Config key " + key + " not found"};
}

const ConfigValue&
Config::get(const std::string& key) const
{
    if (const auto it = _values.find(key); it != std::cend(_values)) {
        return std::get<1>(*it);
    }
    throw std::runtime_error{"Config key " + key + " not found"};
}

template<typename T>
std::vector<T>
Config::getMany(const std::string& prefix) const
{
    std::vector<T> output;
    for (const auto& [k, v] : _values) {
        if (k.starts_with(prefix)) {
            if (not std::holds_alternative<T>(v)) {
                throw std::runtime_error{"Config key " + k + " has value of wrong type"};
            }
            output.push_back(std::get<T>(v));
        }
    }
    return output;
}

std::vector<ConfigValue>
Config::getMany(const std::string& prefix) const
{
    std::vector<ConfigValue> output;
    for (const auto& [k, v] : _values) {
        if (k.starts_with(prefix)) {
            output.push_back(v);
        }
    }
    return output;
}

template<typename T>
std::optional<T>
Config::pick(const std::string& key) const
{
    if (const auto it = _values.find(key); it != std::cend(_values)) {
        if (const ConfigValue& value = std::get<1>(*it); std::holds_alternative<T>(value)) {
            return std::get<T>(value);
        }
    }
    return std::nullopt;
}

bool
Config::has(const std::string& key) const
{
    return _values.contains(key);
}

} // namespace jar

/* Explicit template specializations */
namespace jar {

template std::nullptr_t
Config::get<std::nullptr_t>(const std::string&) const;
template bool
Config::get<bool>(const std::string&) const;
template std::int64_t
Config::get<std::int64_t>(const std::string&) const;
template std::uint64_t
Config::get<std::uint64_t>(const std::string&) const;
template double
Config::get<double>(const std::string&) const;
template std::string
Config::get<std::string>(const std::string&) const;

template std::vector<std::nullptr_t>
Config::getMany<std::nullptr_t>(const std::string&) const;
template std::vector<bool>
Config::getMany<bool>(const std::string&) const;
template std::vector<std::int64_t>
Config::getMany<std::int64_t>(const std::string&) const;
template std::vector<std::uint64_t>
Config::getMany<std::uint64_t>(const std::string&) const;
template std::vector<double>
Config::getMany<double>(const std::string&) const;
template std::vector<std::string>
Config::getMany<std::string>(const std::string&) const;

template std::optional<std::nullptr_t>
Config::pick<std::nullptr_t>(const std::string&) const;
template std::optional<bool>
Config::pick<bool>(const std::string&) const;
template std::optional<std::int64_t>
Config::pick<std::int64_t>(const std::string&) const;
template std::optional<std::uint64_t>
Config::pick<std::uint64_t>(const std::string&) const;
template std::optional<double>
Config::pick<double>(const std::string&) const;
template std::optional<std::string>
Config::pick<std::string>(const std::string&) const;

} // namespace jar