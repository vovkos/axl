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

THIS_DIR=`pwd`

mkdir build
pushd build
cmake .. -DTARGET_CPU=$TARGET_CPU -DCMAKE_BUILD_TYPE=$BUILD_CONFIGURATION
make
ctest --output-on-failure
popd

source ci/travis/get-coverage.sh

if [ "$BUILD_DOC" != "" ]; then
	mkdir doxyrest/build
	pushd doxyrest/build
	cmake .. -DTARGET_CPU=$TARGET_CPU -DCMAKE_BUILD_TYPE=$BUILD_CONFIGURATION
	make
	popd

	echo "set (DOXYREST_CMAKE_DIR $THIS_DIR/doxyrest/cmake $THIS_DIR/doxyrest/build/cmake)" >> paths.cmake

	# re-run cmake to include doxyrest

	pushd build
	cmake ..

	cd doc
	source index/build-html.sh
	source build-guide/build-html.sh
	source manual/build-xml.sh
	source manual/build-rst.sh
	source manual/build-html.sh

	touch html/.nojekyll

	popd
fi
