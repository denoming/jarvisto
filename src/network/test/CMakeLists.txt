add_executable(jarvisto-network-test "")

target_include_directories(jarvisto-network-test
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include
)

target_sources(jarvisto-network-test
    PRIVATE src/MockAvailabilitySubject.cpp
            src/MockMqttClient.cpp
            src/WorkerTest.cpp
            src/WorkerGroupTest.cpp
            src/AsyncResultTest.cpp
            src/AvailabilityObserverTest.cpp
            src/AvailabilityPublisherTest.cpp
            src/HttpRequestTest.cpp
            src/MqttAsyncClientTest.cpp
)

target_link_libraries(jarvisto-network-test
    PRIVATE Jarvisto::Network
            Jarvisto::Test
)

target_compile_features(jarvisto-network-test PUBLIC cxx_std_23)

if(NOT CMAKE_CROSSCOMPILING)
    gtest_discover_tests(jarvisto-network-test
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        PROPERTIES LABELS "Unit"
    )
endif()
