set(TARGET jarvisto-unit-test)

add_executable(${TARGET} "")

target_sources(${TARGET}
    PRIVATE src/WorkerTest.cpp
            src/WorkerGroupTest.cpp
            src/AsyncResultTest.cpp
            src/HttpRequestTest.cpp
            src/DateTimeTest.cpp
            src/MqttAsyncClientTest.cpp
)

target_link_libraries(${TARGET}
    PRIVATE Jarvisto::Test::Common
)

target_compile_features(${TARGET} PUBLIC cxx_std_23)

install(
    TARGETS ${TARGET}
    COMPONENT JarvistoRuntime
)

if(NOT CMAKE_CROSSCOMPILING)
    gtest_discover_tests(${TARGET}
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        PROPERTIES LABELS "Unit"
    )
endif()

