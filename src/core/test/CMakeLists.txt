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

add_executable(jarvisto-core-test "")

target_sources(jarvisto-core-test
    PRIVATE src/DateTimeTest.cpp
            src/ConfigTest.cpp
            src/LoggerFactoryTest.cpp
)

target_link_libraries(jarvisto-core-test
    PRIVATE Jarvisto::Test
            Jarvisto::Core
)

target_compile_features(jarvisto-core-test PUBLIC cxx_std_23)

if(NOT CMAKE_CROSSCOMPILING)
    gtest_discover_tests(jarvisto-core-test
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        PROPERTIES LABELS "Unit"
    )
endif()
