#include "jarvisto/core/Application.hpp"

#include "jarvisto/core/Logger.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>

namespace asio = boost::asio;

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
    if (not waitForTermination()) {
        LOGE("Waiting for termination has failed");
    }
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

bool
Application::waitForTermination()
{
    asio::io_context context;
    asio::signal_set signals(context, SIGINT, SIGTERM);
    signals.async_wait([this, &context](const auto& /*error*/, int signal) {
        if (!context.stopped()) {
            LOGD("Terminate <{}> application by <{}> signal", name(), signal);
            context.stop();
        }
    });
    return (context.run() > 0);
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