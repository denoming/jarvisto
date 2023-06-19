cmake_minimum_required(VERSION 3.22)

project(jarvisto
    VERSION 0.2.1
    DESCRIPTION "Jarvisto Library"
)

include(cmake/ProjectOptions.cmake)
include(cmake/ProjectConfigs.cmake)
include(cmake/ProjectInstall.cmake)

add_subdirectory(config)
add_subdirectory(src)
if(ENABLE_TESTS)
    add_subdirectory(test)
endif()
