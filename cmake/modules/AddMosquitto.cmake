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

# Try to find CMake target (vcpkg)
find_package(unofficial-mosquitto QUIET CONFIG)
if(TARGET unofficial::mosquitto::mosquitto)
    set(MOSQUITTO_LIBRARIES unofficial::mosquitto::mosquitto)
else()
    # Try to find system wide library using pkg-config
    find_package(PkgConfig)
    pkg_check_modules(Mosquitto IMPORTED_TARGET libmosquitto)
    if(NOT DEFINED Mosquitto_FOUND)
        message(FATAL_ERROR "Unable to find mosquitto dependency")
    endif()
    set(MOSQUITTO_LIBRARIES PkgConfig::Mosquitto)
endif()
