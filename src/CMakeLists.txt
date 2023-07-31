include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)

set(TARGET jarvisto)

add_library(${TARGET} SHARED)
add_library(Jarvisto::Jarvisto ALIAS ${TARGET})

set_target_properties(${TARGET}
    PROPERTIES
        EXPORT_NAME Jarvisto
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
        src/SecureContext.cpp
        src/Worker.cpp
        src/WorkerGroup.cpp
        src/Network.cpp
        src/Cancellable.cpp
        src/HttpRequest.cpp
        src/Utils.cpp
        src/DateTime.cpp
        src/Timestamp.cpp
)

target_include_directories(${TARGET}
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
            $<BUILD_INTERFACE:${CMAKE_RUNTIME_OUTPUT_DIRECTORY}>
)

target_link_libraries(${TARGET}
    PUBLIC spdlog::spdlog
           Boost::headers
           Boost::program_options
           Boost::url
           OpenSSL::SSL
           OpenSSL::Crypto
           date::date
           date::date-tz
)

if (ENABLE_DLT)
    target_sources(${TARGET} PRIVATE src/LoggerDltSink.cpp)
    target_link_libraries(${TARGET} PRIVATE Genivi::dlt)
endif()

target_compile_features(${TARGET} PUBLIC cxx_std_23)

target_compile_definitions(${TARGET}
    PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG
)

# Generate export header file with export macros
generate_export_header(${TARGET}
    BASE_NAME JARVISTO
    EXPORT_FILE_NAME jarvisto/Export.hpp
)

# Install target files except namelink file
install(TARGETS ${TARGET}
    EXPORT JarvistoRuntime
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT JarvistoRuntime
        NAMELINK_SKIP
    INCLUDES
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install target namelink file
install(TARGETS ${TARGET}
    EXPORT JarvistoDevelopment
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT JarvistoDevelopment
        NAMELINK_ONLY
    ARCHIVE
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT JarvistoDevelopment
)

# Install package config file
install(EXPORT JarvistoRuntime
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jarvisto
    NAMESPACE Jarvisto::
    COMPONENT JarvistoRuntime
    FILE JarvistoRuntime.cmake
)

# Install package config file
install(EXPORT JarvistoDevelopment
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jarvisto
    NAMESPACE Jarvisto::
    COMPONENT JarvistoDevelopment
    FILE JarvistoDevelopment.cmake
)

# Install library header files
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    TYPE INCLUDE
    COMPONENT JarvistoDevelopment
    FILES_MATCHING
        PATTERN *.hpp
)

# Install export header file
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/jarvisto/Export.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/jarvisto
    COMPONENT JarvistoDevelopment
)

# Generate package config and version file
configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/config/cmake/JarvistoConfig.cmake.in JarvistoConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jarvisto
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/JarvistoConfigVersion.cmake
    COMPATIBILITY SameMajorVersion
)

# Install package config and version file
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/JarvistoConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jarvisto
    COMPONENT JarvistoDevelopment
)
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/JarvistoConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jarvisto
    COMPONENT JarvistoDevelopment
)