#include <gtest/gtest.h>

#include "jarvisto/DateTime.hpp"

#include <date/date.h>
#include <date/tz.h>

using namespace date;
using namespace testing;
using namespace jar;

using namespace std::literals;

TEST(DateTimeTest, ParseUtcDateTime)
{
    year_month_day ymd{April / 22 / 2023};
    const auto expected = Timestamp{clock_cast<utc_clock>(sys_days{ymd} + 13h + 20min + 11s)};

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
    const auto expected = Timestamp{clock_cast<utc_clock>(sys_days{ymd} + 13h + 20min + 11s)};
    EXPECT_EQ(formatUtcDateTime(expected), "2023-04-22T13:20:11Z");
}

#include <sstream>

TEST(DateTimeTest, ParseZonedDateTime)
{
    static const std::string_view kInput{"2023-04-22T13:00:00.000+03:00"};

    year_month_day ymd{April / 22 / 2023};
    const auto expected = Timestamp{clock_cast<utc_clock>(sys_days{ymd} + 10h)};
    EXPECT_EQ(parseZonedDateTime(kInput), expected);
}