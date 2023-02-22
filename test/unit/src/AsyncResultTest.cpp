#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "jarvis/AsyncResult.hpp"

#include <thread>

using namespace jar;
using namespace testing;

struct Data {
    Data(int v1, const std::string& v2)
    {
    }
};

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
    EXPECT_CALL(onError, Call(Not(sys::error_code{})));
    s2.setError(sys::errc::make_error_code(sys::errc::bad_message));
}

TEST(AsyncResultTest, Wait)
{
    auto s = makeResultSetter<std::string>();
    auto r = s.getResult();

    std::jthread thread{[s = std::move(s)]() mutable {
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
        s.setError(sys::errc::make_error_code(sys::errc::bad_message));
    }};

    EXPECT_FALSE(r.ready());
    r.wait();
    EXPECT_TRUE(r.ready());
}
