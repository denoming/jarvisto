list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")

if (DEFINED CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Toolchain file ${CMAKE_TOOLCHAIN_FILE}")
endif()

include(BuildType)
include(BuildLocation)
include(BuildOptions)

include(AddBoost)
include(AddSpdLog)
