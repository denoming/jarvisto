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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "jarvisto/network/HttpRequest.hpp"
#include "jarvisto/network/SecureContext.hpp"
#include "jarvisto/network/Worker.hpp"

#include <thread>
#include <string_view>

using namespace jar;
using namespace testing;

using namespace std::literals;

class HttpRequestTest : public Test {
public:
    const std::string_view kUrl{
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
