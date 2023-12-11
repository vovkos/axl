#...............................................................................
#
#  This file is part of the AXL library.
#
#  AXL is distributed under the MIT license.
#  For details see accompanying license.txt file,
#  the public copy of which is also available at:
#  http://tibbo.com/downloads/archive/axl/license.txt
#
#...............................................................................

THIS_DIR=$(pwd)

mkdir re2s/.build
pushd re2s/.build

echo config: $1

cmake .. \
	-DCMAKE_INSTALL_PREFIX=$THIS_DIR/re2s/install \
	-DCMAKE_BUILD_TYPE=$1 \
	-DCMAKE_CXX_FLAGS=-fPIC \
	-DBUILD_SHARED_LIBS=OFF \
	-DRE2_BUILD_TESTING=OFF

make install
popd

echo set\(RE2S_CMAKE_DIR $THIS_DIR/re2s/install/lib/cmake/re2s\) >> paths.cmake

cat paths.cmake
