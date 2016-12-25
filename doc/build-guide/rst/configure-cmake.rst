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

CMake Configuration Step
========================

.. expand-macro:: cmake-configure axl

.. rubric:: Sample Linux output:

::

	AXL CMake:
	    Invoked from:        /home/vladimir/Projects/ioninja/axl/CMakeLists.txt
	    dependencies.cmake:  /home/vladimir/Projects/ioninja/axl/dependencies.cmake
	    settings.cmake:      /home/vladimir/Projects/ioninja/axl/settings.cmake
	    paths.cmake:         /home/vladimir/Projects/paths.cmake
	    Target CPU:          amd64
	    Build configuration: Debug

.. rubric:: Sample Windows output:

::

	AXL CMake:
	    Invoked from:        C:/Projects/repos/ioninja/axl/CMakeLists.txt
	    dependencies.cmake:  C:/Projects/repos/ioninja/axl/dependencies.cmake
	    paths.cmake:         C:/Projects/repos/ioninja/paths.cmake
	    Target CPU:          amd64
	C/C++:
	    C Compiler:          C:/Develop/msvs/msvs2010/VC/bin/x86_amd64/cl.exe
	    C flags (Debug):     /DWIN32 /D_WINDOWS /W3 /EHsc /GR /D_DEBUG /Ob0 /Od /RTC1 /MTd /Zi
	    C flags (Release):   /DWIN32 /D_WINDOWS /W3 /EHsc /GR /O2 /Ob2 /D NDEBUG /MT /Zi
	    C++ Compiler:        C:/Develop/msvs/msvs2010/VC/bin/x86_amd64/cl.exe
	    C++ flags (Debug):   /DWIN32 /D_WINDOWS /W3 /EHsc /GR /D_DEBUG /Ob0 /Od /RTC1 /MTd /Zi
	    C++ flags (Release): /DWIN32 /D_WINDOWS /W3 /EHsc /GR /O2 /Ob2 /D NDEBUG /MT /Zi
	    C/C++ definitions:   UNICODE;_UNICODE
	Dependency path definitions:
	    7Z_EXE:              c:/Program Files/7-Zip/7z.exe
	    DOXYGEN_EXE:         c:/Develop/doxygen/doxygen-1.8.11-x86/doxygen.exe
	    DOXYREST_CMAKE_DIR:  C:/Projects/repos/ioninja/doxyrest/build/msvc10-amd64/cmake
	    EXPAT_INC_DIR:       c:/Develop/expat/expat-2.1.0/lib
	    EXPAT_LIB_DIR:       c:/Develop/expat/expat-2.1.0/build/msvc10-amd64/$(Configuration)
	    LIBSSH2_INC_DIR:     c:/Develop/libssh2/libssh2-1.4.4/include
	    LIBSSH2_LIB_DIR:     c:/Develop/libssh2/libssh2-1.4.4/lib/amd64/$(Configuration)
	    LIBUSB_INC_DIR:      c:/Develop/libusb/libusb-1.0.19/include
	    LIBUSB_LIB_DIR:      c:/Develop/libusb/libusb-1.0.19/x64/$(Configuration)/lib
	    LUA_INC_DIR:         c:/Develop/lua/lua-5.2.1/include
	    LUA_LIB_DIR:         c:/Develop/lua/lua-5.2.1/lib/amd64/$(Configuration)
	    LUA_LIB_NAME:        lua52
	    OPENSSL_INC_DIR:     c:/Develop/openssl/openssl-win64-1.0.1h/include
	    OPENSSL_LIB_DIR:     c:/Develop/openssl/openssl-win64-1.0.1h/lib/vc/static
	    PCAP_INC_DIR:        c:/Develop/winpcap/winpcap-4.1.2/include
	    PCAP_LIB_DIR:        c:/Develop/winpcap/winpcap-4.1.2/lib/x64
	    PDFLATEX_EXE:        c:/Program Files (x86)/MiKTeX 2.9/miktex/bin/pdflatex.exe
	    QT_CMAKE_DIR:        e:/Develop/qt/qt-5.3.2/build/amd64/qtbase/lib/cmake
	    QT_DLL_DIR:          e:/Develop/qt/qt-5.3.2/build/amd64/qtbase/lib
	    RAGEL_EXE:           c:/Develop/ragel/ragel-6.7/ragel.exe
	    SPHINX_BUILD_EXE:    c:/Develop/Python3/Scripts/sphinx-build.exe
	Lua paths:
	    Includes:            c:/Develop/lua/lua-5.2.1/include
	    Library dir:         c:/Develop/lua/lua-5.2.1/lib/amd64/$(Configuration)
	    Library name:        lua52
	Pcap paths:
	    Includes:            c:/Develop/winpcap/winpcap-4.1.2/include
	    Libraries:           c:/Develop/winpcap/winpcap-4.1.2/lib/x64
	LibUSB paths:
	    Includes:            c:/Develop/libusb/libusb-1.0.19/include
	    Libraries:           c:/Develop/libusb/libusb-1.0.19/x64/$(Configuration)/lib
	OpenSSL paths:
	    Includes:            c:/Develop/openssl/openssl-win64-1.0.1h/include
	    Libraries:           c:/Develop/openssl/openssl-win64-1.0.1h/lib/vc/static
	Expat paths:
	    Includes:            c:/Develop/expat/expat-2.1.0/lib
	    Libraries:           c:/Develop/expat/expat-2.1.0/build/msvc10-amd64/$(Configuration)
	QT paths:
	    Core CMake files:    e:/Develop/qt/qt-5.3.2/build/amd64/qtbase/lib/cmake/Qt5Core
	    Gui CMake files:     e:/Develop/qt/qt-5.3.2/build/amd64/qtbase/lib/cmake/Qt5Gui
	    Widgets CMake files: e:/Develop/qt/qt-5.3.2/build/amd64/qtbase/lib/cmake/Qt5Widgets
	    Network CMake files: e:/Develop/qt/qt-5.3.2/build/amd64/qtbase/lib/cmake/Qt5Network
	doxyrest:                NOT FOUND, adjust AXL_IMPORT_DIR_LIST in dependencies.cmake (optional)
	Configuring done

.. expand-macro:: cmake-post-configure
