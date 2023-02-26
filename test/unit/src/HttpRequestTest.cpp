#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "jarvis/HttpRequest.hpp"
#include "jarvis/SecureContext.hpp"
#include "jarvis/Worker.hpp"

#include <thread>

using namespace jar;
using namespace testing;

using namespace std::literals;

class HttpRequestTest : public Test {
public:
    const urls::url kUrl{"<PUT-ADDRESS-HERE>"};

    HttpRequestTest()
        : request{HttpRequest::create(io::make_strand(worker.executor()), context.ref())}
    {
    }

    void
    SetUp() override
    {
        worker.start();
    }

    void
    TearDown() override
    {
        worker.stop();
    }

public:
    Worker worker;
    SecureContext context;
    HttpRequest::Ptr request;
};

TEST_F(HttpRequestTest, DISABLED_GET)
{
    MockFunction<HttpRequest::SetterType::OnReady> onReady;
    MockFunction<HttpRequest::SetterType::OnError> onError;

    EXPECT_CALL(onReady, Call(Not(IsEmpty())));
    auto result = request->GET(kUrl, onReady.AsStdFunction(), onError.AsStdFunction());
    EXPECT_TRUE(request->pending());
    result.wait();
    EXPECT_FALSE(request->pending());
    EXPECT_TRUE(result.ready());
}