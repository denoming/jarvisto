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

set(CPACK_DEB_COMPONENT_INSTALL ON)

set(CPACK_DEBIAN_ENABLE_COMPONENT_DEPENDS ON)
set(CPACK_DEBIAN_PACKAGE_SECTION "sound")
if(DEFINED VCPKG_INSTALLED_DIR)
    list(APPEND CPACK_DEBIAN_PACKAGE_SHLIBDEPS_PRIVATE_DIRS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib")
endif()

# Runtime package configuration
set(CPACK_DEBIAN_JARVISTO_LIB_PACKAGE_NAME "libjarvisto")
set(CPACK_DEBIAN_JARVISTO_LIB_FILE_NAME DEB-DEFAULT)
if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(CPACK_DEBIAN_JARVISTO_LIB_DEBUGINFO_PACKAGE ON)
endif()

# Set package dependencies
string(CONCAT CPACK_DEBIAN_JARVISTO_LIB_PACKAGE_DEPENDS
    "libspdlog-dev (>= 1.10.0),"
    "libboost-url1.81-dev (>= 1.81.0),"
    "libhowardhinnant-date-dev (>= 3.0.1),"
    "libsigc++-3.0-dev (>= 3.4.0),"
    "libmosquitto1 (>= 2.0.11),"
    "libmosquittopp1 (>= 2.0.11),"
    "libssl3 (>= 3.0.15)"
)

# Development package configuration
set(CPACK_DEBIAN_JARVISTO_DEVLIB_PACKAGE_NAME "libjarvisto-dev")
set(CPACK_DEBIAN_JARVISTO_DEVLIB_FILE_NAME DEB-DEFAULT)
