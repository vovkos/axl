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

if [ $TRAVIS_OS_NAME == "windows" ]; then
	CMAKE_GENERATOR="Visual Studio 15 2017"

	if [ $TARGET_CPU == "amd64" ]; then
		CMAKE_GENERATOR="$CMAKE_GENERATOR Win64"
	fi

	cmake .. -G "$CMAKE_GENERATOR"

	echo "CMAKE_GENERATOR: $CMAKE_GENERATOR"
	echo +++++++++++++

	cmake --build . --config $BUILD_CONFIGURATION

	echo -------------

	ctest -C $BUILD_CONFIGURATION --output-on-failure
else
	cmake .. -DTARGET_CPU=$TARGET_CPU -DCMAKE_BUILD_TYPE=$BUILD_CONFIGURATION
	make
	ctest --output-on-failure
fi

popd

if [ "$GET_COVERAGE" != "" ]; then
	lcov --capture --directory . --no-external --output-file coverage.info
	lcov --list coverage.info

	curl -s https://codecov.io/bash | bash
fi

if [ "$BUILD_DOC" != "" ]; then
	echo "set (AXL_CMAKE_DIR $THIS_DIR/cmake $THIS_DIR/build/cmake)" >> paths.cmake

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
