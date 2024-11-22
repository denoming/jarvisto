#!/usr/bin/env bash

set -e

PROJECT_ROOT=$(dirname "$(dirname "$(realpath -s $0)")")

# Handle arguments
PLATFORM_ARCH="${1:-arm64}"
PLATFORM_VARIANT="${2:-v8}"
USER_NAME="${3:-$(whoami)}"

# Define variables
USER_UID="$(id ${USERNAME} -u)"
USER_GID="$(id ${USERNAME} -g)"
PLATFORM="${PLATFORM_ARCH}${PLATFORM_VARIANT}"
IMAGE_NAME="my/jarvisto:${PLATFORM_ARCH}${PLATFORM_VARIANT}"

echo "=============================="
echo "        Platform: ${PLATFORM}"
echo "           Image: ${IMAGE_NAME}"
echo "        Username: ${USER_NAME}"
echo "         User ID: ${USER_UID}"
echo "        Group ID: ${USER_GID}"
echo "=============================="

command -v docker > /dev/null
if [ $? != 0 ]; then
    echo -e "Error:"
    echo -e "Docker is not installed"
    exit 1
fi

build_image() {
  BUILD_CMD=(docker build \
  --platform "linux/${PLATFORM_ARCH}/${PLATFORM_VARIANT}" \
  --tag "${IMAGE_NAME}" \
  --build-arg "PLATFORM=${PLATFORM}" \
  --build-arg "USER_NAME=${USER_NAME}" \
  --build-arg "USER_UID=${USER_UID}" \
  --build-arg "USER_GID=${USER_GID}" \
  --file "${PROJECT_ROOT}/Dockerfile"
  "${PROJECT_ROOT}")

  if [ -z "$(docker images -q ${IMAGE_NAME})" ]; then
    echo -e "Building <${IMAGE_NAME}> image"
    echo "${BUILD_CMD[@]}"
    "${BUILD_CMD[@]}"
  fi
}

run_image() {
  RUN_CMD=(docker run -it \
  --platform "linux/${PLATFORM_ARCH}/${PLATFORM_VARIANT}" \
  --rm \
  #--user "${USER_UID}:${USER_GID}" \
  --volume "${HOME}/.ssh:${HOME}/.ssh" \
  --volume "${PROJECT_ROOT}:${PROJECT_ROOT}" \
  --network host \
  --workdir "${PROJECT_ROOT}" \
  "${IMAGE_NAME}")

  if [ -n "$(docker images -q ${IMAGE_NAME})" ]; then
    "${RUN_CMD[@]}"
  else
    echo "Docker image <${IMAGE_NAME}> is absent"
  fi
}

build_image
run_image
