#!/bin/sh
OS=`uname`
if [ $OS = "Darwin" ]
then
	#osx
	ARMGCC_DIR=/usr/local/ cmake -DCMAKE_TOOLCHAIN_FILE="armgcc.cmake" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug .
else
	#ammdk boards
	ARMGCC_DIR=/usr/ cmake -DCMAKE_TOOLCHAIN_FILE="armgcc.cmake" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug .
fi
