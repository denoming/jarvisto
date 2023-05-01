#include <gtest/gtest.h>

#include "jarvis/Utils.hpp"

#include <chrono>

using namespace std::literals;

using namespace testing;
using namespace jar;

namespace krn = std::chrono;

TEST(UtilsTest, ParseValidDateTime)
{
    static const std::string_view kInput{"2023-04-22T13:20:11.000+03:00"};
    auto ts = parseDateTime(kInput);
    ASSERT_TRUE(ts.has_value());

    krn::year_month_day ymd{krn::year{2023} / 4 / 22};
    krn::sys_seconds expected = krn::sys_days{ymd} + 13h + 20min + 11s;

    EXPECT_EQ(ts.value(), expected.time_since_epoch().count());
}

TEST(UtilsTest, ParseInvalidDateTime)
{
    static const std::string_view kInput1{"2023-04-22T13:20:"};
    auto ts1 = parseDateTime(kInput1);
    EXPECT_FALSE(ts1.has_value());

    static const std::string_view kInput2{"2023-s4-q2T13:20:11"};
    auto ts2 = parseDateTime(kInput2);
    EXPECT_FALSE(ts2.has_value());
}