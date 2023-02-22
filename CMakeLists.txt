cmake_minimum_required(VERSION 3.22)

project(jarvis-common
    VERSION 0.1.1
    DESCRIPTION "J.A.R.V.I.S Common Library"
)

include(cmake/ProjectOptions.cmake)
include(cmake/ProjectConfigs.cmake)
include(cmake/ProjectInstall.cmake)

add_subdirectory(src)
if(JARVIS_ENABLE_TESTS)
    add_subdirectory(test)
endif()