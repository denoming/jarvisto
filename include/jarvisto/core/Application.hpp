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
