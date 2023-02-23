#!/bin/bash
set -e

declare -a versions=("18.04" "20.04" "22.04" "22.10" "23.04")
declare -a archs=("amd64" "armv7" "aarch64")

for version in ${versions[@]}; do
  for arch in ${archs[@]}; do
    docker build -t pavignol/cpp-kzg-ceremony-client:ubuntu-$version-$arch-build -f containers/ubuntu-$version-$arch-build.Dockerfile .
    docker image push pavignol/cpp-kzg-ceremony-client:ubuntu-$version-$arch-build
  done
done
