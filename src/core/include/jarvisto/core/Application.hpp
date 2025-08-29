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

#include "jarvisto/core/Subsystem.hpp"
#include "jarvisto/core/Export.hpp"

#include <vector>
#include <memory>

namespace jar {

class JARVISTO_CORE_EXPORT Application : public Subsystem {
public:
    static Application&
    instance();

    ~Application() override;

    [[nodiscard]] bool
    parseArgs(int argc, char* argv[]);

    [[nodiscard]] int
    run();

protected:
    Application();

    virtual void
    proceed();

    virtual void
    defineOptions();

    [[nodiscard]] virtual bool
    processOptions(int argc, char* argv[]);

    void
    addSubsystem(std::unique_ptr<Subsystem> subsystem);

    void
    initialize(Application& application) override;

    void
    setUp(Application& application) override;

    void
    tearDown() override;

    void
    finalize() override;

private:
    void
    handleHelp();

private:
    using Subsystems = std::vector<std::unique_ptr<Subsystem>>;
    Subsystems _subsystems;
    bool _helpRequested;
    static Application* s_instance;
};

} // namespace jar

#define APP_MAIN(ServiceName)                                                                      \
    int main(int argn, char* argv[])                                                               \
    {                                                                                              \
        try {                                                                                      \
            if (ServiceName service; service.parseArgs(argn, argv)) {                              \
                return service.run();                                                              \
            }                                                                                      \
        } catch (...) {                                                                            \
            /* Suppress any unhandled exceptions */                                                \
        }                                                                                          \
        return EXIT_FAILURE;                                                                       \
    }
