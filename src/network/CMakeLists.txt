include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(GenerateExportHeader)

add_library(jarvisto-network SHARED)
add_library(Jarvisto::Network ALIAS jarvisto-network)

set_target_properties(jarvisto-network
    PROPERTIES
        EXPORT_NAME Network
)

set_target_properties(jarvisto-network
    PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES
        INTERFACE_FLOAT_SUPPORT YES
)

target_sources(jarvisto-network
    PRIVATE src/SecureContext.cpp
            src/Worker.cpp
            src/WorkerGroup.cpp
            src/Http.cpp
            src/Cancellable.cpp
            src/HttpRequest.cpp
            src/Mqtt.cpp
            src/MqttClient.cpp
            src/AvailabilityObserver.cpp
            src/AvailabilityPublisher.cpp
            src/AvailabilitySubject.cpp
)

target_include_directories(jarvisto-core
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE $<BUILD_INTERFACE:${CMAKE_RUNTIME_OUTPUT_DIRECTORY}>
            $<BUILD_INTERFACE:${MOSQUITTO_INCLUDE_DIRS}>
)

target_link_libraries(jarvisto-network
    PUBLIC Jarvisto::Core
           Boost::url
           PkgConfig::SigCpp
           OpenSSL::SSL
           OpenSSL::Crypto
    PRIVATE PkgConfig::Mosquitto
)

target_compile_features(jarvisto-network PUBLIC cxx_std_23)

generate_export_header(jarvisto-network
    BASE_NAME JARVISTO_NETWORK
    EXPORT_FILE_NAME jarvisto/network/Export.hpp
)

install(TARGETS jarvisto-network
    EXPORT JarvistoNetworkTargets
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT Jarvisto_Lib
        NAMELINK_COMPONENT Jarvisto_DevLib
    FILE_SET HEADERS
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT JarvistoNetworkTargets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jarvisto
    NAMESPACE Jarvisto::
    COMPONENT Jarvisto_DevLib
    FILE JarvistoNetworkTargets.cmake
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    TYPE INCLUDE
    COMPONENT Jarvisto_DevLib
    FILES_MATCHING
    PATTERN network/*.hpp
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/jarvisto/network/Export.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/jarvisto/network
    COMPONENT Jarvisto_DevLib
)

if(ENABLE_TESTS)
    add_subdirectory(test)
endif()
