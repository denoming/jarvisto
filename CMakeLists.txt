cmake_minimum_required(VERSION 3.25.1)

include(cmake/ProjectOptions.cmake)

project(jarvisto
    VERSION 0.3.6
    DESCRIPTION "Jarvisto Library"
)

include(cmake/ProjectConfigs.cmake)
if (PROJECT_IS_TOP_LEVEL)
    include(cmake/ProjectInstall.cmake)
endif()
if (PROJECT_IS_TOP_LEVEL)
    include(cmake/ProjectPackaging.cmake)
endif()

add_subdirectory(config)
add_subdirectory(src)
add_subdirectory(tools)
