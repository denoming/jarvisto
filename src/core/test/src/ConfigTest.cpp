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

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "jarvisto/core/Config.hpp"

#include <unordered_map>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

using namespace jar;
using namespace testing;

constexpr std::string_view kDefaultJson{R"({
    "db": {
        "port": 1996,
        "timeout": 1000
    }
})"};

constexpr std::string_view kValidJson{R"({
    "db": {
        "port": 2001
    },
    "auth": {
        "enabled": true,
        "roles": [
            "admin",
            "user"
        ]
    }
})"};

constexpr std::string_view kInvalidJson{R"({
    "db": {
        "port": 2001
    },
    "auth": {
        "enabled": true,
        "roles": [
            "admin",
            "user"
        ]
})"};

class ConfigTest : public Test {
public:
    const fs::path kTestDir{fs::current_path() / "test"};

    void
    SetUp() override
    {
        fs::remove_all(kTestDir);
        fs::create_directories(kTestDir);

        createFile(kTestDir / "default.json", kDefaultJson);
    }

    void
    TearDown() override
    {
    }

    void
    createFile(const fs::path& path, std::string_view content) const
    {
        if (std::ofstream file{kTestDir / path}; file.is_open()) {
            file << content;
        }
    }
};

TEST_F(ConfigTest, LoadValidConfig)
{
    createFile(kTestDir / "config.json", kValidJson);

    setenv("JAR_ENV", "test", 1);
    setenv("JAR_CONFIG_DIR", "test", 1);

    Config config;
    ASSERT_NO_THROW({ config.load(); });

    ConfigValues values{
        {"db.port", ConfigValue{2001}},
        {"db.timeout", ConfigValue{1000}},
        {"auth.enabled", ConfigValue{true}},
        {"auth.roles.0", ConfigValue{"admin"}},
        {"auth.roles.1", ConfigValue{"user"}},
    };

    for (const auto& [key, value] : values) {
        ASSERT_TRUE(config.has(key));
        EXPECT_EQ(config.get(key), value);
    }

    EXPECT_EQ(config.get<int64_t>("db.port"), std::get<int64_t>(values["db.port"]));
    EXPECT_EQ(config.get<bool>("auth.enabled"), std::get<bool>(values["auth.enabled"]));
    EXPECT_EQ(config.get<std::string>("auth.roles.0"),
              std::get<std::string>(values["auth.roles.0"]));
    EXPECT_EQ(config.get<std::string>("auth.roles.1"),
              std::get<std::string>(values["auth.roles.1"]));
    EXPECT_THAT(config.getMany<std::string>("auth.roles"), UnorderedElementsAre("admin", "user"));
}

TEST_F(ConfigTest, LoadInvalidConfig)
{
    createFile(kTestDir / "config.json", kInvalidJson);

    Config config;
    EXPECT_ANY_THROW({ config.load(); });
}
