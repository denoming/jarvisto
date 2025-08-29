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

add_library(jarvisto-core SHARED)
add_library(Jarvisto::Core ALIAS jarvisto-core)

set_target_properties(jarvisto-core
    PROPERTIES
        EXPORT_NAME Core
)

set_target_properties(jarvisto-core
    PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        CXX_VISIBILITY_PRESET hidden
)

target_sources(jarvisto-core
    PRIVATE src/Subsystem.cpp
            src/Application.cpp
            src/LoggerFactory.cpp
            src/Environment.cpp
            src/DateTime.cpp
            src/Timestamp.cpp
            src/SignalHandler.cpp
            src/Config.cpp
)

target_include_directories(jarvisto-core
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE $<BUILD_INTERFACE:${CMAKE_RUNTIME_OUTPUT_DIRECTORY}>
)

target_link_libraries(jarvisto-core
    PUBLIC spdlog::spdlog
           date::date
           date::date-tz
    PRIVATE nlohmann_json::nlohmann_json
            PkgConfig::Systemd
)

target_compile_features(jarvisto-core PUBLIC cxx_std_23)

target_compile_definitions(jarvisto-core
    PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG
)

generate_export_header(jarvisto-core
    BASE_NAME JARVISTO_CORE
    EXPORT_FILE_NAME jarvisto/core/Export.hpp
)

install(TARGETS jarvisto-core
    EXPORT JarvistoCoreTargets
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT Jarvisto_Lib
        NAMELINK_COMPONENT Jarvisto_DevLib
    FILE_SET HEADERS
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT JarvistoCoreTargets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jarvisto
    NAMESPACE Jarvisto::
    COMPONENT Jarvisto_DevLib
    FILE JarvistoCoreTargets.cmake
)

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
    TYPE INCLUDE
    COMPONENT Jarvisto_DevLib
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/jarvisto/core/Export.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/jarvisto/core
    COMPONENT Jarvisto_DevLib
)

if(ENABLE_TESTS)
    add_subdirectory(test)
endif()
