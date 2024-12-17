add_executable(jarvisto-core-test "")

target_sources(jarvisto-core-test
    PRIVATE src/DateTimeTest.cpp
)

target_link_libraries(jarvisto-core-test
    PRIVATE Jarvisto::Core
            Jarvisto::Test
)

target_compile_features(jarvisto-core-test PUBLIC cxx_std_23)

if(NOT CMAKE_CROSSCOMPILING)
    gtest_discover_tests(jarvisto-core-test
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        PROPERTIES LABELS "Unit"
    )
endif()
