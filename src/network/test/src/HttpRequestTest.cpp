#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "jarvisto/network/HttpRequest.hpp"
#include "jarvisto/network/SecureContext.hpp"
#include "jarvisto/network/Worker.hpp"

#include <thread>

using namespace jar;
using namespace testing;

using namespace std::literals;

class HttpRequestTest : public Test {
public:
    const urls::url kUrl{
        "https://api.waqi.info/feed/A252832/?token=e971e6bb50fff16c97de289eb50cb69952413a04"};

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
    std::shared_ptr<HttpRequest> request;
};

TEST_F(HttpRequestTest, GET)
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
