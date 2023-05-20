#include <gtest/gtest.h>

#include "jarvis/DateTime.hpp"

#include <date/date.h>
#include <date/tz.h>

using namespace date;
using namespace testing;
using namespace jar;

using namespace std::literals;

TEST(DateTimeTest, GetTimestamp)
{
    const auto ts = getUtcTimestamp();
    EXPECT_NE(ts, UtcTimestamp{});
}

TEST(DateTimeTest, ParseUtcDateTime)
{
    year_month_day ymd{April / 22 / 2023};
    const auto expected = clock_cast<utc_clock>(sys_days{ymd} + 13h + 20min + 11s);

    auto ts1 = parseUtcDateTime("2023-04-22T13:20:11Z");
    ASSERT_TRUE(ts1.has_value());
    EXPECT_EQ(ts1.value(), expected);

    auto ts2 = parseUtcDateTime("2023-04-22T13:20:11.000Z");
    ASSERT_TRUE(ts2.has_value());
    EXPECT_EQ(ts2.value(), expected);

    auto ts3 = parseUtcDateTime("2023-04-22T13:20:11.XXXZ");
    EXPECT_FALSE(ts3);
}

TEST(DateTimeTest, FormatUtcDateTime)
{
    year_month_day ymd{April / 22 / 2023};
    const auto input = clock_cast<utc_clock>(sys_days{ymd} + 13h + 20min + 11s);

    auto str = formatUtcDateTime(input);
    EXPECT_EQ(str, "2023-04-22T13:20:11Z");
}