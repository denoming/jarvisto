# Introduction

The common classes are intended to be as building blocks for any service and helpful to handle service lifetime.

# Purpose

The main purpose is providing useful classes to compose any service and provide tools to handle service lifetime.

# Component Structure

## Software Units

| Name              | Description                                                 |
|-------------------|-------------------------------------------------------------|
| Application       | Represents the base class of service                        |
| Subsystem         | Represents the base class of subsystem within service       |
| AsyncResult       | Represents any asynchronous result                          |
| HttpRequest       | Represents HTTP request                                     |
| LoggerInitializer | The logger initializer to initialize logging within service |
| SecureContext     | The SSL secure context                                      |
| Worker            | The worker to handle multiple worker threads                |
| WorkerGroup       | The worker group to handle multiple workers                 |

## Class Diagrams

* Application service base class

```plantuml
@startuml

interface Subsystem {
    +name(): const char*
    #initialize()
    #setUp(application)
    #tearDown()
    #finalize()
}

class Application {
    +parseArgs(argc, argv)
    +run(): int
    +options(): options
    #contextId(): const char*
    #contextDesc(): const char*
    #proceed()
    #defineOptions(description)
    #waitForTermination()
    #addSubsystem()
    #initialize(application)
    #setUp(application)
    #tearDown()
    #finalize()
}

Subsystem <|.. Application
Application o-- Subsystem : 0..*

@enduml
```

* Worker and worker group classes

```plantuml
@startuml

interface IWorker {
    +active(): bool
    +start()
    +stop()
    +executor(): executor
    +current(): IWorker&
    +next(): IWorker&
}

IWorker <|.. Worker
IWorker <|.. WorkerGroup

WorkerGroup o-- Worker

@enduml
```

