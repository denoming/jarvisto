# syntax=docker/dockerfile:1

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

ARG BASE_CONTAINER=python:3.12-bookworm
FROM $BASE_CONTAINER

ARG USERNAME=bender
ARG USER_UID=1000
ARG USER_GID=1000

RUN apt update \
 && apt install -y build-essential autoconf automake autopoint sudo vim git \
                   cmake ninja-build gdb curl tar zip unzip sudo dbus flex \
                   bison nasm texinfo wget file pkg-config libtool \
                   python3 libmosquitto-dev libmosquittopp-dev libssl-dev \
                   libboost1.81-dev libboost-url1.81-dev libsystemd-dev \
                   libspdlog-dev libhowardhinnant-date-dev libsigc++-3.0-dev \
                   libgtest-dev libgmock-dev nlohmann-json3-dev clang-format-16

RUN groupadd -f -g $USER_GID $USERNAME \
 && useradd -l -g $USER_GID -G sudo --uid $USER_UID -ms /bin/bash $USERNAME \
 && echo $USERNAME:$USERNAME | chpasswd \
 && echo $USERNAME 'ALL=(ALL) NOPASSWD:SETENV: ALL' > /etc/sudoers.d/010_$USERNAME || true
