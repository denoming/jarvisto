cmake_minimum_required(VERSION 3.22)

include(cmake/ProjectOptions.cmake)

project(jarvisto
    VERSION 0.3.4
    DESCRIPTION "Jarvisto Library"
)

include(cmake/ProjectConfigs.cmake)
include(cmake/ProjectInstall.cmake)

add_subdirectory(config)
add_subdirectory(src)
if(ENABLE_TESTS)
    add_subdirectory(test)
endif()
