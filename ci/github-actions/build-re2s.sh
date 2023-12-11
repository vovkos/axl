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

CONFIGURATION=$1
THIS_DIR=$(pwd)
CXX_FLAGS=-fPIC

mkdir re2s/.build
pushd re2s/.build

echo config: $1

if [[ "$TARGET_CPU" == "x86" ]]; then
	CXX_FLAGS=-m32 $CXX_FLAGS
fi

cmake .. \
	-DCMAKE_INSTALL_PREFIX=$THIS_DIR/re2s/install \
	-DCMAKE_BUILD_TYPE=$CONFIGURATION \
	-DCMAKE_CXX_FLAGS="$CXX_FLAGS" \
	-DBUILD_SHARED_LIBS=OFF \
	-DRE2_BUILD_TESTING=OFF

make install
popd

echo "set(RE2S_CMAKE_DIR $THIS_DIR/re2s/install/lib/cmake/re2)" >> paths.cmake
echo "axl_override_setting(GCC_FLAG_CPP_STANDARD -std=c++11)" >> settings.cmake
