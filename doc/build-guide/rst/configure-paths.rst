.. .............................................................................
..
..  This file is part of the AXL library.
..
..  AXL is distributed under the MIT license.
..  For details see accompanying license.txt file,
..  the public copy of which is also available at:
..  http://tibbo.com/downloads/archive/axl/license.txt
..
.. .............................................................................

paths.cmake
===========

AXL build system uses ``paths.cmake`` file as the main reference when it needs to find a certain tool or library. When a path is not specified, a fallback attempt to find it using ``find_package`` will be made.

This allows for out-of-the-box *default* build and at the same time provides a *fine-grained* control over locations of dependencies. Here in Tibbo, we have multiple versions of tools and libraries installed on the single build machine and at the same time, we are always in full control over which tool or library is going to be used when building a particular project.

``paths.cmake`` files are **cascading**. It means, you can place one *anywhere* above the current directory and it will be found and used. From there you can chain-include the next ``paths.cmake`` and so on. This way you can specify some default locations for *all* your projects but still be able to override the paths for sub-projects.

Being machine-specific ``paths.cmake`` files are added to ``.gitignore`` and are never tracked in Git. Therefore, you need to write ``paths.cmake`` file as the very first step of the configuration process. So, what should be inside?

To answer this question, you need to check ``dependencies.cmake`` file. Inside this file, a variable called ``AXL_PATH_LIST`` contains all the paths that will be used during the build:

.. code-block:: bash

	LUA_INC_DIR         # (optional) path to Lua C include directory
	LUA_LIB_DIR         # (optional) path to Lua library directory
	LUA_LIB_NAME        # (optional) name of Lua library (lua/lua51/lua52/lua53)
	PCAP_INC_DIR        # (optional) path to Pcap include directory
	PCAP_LIB_DIR        # (optional) path to Pcap library directory
	LIBUSB_INC_DIR      # (optional) path to LibUSB include directory
	LIBUSB_LIB_DIR      # (optional) path to LibUSB library directory
	LIBSSH2_INC_DIR     # (optional) path to LibSSH2 include directory
	LIBSSH2_LIB_DIR     # (optional) path to LibSSH2 library directory
	OPENSSL_INC_DIR     # (optional) path to OpenSSL include directory
	OPENSSL_LIB_DIR     # (optional) path to OpenSSL library directory
	EXPAT_INC_DIR       # (optional) path to Expat include directory
	EXPAT_LIB_DIR       # (optional) path to Expat library directory
	QT_CMAKE_DIR        # (optional) path to QT CMake module directory
	QT_DLL_DIR          # (optional) path to QT dynamic library directory (Windows only)
	RAGEL_EXE           # (optional) path to Ragel executable
	7Z_EXE              # (optional) path to 7-Zip executable
	DOXYGEN_EXE         # (optional) path to Doxygen executable
	DOXYREST_CMAKE_DIR  # (optional) path to Doxyrest CMake module directory
	SPHINX_BUILD_EXE    # (optional) path to Sphinx compiler executable sphinx-build
	PDFLATEX_EXE        # (optional) path to Latex-to-PDF compiler

Note that dependencies may be auto-detected -- on Unix systems installed libraries and tools will likely be found automatically. On Windows AXL build system will automatically find executables if they are added to ``PATH`` (via ``where`` command).

On Windows, you will also need to specify paths to the libraries -- they are unlikely to be found automatically.

And of course, you can always use ``paths.cmake`` to *fine-tune* the location of a specific tool/library.

I personally prefer to always specify all the paths explicitly.

.. rubric:: Sample paths.cmake on Windows:

.. code-block:: cmake

	set (LUA_VERSION     5.2.1)
	set (LUA_LIB_NAME    lua52)
	set (LIBUSB_VERSION  1.0.19)
	set (OPENSSL_VERSION 1.0.1h)
	set (EXPAT_VERSION   2.1.0)
	set (WINPCAP_VERSION 4.1.2)
	set (QT_VERSION      5.3.2)
	set (RAGEL_VERSION   6.7)
	set (DOXYGEN_VERSION 1.8.11)

	set (LUA_INC_DIR         c:/Develop/lua/lua-${LUA_VERSION}/include)
	set (PCAP_INC_DIR        c:/Develop/winpcap/winpcap-${WINPCAP_VERSION}/include)
	set (LIBUSB_INC_DIR      c:/Develop/libusb/libusb-${LIBUSB_VERSION}/libusb)
	set (EXPAT_INC_DIR       c:/Develop/expat/expat-${EXPAT_VERSION}/lib)

	if ("${AXL_CPU}" STREQUAL "amd64")
		set (LUA_LIB_DIR        c:/Develop/lua/lua-${LUA_VERSION}/lib/amd64/${CONFIGURATION_SUFFIX})
		set (PCAP_LIB_DIR       c:/Develop/winpcap/winpcap-${WINPCAP_VERSION}/lib/x64)
		set (LIBUSB_LIB_DIR     c:/Develop/libusb/libusb-${LIBUSB_VERSION}/x64/${CONFIGURATION_SUFFIX}/lib)
		set (OPENSSL_INC_DIR    c:/Develop/openssl/openssl-win64-${OPENSSL_VERSION}/include)
		set (OPENSSL_LIB_DIR    c:/Develop/openssl/openssl-win64-${OPENSSL_VERSION}/lib/vc/static)
		set (EXPAT_LIB_DIR      c:/Develop/expat/expat-${EXPAT_VERSION}/build/msvc10-amd64/${CONFIGURATION_SUFFIX})
		set (QT_CMAKE_DIR       e:/Develop/qt/qt-${QT_VERSION}/build/amd64/qtbase/lib/cmake)
		set (QT_DLL_DIR         e:/Develop/qt/qt-${QT_VERSION}/build/amd64/qtbase/lib)
	else ()
		set (LUA_LIB_DIR        c:/Develop/lua/lua-${LUA_VERSION}/lib/x86/${CONFIGURATION_SUFFIX})
		set (PCAP_LIB_DIR       c:/Develop/winpcap/winpcap-${WINPCAP_VERSION}/lib)
		set (LIBUSB_LIB_DIR     c:/Develop/libusb/libusb-${LIBUSB_VERSION}/Win32/${CONFIGURATION_SUFFIX}/lib)
		set (OPENSSL_INC_DIR    c:/Develop/openssl/openssl-win32-${OPENSSL_VERSION}/include)
		set (OPENSSL_LIB_DIR    c:/Develop/openssl/openssl-win32-${OPENSSL_VERSION}/lib/vc/static)
		set (EXPAT_LIB_DIR      c:/Develop/expat/expat-${EXPAT_VERSION}/build/msvc10/${CONFIGURATION_SUFFIX})
		set (QT_CMAKE_DIR       e:/Develop/qt/qt-${QT_VERSION}/build/x86/qtbase/lib/cmake)
		set (QT_DLL_DIR         e:/Develop/qt/qt-${QT_VERSION}/build/x86/qtbase/lib)
	endif()

	set (7Z_EXE           "c:/Program Files/7-Zip/7z.exe")
	set (RAGEL_EXE        c:/Develop/ragel/ragel-${RAGEL_VERSION}/ragel.exe)
	set (PDFLATEX_EXE     "c:/Program Files (x86)/MiKTeX 2.9/miktex/bin/pdflatex.exe")
	set (DOXYGEN_EXE      c:/Develop/doxygen/doxygen-${DOXYGEN_VERSION}-x86/doxygen.exe)
	set (SPHINX_BUILD_EXE c:/Develop/Python3/Scripts/sphinx-build.exe)

