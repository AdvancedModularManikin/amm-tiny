#!/bin/sh
#ARMGCC_DIR=~/code/arm-gcc-toolchain/gcc-arm-none-eabi-5_4-2016q3/ cmake .
#osx
#export ARMGCC_DIR=~/code/gcc-arm-none-eabi-6-2017-q1-update 
#cmake -DCMAKE_TOOLCHAIN_FILE="armgcc.cmake" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug  .

#ammdk boards
ARMGCC_DIR=/usr/ cmake -DCMAKE_TOOLCHAIN_FILE="armgcc.cmake" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug  .
