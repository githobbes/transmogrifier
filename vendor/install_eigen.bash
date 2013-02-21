#!/usr/bin/env bash

# This script installs Eigen into the vendor directory.

set -o errexit
set -o nounset
set -o xtrace

readonly PREFIX=$PWD
readonly EIGEN_VERSION=3.1.2
# This name comes from the directory inside the tar file.
readonly EIGEN_SRC_DIR=eigen-eigen-5097c01bcdc4
readonly EIGEN_TAR_FILE=eigen-$EIGEN_VERSION.tar.bz2
readonly EIGEN_BUILD_DIR=eigen-build

cd /tmp
wget "http://bitbucket.org/eigen/eigen/get/$EIGEN_VERSION.tar.bz2" -O "$EIGEN_TAR_FILE"
tar -xf "$EIGEN_TAR_FILE"
rm "$EIGEN_TAR_FILE"
mkdir "$EIGEN_BUILD_DIR"
cd "$EIGEN_BUILD_DIR"
cmake -DCMAKE_INSTALL_PREFIX="$PREFIX" "../$EIGEN_SRC_DIR"
make install
cd /tmp
rm -r "$EIGEN_SRC_DIR" "$EIGEN_BUILD_DIR"
