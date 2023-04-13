#pragma once

#include "jarvis/Subsystem.hpp"
#include "jarvis/CommonExport.hpp"

#include <boost/program_options.hpp>

#include <vector>

namespace po = boost::program_options;

namespace jar {

class JARC_EXPORT Application : public Subsystem {
public:
    static Application&
    instance();

    ~Application() override;

    void
    parseArgs(int argc, char* argv[]);

    int
    run();

    [[nodiscard]] const po::variables_map&
    options() const;

protected:
    Application();

    virtual const char*
    contextId()
        = 0;

    virtual const char*
    contextDesc()
        = 0;

    virtual void
    proceed();

    virtual void
    defineOptions(po::options_description& description);

    bool
    waitForTermination();

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
    processOptions(int argc, char* argv[]);

    void
    handleHelp(const po::options_description& description);

private:
    using Subsystems = std::vector<std::unique_ptr<Subsystem>>;

    Subsystems _subsystems;
    po::variables_map _options;
    bool _helpRequested;

    static Application* s_instance;
};

} // namespace jar

#define APP_MAIN(ServiceName)                                                                      \
    int main(int argn, char* argv[])                                                               \
    {                                                                                              \
        try {                                                                                      \
            ServiceName service;                                                                   \
            service.parseArgs(argn, argv);                                                         \
            service.run();                                                                         \
        } catch (...) {                                                                            \
            return EXIT_FAILURE;                                                                   \
        }                                                                                          \
        return EXIT_SUCCESS;                                                                       \
    }
