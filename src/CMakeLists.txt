include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)

set(TARGET jarvis-common-library)

add_library(${TARGET} SHARED)
add_library(Jarvis::Common ALIAS ${TARGET})

set_target_properties(${TARGET}
    PROPERTIES
        OUTPUT_NAME jarvis-common
        EXPORT_NAME Common
)

set_target_properties(${TARGET}
    PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES
        INTERFACE_FLOAT_SUPPORT YES
)

target_sources(${TARGET}
    PRIVATE
        src/Subsystem.cpp
        src/Application.cpp
        src/LoggerInitializer.cpp
)

target_include_directories(${TARGET}
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries(${TARGET}
    PUBLIC Boost::headers Boost::program_options
    PUBLIC spdlog::spdlog
)

target_compile_features(${TARGET} PUBLIC cxx_std_20)

# Generate export header file with export macros
generate_export_header(${TARGET}
    BASE_NAME JARC
    EXPORT_FILE_NAME jarvis/Export.hpp
)

# Install target files except namelink file
install(TARGETS ${TARGET}
    EXPORT JarvisCommonRuntime
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT JarvisCommonRuntime
        NAMELINK_SKIP
    INCLUDES
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install target namelink file
install(TARGETS ${TARGET}
    EXPORT JarvisCommonDevelopment
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT JarvisCommonDevelopment
        NAMELINK_ONLY
    ARCHIVE
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT JarvisCommonDevelopment
    INCLUDES
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install package config file
install(EXPORT JarvisCommonRuntime
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/JarvisCommon
    NAMESPACE Jarvis::
    COMPONENT JarvisCommonRuntime
    FILE JarvisCommonRuntime.cmake
)

# Install package config file
install(EXPORT JarvisCommonDevelopment
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/JarvisCommon
    NAMESPACE Jarvis::
    COMPONENT JarvisCommonDevelopment
    FILE JarvisCommonDevelopment.cmake
)

# Install library header files
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    TYPE INCLUDE
    COMPONENT JarvisCommonDevelopment
    FILES_MATCHING
        PATTERN *.hpp
)

# Install export header file
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/jarvis/Export.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/jarvis
    COMPONENT JarvisCommonDevelopment
)

# Generate package config and version file
configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/config/JarvisCommonConfig.cmake.in
    JarvisCommonConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/JarvisCommon
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/JarvisCommonConfigVersion.cmake
    COMPATIBILITY SameMajorVersion
)

# Install package config and version file
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/JarvisCommonConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/JarvisCommon
    COMPONENT JarvisCommonDevelopment
)
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/JarvisCommonConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/JarvisCommon
    COMPONENT JarvisCommonDevelopment
)