#!/usr/bin/env bash

if [ $BUILD_TYPE == "default" ]; then
    # Clone and build dependencies
    # Build and check this project
    ./autogen.sh && ./configure && make && make check && make memcheck && sudo make install
else
    cd ./builds/${BUILD_TYPE} && ./ci_build.sh
fi
