::..............................................................................
::
::  This file is part of the AXL library.
::
::  AXL is distributed under the MIT license.
::  For details see accompanying license.txt file,
::  the public copy of which is also available at:
::  http://tibbo.com/downloads/archive/axl/license.txt
::
::..............................................................................

@echo off

:: . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

set THIS_DIR=%CD%
set THIS_DIR_CMAKE=%THIS_DIR:\=/%

mkdir re2s\.build
cd re2s\.build

cmake .. ^
	-DCMAKE_INSTALL_PREFIX=%THIS_DIR_CMAKE%/re2s/install ^
	-DBUILD_SHARED_LIBS=OFF ^
	-DRE2_BUILD_TESTING=OFF ^
	|| exit

cmake --build . --target install %CMAKE_BUILD_FLAGS% || exit

cd %THIS_DIR%
echo set(RE2S_CMAKE_DIR %THIS_DIR_CMAKE%/re2s/install/lib/cmake/re2) >> paths.cmake

:: . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
