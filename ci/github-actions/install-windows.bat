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

:: Ragel

mkdir %DOWNLOAD_DIR%\ragel
appveyor DownloadFile %RAGEL_DOWNLOAD_URL% -FileName %DOWNLOAD_DIR%\ragel.exe

echo set (RAGEL_EXE %DOWNLOAD_DIR_CMAKE%/ragel.exe) >> paths.cmake

:: . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

:: Lua

mkdir %DOWNLOAD_DIR%\lua
appveyor DownloadFile %LUA_DOWNLOAD_URL% -FileName %DOWNLOAD_DIR%\lua\lua.zip
7z x -y %DOWNLOAD_DIR%\lua\lua.zip -o%DOWNLOAD_DIR%\lua

echo set (LUA_INC_DIR %DOWNLOAD_DIR_CMAKE%/lua/include) >> paths.cmake
echo set (LUA_LIB_DIR %DOWNLOAD_DIR_CMAKE%/lua) >> paths.cmake
echo set (LUA_DLL_DIR %DOWNLOAD_DIR_CMAKE%/lua) >> paths.cmake
echo set (LUA_LIB_NAME %LUA_LIB_NAME%) >> paths.cmake

:: . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

:: OpenSSL

set OPENSSL_DIR_CMAKE=%OPENSSL_DIR:\=/%

echo set (OPENSSL_INC_DIR %OPENSSL_DIR_CMAKE%/include) >> paths.cmake
echo set (OPENSSL_LIB_DIR %OPENSSL_DIR_CMAKE%/lib) >> paths.cmake
echo set (OPENSSL_DLL_DIR %OPENSSL_DIR_CMAKE%/bin) >> paths.cmake
echo set (OPENSSL_CRYPTO_LIB_NAME libeay32) >> paths.cmake
echo set (OPENSSL_CRYPTO_DLL_NAME libeay32) >> paths.cmake
echo set (OPENSSL_SSL_LIB_NAME ssleay32) >> paths.cmake
echo set (OPENSSL_SSL_DLL_NAME ssleay32) >> paths.cmake

:: . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

:: Get rid of annoying Xamarin build warnings

if exist "c:\Program Files (x86)\MSBuild\14.0\Microsoft.Common.targets\ImportAfter\Xamarin.Common.targets" (
	del "c:\Program Files (x86)\MSBuild\14.0\Microsoft.Common.targets\ImportAfter\Xamarin.Common.targets"
)

if exist "c:\Program Files (x86)\MSBuild\4.0\Microsoft.Common.targets\ImportAfter\Xamarin.Common.targets" (
	del "c:\Program Files (x86)\MSBuild\4.0\Microsoft.Common.targets\ImportAfter\Xamarin.Common.targets"
)
