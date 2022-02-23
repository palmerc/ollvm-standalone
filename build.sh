#!/bin/bash

SRC_ROOT=$( pwd )
INSTALL_DIR=${SRC_ROOT}/install
BUILD_DIR=${SRC_ROOT}/build
LLVM_INSTALL_DIR=/opt/llvm/12.0.1

rm -rf ${BUILD_DIR} ${INSTALL_DIR}
mkdir -p ${BUILD_DIR}

pushd ${BUILD_DIR}

cmake \
    -G Ninja \
    -D CMAKE_VERBOSE_MAKEFILE=on \
    -D CMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
    -D LT_LLVM_INSTALL_DIR=${LLVM_INSTALL_DIR} \
    ${SRC_ROOT}

cmake --build .
cmake --build . --target install

popd

