#!/bin/bash

LUA_VERSION=5.2

CMAKE_VERSION=3.3.2
CMAKE_VERSION_DIR=v3.3
CMAKE_OS=Linux-x86_64
CMAKE_TAR=cmake-$CMAKE_VERSION-$CMAKE_OS.tar.gz
CMAKE_URL=http://www.cmake.org/files/$CMAKE_VERSION_DIR/$CMAKE_TAR
CMAKE_DIR=$(pwd)/cmake-$CMAKE_VERSION

wget --quiet $CMAKE_URL
mkdir -p $CMAKE_DIR
tar --strip-components=1 -xzf $CMAKE_TAR -C $CMAKE_DIR
export PATH=$CMAKE_DIR/bin:$PATH

sudo apt-get -qq update
sudo apt-get install -y liblua${LUA_VERSION}-dev
sudo apt-get install -y libpcap-dev
sudo apt-get install -y libudev-dev
sudo apt-get install -y libusb-1.0
