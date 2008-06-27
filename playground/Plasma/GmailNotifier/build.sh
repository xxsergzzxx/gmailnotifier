#!/bin/bash

function msg {
    echo -e "\n$1"
}

[ ! -d Build ] && mkdir Build
cd Build

msg "Running CMake..."
cmake .. -DCMAKE_INSTALL_PREFIX=$KDEDIR -DCMAKE_BUILD_TYPE=debugfull

msg "Compiling..."
make

msg "Installing..."
make install
