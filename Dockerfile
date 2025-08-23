# syntax=docker/dockerfile:1

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
                   libgtest-dev libgmock-dev nlohmann-json3-dev

RUN groupadd -f -g $USER_GID $USERNAME \
 && useradd -l -g $USER_GID -G sudo --uid $USER_UID -ms /bin/bash $USERNAME \
 && echo $USERNAME:$USERNAME | chpasswd \
 && echo $USERNAME 'ALL=(ALL) NOPASSWD:SETENV: ALL' > /etc/sudoers.d/010_$USERNAME || true
