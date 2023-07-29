#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "jarvisto/AsyncResult.hpp"

#include <thread>

using namespace jar;
using namespace testing;

struct Data {
    Data(int v1, std::string v2)
        : v1{v1}
        , v2{std::move(v2)}
    {
    }

    int v1;
    std::string v2;
};

static Matcher<Data>
matchTo(const Data& data)
{
    return AllOf(Field(&Data::v1, data.v1), Field(&Data::v2, data.v2));
}

TEST(AsyncResultTest, Submit)
{
    auto s1 = makeResultSetter<std::string>();
    auto r1 = s1.getResult();
    EXPECT_FALSE(r1.ready());
    s1.setValue("Value");
    EXPECT_TRUE(r1.ready());

    auto s2 = makeResultSetter<std::string>();
    auto r2 = s2.getResult();
    EXPECT_FALSE(r2.ready());
    std::string value{"Value"};
    s2.setValue(value);
    EXPECT_TRUE(r2.ready());

    auto s3 = makeResultSetter<Data>();
    auto r3 = s3.getResult();
    EXPECT_FALSE(r3.ready());
    s3.emplaceValue(0, "Value");
    EXPECT_TRUE(r3.ready());
    EXPECT_TRUE(r3.hasValue());
    EXPECT_THAT(r3.value(), matchTo(Data{0, "Value"}));
    EXPECT_FALSE(r3.hasError());
    EXPECT_ANY_THROW({ auto error = r3.error(); });
}

TEST(AsyncResultTest, Callback)
{
    using ResultHolder = AsyncResultSetter<std::string>;
    MockFunction<ResultHolder::OnReady> onReady;
    MockFunction<ResultHolder::OnError> onError;

    auto s1 = makeResultSetter<std::string>(onReady.AsStdFunction(), onError.AsStdFunction());
    auto r1 = s1.getResult();
    EXPECT_CALL(onReady, Call(Not(IsEmpty())));
    s1.setValue("value");

    auto s2 = makeResultSetter<std::string>(onReady.AsStdFunction(), onError.AsStdFunction());
    auto r2 = s2.getResult();
    EXPECT_CALL(onError, Call(Not(std::error_code{})));
    s2.setError(std::make_error_code(std::errc::bad_message));
}

TEST(AsyncResultTest, Wait)
{
    auto s = makeResultSetter<std::string>();
    auto r = s.getResult();

    std::jthread thread{[s = std::move(s)]() mutable {
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
        s.setError(std::make_error_code(std::errc::bad_message));
    }};

    EXPECT_FALSE(r.ready());
    r.wait();
    EXPECT_TRUE(r.ready());
}
