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

.. define-macro:: lua-common-info

	Lua executables, header files and both static and dynamic libraries are available in official repositories of most Linux distributions.

	Lua sources, as well as the links to pre-compiled binaries (of both executables and libraries), could be found at the official Lua website: http://www.lua.org

	Both 5.2.x and 5.3.x branches should be fine.

.. define-macro:: pcap-common-info

	Pcap library provides facilities for low-level network packet management including filtering, capturing and injecting network traffic.

	Pcap is part of Mac OS X SDK and is usually available out-of-the-box in many Linux distributions as well. On Windows systems sources, pre-compiled libraries and drivers could be downloaded from the official WinPcap website: http://www.winpcap.org

.. define-macro:: libusb-common-info

	LibUSB is a library providing applications with access for controlling data transfer to and from USB devices on Unix and non-Unix systems, without the need for kernel-mode drivers.

	LibUSB is available in official repositories of most Linux distributions. Sources and Windows binaries could be downloaded from the official website: http://libusb.info

	Note that pre-compiled Windows libraries are built with /MD option (dynamic version of C runtime library). If you need to use LibUSB in a project built with /MT rather than /MD option, you need to re-compile LibUSB from sources.

.. define-macro:: openssl-common-info

	OpenSSL is available in official repositories of most Linux distributions. Alternatively, it can be built from sources available at the official website: http://www.openssl.org

	There is also a number of unofficial projects on the web offering pre-compiled libraries for OpenSSL.

.. define-macro:: expat-common-info

	LibExpat is an XML parser library written in C. It is a stream-oriented parser in which an application registers handlers for things the parser might find in the XML document (like start tags).

	LibExpat is available in official repositories of most Linux distributions. Sources and Windows binaries could be downloaded from the official project page: https://sourceforge.net/projects/expat

.. define-macro:: qt-common-info

	Any QT version 5.x should be fine. However, newer versions of QT may not be compatible with Visual Studio 2010. Here in Tibbo, we use QT 5.4.2 to build the official packages of `IO Ninja <http://tibbo.com/ninja>`_.

	QT official download archive is available at: http://download.qt.io/archive/qt

.. define-macro:: axl-common-info

	AXL is a lightweight C++ library featuring:

	* Java naming convention;
	* Non-zero-terminated UTF-8 string slices as the default string-passing model;
	* TLS-based error-handling system;
	* Unified reference-counting model;
	* Wrappers for many popular libraries;
	* And more...

	Sources of AXL are available at: http://tibbo.com/downloads/archive/axl

.. define-macro:: axl-bundle-info project package package_b

	.. expand-macro:: axl-common-info

	AXL is included in the recommended ``$package_b`` bundle package. Therefore, if you build $project from ``$package_b`` package, you don't have to do anything extra -- AXL will be built automatically during the build process.

	However, if you use the standalone ``$package`` package, you will need to download and build AXL first.
