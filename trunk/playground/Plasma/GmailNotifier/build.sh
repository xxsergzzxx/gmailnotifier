#!/bin/bash

function msg {
    echo -e "\n$1"
}

KDE4PREFIX="$(kde4-config --prefix)"

[ ! -d Build ] && mkdir Build
cd Build

msg "Running CMake..."
cmake .. -DCMAKE_INSTALL_PREFIX=${KDEDIR:=$KDE4PREFIX} -DCMAKE_BUILD_TYPE=Debugfull

msg "Compiling..."
make

msg "Installing..."
if [ ${KDE4PREFIX:0:${#HOME}} == $HOME ]; then
    make install
else
    sudo make install
fi
