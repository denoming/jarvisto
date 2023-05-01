#include "jarvis/Utils.hpp"

#include <cstdlib>
#include <chrono>
#include <iomanip>

namespace krn = std::chrono;

namespace jar {

std::optional<std::string>
getEnvVar(std::string_view name)
{
    const auto* const value = std::getenv(name.data());
    return (value == nullptr) ? std::nullopt : std::make_optional<std::string>(value);
}

std::expected<int64_t, std::error_code>
parseDateTime(std::string_view input)
{
    static const char* kFormat{"%Y-%m-%dT%H:%M:%S"};
    std::tm t = {};
    std::istringstream ss{std::string{input}};
    ss >> std::get_time(&t, kFormat);
    if (ss.fail()) {
        return std::unexpected(std::make_error_code(std::errc::invalid_argument));
    } else {
        const auto timePoint = krn::system_clock::from_time_t(std::mktime(&t));
        return krn::duration_cast<krn::seconds>(timePoint.time_since_epoch()).count();
    }
}

} // namespace jar