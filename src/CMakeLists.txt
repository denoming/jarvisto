include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)

add_library(jarvisto SHARED)
add_library(Jarvisto::Jarvisto ALIAS jarvisto)

set_target_properties(jarvisto
    PROPERTIES
        EXPORT_NAME Jarvisto
)

set_target_properties(jarvisto
    PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES
        INTERFACE_FLOAT_SUPPORT YES
)

target_sources(jarvisto
    PRIVATE src/Subsystem.cpp
            src/Application.cpp
            src/LoggerInitializer.cpp
            src/SecureContext.cpp
            src/Worker.cpp
            src/WorkerGroup.cpp
            src/Http.cpp
            src/Cancellable.cpp
            src/HttpRequest.cpp
            src/Utils.cpp
            src/DateTime.cpp
            src/Timestamp.cpp
            src/Mqtt.cpp
            src/MqttClient.cpp
            src/MqttBasicClient.cpp
            src/AvailabilityObserver.cpp
            src/AvailabilityPublisher.cpp
            src/AvailabilitySubject.cpp
)

target_include_directories(jarvisto
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
            $<BUILD_INTERFACE:${CMAKE_RUNTIME_OUTPUT_DIRECTORY}>
)

target_link_libraries(jarvisto
    PUBLIC spdlog::spdlog
           Boost::headers
           Boost::program_options
           Boost::url
           date::date
           date::date-tz
           PkgConfig::SigCpp
    PRIVATE mosquitto::mosquitto
            OpenSSL::SSL
            OpenSSL::Crypto
)

target_compile_features(jarvisto PUBLIC cxx_std_23)

target_compile_definitions(jarvisto
    PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG
)

generate_export_header(jarvisto
    BASE_NAME JARVISTO
    EXPORT_FILE_NAME jarvisto/Export.hpp
)

install(TARGETS jarvisto
    EXPORT JarvistoTargets
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT Jarvisto_Lib
        NAMELINK_COMPONENT Jarvisto_DevLib
    FILE_SET HEADERS
)

install(TARGETS jarvisto
    FILE_SET HEADERS
    COMPONENT Jarvisto_DevLib
    INCLUDES
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT JarvistoTargets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jarvisto
    NAMESPACE Jarvisto::
    COMPONENT Jarvisto_DevLib
    FILE JarvistoTargets.cmake
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
        TYPE INCLUDE
        COMPONENT Jarvisto_DevLib
        FILES_MATCHING
        PATTERN *.hpp
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/jarvisto/Export.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/jarvisto
        COMPONENT Jarvisto_DevLib
)
