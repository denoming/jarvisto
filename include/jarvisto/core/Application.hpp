#pragma once

#include "jarvisto/core/Subsystem.hpp"
#include "jarvisto/core/Export.hpp"

#include <boost/program_options.hpp>

#include <vector>

namespace jar {

class JARVISTO_CORE_EXPORT Application : public Subsystem {
public:
    static Application&
    instance();

    ~Application() override;

    void
    parseArgs(int argc, char* argv[]);

    int
    run();

    [[nodiscard]] const boost::program_options::variables_map&
    options() const;

protected:
    Application();

    virtual void
    proceed();

    virtual void
    defineOptions(boost::program_options::options_description& description);

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
    handleHelp(const boost::program_options::options_description& description);

private:
    using Subsystems = std::vector<std::unique_ptr<Subsystem>>;

    Subsystems _subsystems;
    boost::program_options::variables_map _options;
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
