# Copyright 2025 Denys Asauliak
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE $<BUILD_INTERFACE:${CMAKE_RUNTIME_OUTPUT_DIRECTORY}>
            $<BUILD_INTERFACE:${MOSQUITTO_INCLUDE_DIRS}>
)

target_link_libraries(jarvisto-network
    PUBLIC Jarvisto::Core
           PkgConfig::SigCpp
           PkgConfig::OpenSSL
           PkgConfig::OpenCrypto
    PRIVATE PkgConfig::Mosquitto
            ada-url::ada-url
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

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
    TYPE INCLUDE
    COMPONENT Jarvisto_DevLib
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/jarvisto/network/Export.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/jarvisto/network
    COMPONENT Jarvisto_DevLib
)

if(ENABLE_TESTS)
    add_subdirectory(test)
endif()
