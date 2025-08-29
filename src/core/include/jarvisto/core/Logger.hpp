// Copyright 2025 Denys Asauliak
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <spdlog/spdlog.h>

#include <fmt/std.h>

// clang-format off
#define LOGD(...) \
    SPDLOG_DEBUG(__VA_ARGS__)
#define LOGI(...) \
    SPDLOG_INFO(__VA_ARGS__)
#define LOGW(...) \
    SPDLOG_WARN(__VA_ARGS__)
#define LOGE(...) \
    SPDLOG_ERROR(__VA_ARGS__)
// clang-format on