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

#include "jarvisto/network/AvailabilityObserver.hpp"

using namespace jar;
using namespace testing;

class MockAvailabilitySubject : public AvailabilitySubject {
public:
    explicit MockAvailabilitySubject(std::string name)
        : AvailabilitySubject{std::move(name)}
    {
    }

    using AvailabilitySubject::availability;
};

TEST(AvailabilityObserverTest, AddSubjects)
{
    MockAvailabilitySubject s1{"Subject1"};
    MockAvailabilitySubject s2{"Subject2"};
    s1.availability(AvailabilityState::Online);
    s2.availability(AvailabilityState::Offline);

    AvailabilityObserver o1{"Observer1"};
    EXPECT_EQ(o1.state(), AvailabilityState::Offline);
    o1.add(s1);
    EXPECT_EQ(o1.state(), AvailabilityState::Online);
    o1.add(s2);
    EXPECT_EQ(o1.state(), AvailabilityState::Offline);

    o1.remove(s2);
    EXPECT_EQ(o1.state(), AvailabilityState::Online);

    MockFunction<AvailabilitySubject::OnStateUpdate> callback;
    EXPECT_CALL(callback, Call(Not(IsEmpty()), AvailabilityState::Offline));
    auto c1 = o1.onStateUpdate().connect(callback.AsStdFunction());
    s1.availability(AvailabilityState::Offline);
    EXPECT_EQ(o1.state(), AvailabilityState::Offline);
    c1.disconnect();
}