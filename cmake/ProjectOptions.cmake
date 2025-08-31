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

include(FeatureSummary)

option(ENABLE_CODE_FORMAT "Enable code formatting" ON)
add_feature_info(
    ENABLE_CODE_FORMAT ENABLE_CODE_FORMAT "Enable code formatting support"
)

option(ENABLE_NETWORK_COMPONENT "Enable network component" OFF)
if(ENABLE_NETWORK_COMPONENT)
    list(APPEND VCPKG_MANIFEST_FEATURES "network")
endif()
add_feature_info(
    ENABLE_NETWORK_COMPONENT ENABLE_NETWORK_COMPONENT "Build with network component"
)

option(ENABLE_TESTS "Enable testing" OFF)
if(ENABLE_TESTS)
    list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()
add_feature_info(
    ENABLE_TESTS ENABLE_TESTS "Build project with tests"
)

feature_summary(WHAT ALL)
