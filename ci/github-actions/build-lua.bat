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

mkdir lua\.build
cd lua\.build

cmake .. ^
	-DCMAKE_INSTALL_PREFIX=%THIS_DIR_CMAKE%/lua/install ^
	-DLUA_BUILD_BINARY=OFF ^
	-DLUA_BUILD_COMPILER=OFF ^
	-DLUA_ENABLE_SHARED=ON ^
	-DLUA_ENABLE_TESTING=OFF ^
	%CMAKE_CONFIGURE_FLAGS% ^
	|| exit

cmake ^
	--build . ^
	--target install %CMAKE_BUILD_FLAGS% ^
	|| exit

cd %THIS_DIR%

echo set(LUA_INC_DIR %THIS_DIR_CMAKE%/lua/install/include) >> paths.cmake
echo set(LUA_LIB_DIR %THIS_DIR_CMAKE%/lua/install/lib) >> paths.cmake
echo set(LUA_LIB_NAME lua_shared) >> paths.cmake
echo set(LUA_DLL_DIR %THIS_DIR_CMAKE%/lua/install/bin) >> paths.cmake

:: . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
