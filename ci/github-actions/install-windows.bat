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

set DOWNLOAD_DIR=c:\downloads
set DOWNLOAD_DIR_CMAKE=%DOWNLOAD_DIR:\=/%

:: . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

:: Dependency versions

set EXPAT_VERSION=2.7.5
set EXPAT_VERSION_TAG=R_2_7_5
set EXPAT_DOWNLOAD_FILE=expat-%EXPAT_VERSION%.tar.gz
set EXPAT_DOWNLOAD_URL=https://github.com/libexpat/libexpat/releases/download/%EXPAT_VERSION_TAG%/%EXPAT_DOWNLOAD_FILE%

set LUA_VERSION=5.4.7
set LUA_DOWNLOAD_URL=https://github.com/walterschell/Lua/archive/refs/tags/v%LUA_VERSION%.zip

set OPENSSL_VERSION=1.1.1.2100

set RAGEL_DOWNLOAD_URL=https://github.com/eloraiby/ragel-windows/raw/master/ragel.exe

:: . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

:: wget

echo Installing wget...

choco install wget --no-progress || exit

:: . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

:: Ragel

echo Downloading %RAGEL_DOWNLOAD_URL%...

mkdir %DOWNLOAD_DIR%
wget -nv %RAGEL_DOWNLOAD_URL% -O %DOWNLOAD_DIR%\ragel.exe || exit

echo set(RAGEL_EXE %DOWNLOAD_DIR_CMAKE%/ragel.exe) >> paths.cmake

:: . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

:: Lua (CMake-based)

echo Downloading %LUA_DOWNLOAD_URL%...

mkdir %DOWNLOAD_DIR%\lua
wget -nv %LUA_DOWNLOAD_URL% -O %DOWNLOAD_DIR%\lua\lua.zip || exit
7z x -y %DOWNLOAD_DIR%\lua\lua.zip || exit
ren lua-%LUA_VERSION% lua || exit

:: . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

:: Expat

wget -nv %EXPAT_DOWNLOAD_URL% -O %DOWNLOAD_DIR%\%EXPAT_DOWNLOAD_FILE% || exit
7z x -y %DOWNLOAD_DIR%\%EXPAT_DOWNLOAD_FILE% -o%DOWNLOAD_DIR% || exit
7z x -y %DOWNLOAD_DIR%\expat-%EXPAT_VERSION%.tar || exit
ren expat-%EXPAT_VERSION% expat || exit

:: . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

:: OpenSSL

if /i "%TARGET_CPU%" == "arm64" goto :openssl_arm64
if /i "%TARGET_CPU%" == "x86" goto :openssl_x86

set OPENSSL_DIR_SUFFIX=
set OPENSSL_DLL_SUFFIX=-1_1-x64
goto :openssl_install

:openssl_x86
set OPENSSL_DIR_SUFFIX=-Win32
set OPENSSL_DLL_SUFFIX=-1_1
goto :openssl_install

:: choco doesn't ship arm64 builds; on arm64 we use preinstalled OpenSSL 3
:: (libs are at lib/VC/<arch>/<crt>)

:openssl_arm64
set OPENSSL_DLL_SUFFIX=-3-arm64
set OPENSSL_ROOT_DIR=C:/Program Files/OpenSSL

if /i "%CONFIGURATION%" == "Debug" (
	set OPENSSL_LIB_SUBDIR=lib/VC/arm64/MDd
) else (
	set OPENSSL_LIB_SUBDIR=lib/VC/arm64/MD
)

goto :openssl_finalize

:openssl_install

echo Installing OpenSSL %OPENSSL_VERSION%...

choco install openssl --version=%OPENSSL_VERSION% --no-progress %CHOCO_PLATFORM% || exit

set OPENSSL_ROOT_DIR=C:/Program Files%PROGRAM_FILES_DIR_SUFFIX%/OpenSSL%OPENSSL_DIR_SUFFIX%
set OPENSSL_LIB_SUBDIR=lib

:openssl_finalize

echo set(OPENSSL_INC_DIR "%OPENSSL_ROOT_DIR%/include") >> paths.cmake
echo set(OPENSSL_LIB_DIR "%OPENSSL_ROOT_DIR%/%OPENSSL_LIB_SUBDIR%") >> paths.cmake
echo set(OPENSSL_DLL_DIR "%OPENSSL_ROOT_DIR%/bin") >> paths.cmake
echo set(OPENSSL_CRYPTO_LIB_NAME libcrypto) >> paths.cmake
echo set(OPENSSL_CRYPTO_DLL_NAME libcrypto%OPENSSL_DLL_SUFFIX%) >> paths.cmake
echo set(OPENSSL_SSL_LIB_NAME libssl) >> paths.cmake
echo set(OPENSSL_SSL_DLL_NAME libssl%OPENSSL_DLL_SUFFIX%) >> paths.cmake

:: . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
