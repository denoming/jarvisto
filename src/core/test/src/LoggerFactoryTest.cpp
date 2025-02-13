#include <gtest/gtest.h>

#include "jarvisto/core/LoggerFactory.hpp"

#include <filesystem>

namespace fs = std::filesystem;

class TestLoggerFactory final : public jar::LoggerFactory {
private:
    void
    addSinks(const std::shared_ptr<spdlog::logger> logger, const int logLevel) override
    {
        addConsoleSink(logger, logLevel);
        addFileSink(logger, fs::current_path() / "log.txt");
    }
};

TEST(LoggerCreatorTest, WithFileSink)
{
    TestLoggerFactory factory;
    const auto logger = factory.create("TEST", SPDLOG_LEVEL_DEBUG);
    logger->info("Test Logger");

    EXPECT_TRUE(fs::exists(fs::current_path() / "log.txt"));
}