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

#include "jarvisto/core/Application.hpp"

#include "jarvisto/core/Logger.hpp"

namespace jar {

Application* Application::s_instance{nullptr};

Application&
Application::instance()
{
    assert(s_instance != nullptr);
    return *s_instance;
}

Application::Application()
    : _helpRequested{false}
{
    s_instance = this;
}

Application::~Application()
{
    s_instance = nullptr;
}

bool
Application::parseArgs(const int argc, char* argv[])
{
    defineOptions();
    return processOptions(argc, argv);
}

int
Application::run()
{
    if (_helpRequested) {
        return EXIT_SUCCESS;
    }

    try {
        initialize(*this);
        setUp(*this);
        proceed();
        tearDown();
    } catch (const std::exception& e) {
        LOGE("Application exception: {}", e.what());
        return EXIT_FAILURE;
    }
    finalize();

    return EXIT_SUCCESS;
}

void
Application::proceed()
{
}

void
Application::defineOptions()
{
}

bool
Application::processOptions(int argc, char* argv[])
{
    return true;
}

void
Application::addSubsystem(std::unique_ptr<Subsystem> subsystem)
{
    _subsystems.push_back(std::move(subsystem));
}

void
Application::initialize(Application& application)
{
    for (const auto& subsystem : _subsystems) {
        subsystem->initialize(*this);
    }
}

void
Application::setUp(Application& application)
{
    for (const auto& subsystem : _subsystems) {
        subsystem->setUp(*this);
    }
}

void
Application::tearDown()
{
    for (const auto& subsystem : _subsystems) {
        subsystem->tearDown();
    }
}

void
Application::finalize()
{
    for (const auto& subsystem : _subsystems) {
        subsystem->finalize();
    }
}

void
Application::handleHelp()
{
    _helpRequested = true;
}

} // namespace jar