@rem ...........................................................................
@rem
@rem  This file is part of the AXL library.
@rem
@rem  AXL is distributed under the MIT license.
@rem  For details see accompanying license.txt file,
@rem  the public copy of which is also available at:
@rem  http://tibbo.com/downloads/archive/axl/license.txt
@rem
@rem ...........................................................................

@echo off

:loop

if "%1" == "" goto :finalize
if /i "%1" == "x86" goto :x86
if /i "%1" == "i386" goto :x86
if /i "%1" == "amd64" goto :amd64
if /i "%1" == "x86_64" goto :amd64
if /i "%1" == "x64" goto :amd64

echo Invalid argument: '%1'
exit -1

:x86
set TARGET_CPU=x86
set CMAKE_GENERATOR_SUFFIX=
shift
goto :loop

:amd64
set TARGET_CPU=amd64
set CMAKE_GENERATOR_SUFFIX= Win64
shift
goto :loop

:cpu

if /i "%PROCESSOR_ARCHITECTURE%" == "x86" goto :x86
if /i "%PROCESSOR_ARCHITECTURE%" == "i386" goto :x86
if /i "%PROCESSOR_ARCHITECTURE%" == "amd64" goto :amd64
if /i "%PROCESSOR_ARCHITECTURE%" == "x86_64" goto :amd64
if /i "%PROCESSOR_ARCHITECTURE%" == "x64" goto :amd64

echo Unsupported native architecture: '%PROCESSOR_ARCHITECTURE%'
exit -1

:finalize

if "%TARGET_CPU%" == "" goto :cpu

set CMAKE_FLAGS=-G "Visual Studio 14 2015%CMAKE_GENERATOR_SUFFIX%" -DTARGET_CPU=%TARGET_CPU%

echo CMAKE_FLAGS = %CMAKE_FLAGS%
