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

#pragma once

#include "jarvisto/core/Export.hpp"

namespace jar {

class Application;

class JARVISTO_CORE_EXPORT Subsystem {
public:
    virtual ~Subsystem() = default;

    [[nodiscard]] virtual const char*
    name() const
        = 0;

protected:
    friend class Application;

    virtual void
    initialize(Application& application);

    virtual void
    setUp(Application& application);

    virtual void
    tearDown();

    virtual void
    finalize();
};

} // namespace jar