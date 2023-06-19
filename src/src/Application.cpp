#include "jarvisto/Application.hpp"

#include "jarvisto/Logger.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>

#include <iostream>

namespace asio = boost::asio;
namespace sys = boost::system;
namespace po = boost::program_options;

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

void
Application::parseArgs(int argc, char* argv[])
{
    processOptions(argc, argv);
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

const po::variables_map&
Application::options() const
{
    return _options;
}

void
Application::proceed()
{
    if (!waitForTermination()) {
        LOGE("Waiting for termination has failed");
    }
}

void
Application::defineOptions(po::options_description& description)
{
    description.add_options()("help,h", "Display help");
}

bool
Application::waitForTermination()
{
    asio::io_context context;
    boost::asio::signal_set signals(context, SIGINT, SIGTERM);
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
    for (auto& subsystem : _subsystems) {
        subsystem->initialize(*this);
    }
}

void
Application::setUp(Application& application)
{
    for (auto& subsystem : _subsystems) {
        subsystem->setUp(*this);
    }
}

void
Application::tearDown()
{
    for (auto& subsystem : _subsystems) {
        subsystem->tearDown();
    }
}

void
Application::finalize()
{
    for (auto& subsystem : _subsystems) {
        subsystem->finalize();
    }
}

void
Application::processOptions(int argc, char* argv[])
{
    po::options_description d{"Options"};
    defineOptions(d);
    po::store(po::parse_command_line(argc, argv, d), _options);
    po::notify(_options);
    if (_options.contains("help")) {
        handleHelp(d);
    }
}

void
Application::handleHelp(const po::options_description& description)
{
    _helpRequested = true;

    std::cout << description << std::endl;
}

} // namespace jar