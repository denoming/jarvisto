# syntax=docker/dockerfile:1

ARG PLATFORM=arm64v8
FROM $PLATFORM/debian:bookworm

ARG USER_NAME=bender
ARG USER_UID=1000
ARG USER_GID=1000

ENV VCPKG_ROOT="/home/$USER_NAME/.vcpkg"
ENV VCPKG_FORCE_SYSTEM_BINARIES=true
ENV VCPKG_DISABLE_METRICS=true

USER root

RUN apt update \
 && apt install -y build-essential autoconf automake autopoint sudo vim git \
                   cmake ninja-build gdb curl tar zip unzip sudo dbus flex \
                   bison nasm texinfo wget file pkg-config libtool \
                   python3 libmosquitto-dev libmosquittopp-dev libssl-dev \
                   libboost1.81-dev libboost-program-options1.81-dev libboost-url1.81-dev \
                   libspdlog-dev libhowardhinnant-date-dev libsigc++-3.0-dev \
                   libgtest-dev libgmock-dev nlohmann-json3-dev

# Create custom user
RUN groupadd -f -g $USER_GID $USER_NAME \
 && useradd -l -g $USER_GID -G sudo --uid $USER_UID -ms /bin/bash $USER_NAME \
 && echo $USER_NAME:$USER_NAME | chpasswd \
 && echo $USER_NAME 'ALL=(ALL) NOPASSWD:SETENV: ALL' > /etc/sudoers.d/010_$USER_NAME || true

USER $USER_NAME

CMD ["/bin/bash","--rcfile","$HOME/.profile"]
