# Jarvisto

[![CI](https://github.com/denoming/jarvisto/actions/workflows/build.yaml/badge.svg)](https://github.com/denoming/jarvisto/actions/workflows/build.yaml)

## Introduction

Basic building blocks library.

## Build

### By vcpkg (locally)

Debug configuration:
```shell
$ cmake --preset debug
$ cmake --build --preset build-debug
```
Release configuration:
```shell
$ cmake --preset release
$ cmake --build --preset build-release
```

### By vcpkg (in docker)

Debug configuration:
```shell
$ bash scripts/run-test-env.sh
$ cmake --preset debug-docker
$ cmake --build --preset build-debug-docker
```
Release configuration:
```shell
$ bash scripts/run-test-env.sh
$ cmake --preset release-docker
$ cmake --build --preset build-release-docker
```

## Test

### By vcpkg

```shell
$ bash scripts/run-test-env.sh
$ cmake --preset docker
$ cmake --build --preset build-debug
$ ctest --preset run-unit-tests
```

### By vcpkg (in docker)

```shell
$ bash scripts/run-test-env.sh
$ cmake --preset debug-docker
$ cmake --build --preset build-debug-docker
$ ctest --preset run-unit-tests-docker
```

## Install

```shell
$ cmake --install <build-dir> --prefix <destination-path>/jarvisto
```

Note: The project should be built previously.

## License

See the [LICENSE](LICENSE.md) file for license rights and limitations (MIT).