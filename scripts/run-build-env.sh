#!/usr/bin/env bash

set -e

push_image=false
platform_arch="arm64"
platform_variant="v8"

while getopts "pa:v:" flag; do
  case "$flag" in
    p)
      push_image=true
      ;;
    a)
      platform_arch="$OPTARG"
      ;;
    v)
      platform_variant="$OPTARG"
      ;;
    ?)
      echo "Invalid option: -$OPTARG" >&2
      exit 1
      ;;
  esac
done

dir_root="$(dirname "$(dirname "$(realpath -s $0)")")"
username="$(whoami)"
user_uid="$(id -u)"
user_gid="$(id -g)"
platform="${platform_arch}${platform_variant}"
image="denoming/jarvisto:${platform_arch}${platform_variant}"

echo "=============================="
echo "        Platform: ${platform}"
echo "           Image: ${image}"
echo "=============================="

command -v docker > /dev/null
if [ $? != 0 ]; then
    echo -e "Error:"
    echo -e "Docker is not installed"
    exit 1
fi

build_image() {
  CMD=(docker build \
  --platform "linux/${platform_arch}/${platform_variant}" \
  --tag "${image}" \
  --build-arg "BASE_CONTAINER=debian:bookworm" \
  --build-arg "USERNAME=${username}" \
  --build-arg "USER_UID=${user_uid}" \
  --build-arg "USER_GID=${user_gid}" \
  --file "${dir_root}/Dockerfile"
  "${dir_root}")

  if [ -z "$(docker images -q ${image})" ]; then
    echo -e "Building <${image}> image"
    echo "${CMD[@]}"
    "${CMD[@]}"
  fi
}

run_image() {
  CMD=(docker run -it \
  --platform "linux/${platform_arch}/${platform_variant}" \
  --rm \
  --user "${user_uid}:${user_gid}" \
  --volume "${HOME}/.ssh:${HOME}/.ssh" \
  --volume "${dir_root}:${dir_root}" \
  --network "host" \
  --workdir "${dir_root}" \
  "${image}")

  if [ -n "$(docker images -q ${image})" ]; then
    echo -e "Running <${image}> image"
    echo "${CMD[@]}"
    "${CMD[@]}"
  else
    echo "Docker image <${image}> is absent"
  fi
}

push_image() {
  CMD=(docker image push ${image})
  if [ -n "$(docker images -q ${image})" ]; then
    echo -e "Pushing <${image}> image"
    echo "${CMD[@]}"
    "${CMD[@]}"
  else
    echo "Docker image <${image}> is absent"
  fi
}

build_image
if [ "$push_image" == "true" ]; then
  push_image
else
  run_image
fi
