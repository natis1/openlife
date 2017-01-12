#!/bin/bash

# This program will automatically build life with normal defaults
#
# Please be sure to install sfml and cmake. The readme should explain everything.
#

# Do not move build.sh. You can execute it from anywhere though.
cd $(dirname $0)

SRC_PATH=$(pwd)
BUILD_PATH="$SRC_PATH/build"
EXE_NAME="openlifecli"
TEST_NAME="openlifetests"
LIB_NAME="libopenlife.so"
GUI_NAME="openlifegui"

rm -r "$BUILD_PATH"
mkdir "$BUILD_PATH"
cd "$BUILD_PATH"

#TODO: add any possible cmake variables here
cmake "$SRC_PATH" -DCMAKE_BUILD_TYPE=DEBUG

make -j $(nproc) $@
chmod a+x "$EXE_NAME"
cp "$EXE_NAME" ..
cp "$LIB_NAME" ..
cp "$GUI_NAME" ..
chmod a+x "$TEST_NAME"
cp "$TEST_NAME" ..

#rm -r "$BUILD_PATH"
