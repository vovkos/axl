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

Libraries
=========

All these libraries are **optional**. If they are missing, the corresponding module of AXL library will not be built.

* Lua

	AXL has two modules which require Lua:

		- ``axl_lua`` provides C++ wrappers for Lua library;
		- ``axl_st`` provides Lua-based string template module.

	Note that only Lua headers and libraries are needed (Lua executables are not necessary). If Lua library is not installed, neither ``axl_lua`` nor ``axl_st`` will be built.

	Lua executables, header files and both static and dynamic libraries are available in official repositories of most Linux distributions.

	Lua sources, as well as the links to pre-compiled binaries (of both executables and libraries), could be found at the official Lua website: http://www.lua.org

	Both 5.2.x and 5.3.x branches should be fine.

* Pcap/WinPcap

	Pcap (called WinPcap on Windows) library is required to build ``axl_io_pcap`` module (C++ wrappers for Pcap). If Pcap is missing, ``axl_io_pcap`` will not be built.

	Pcap library provides facilities for low-level network packet management including filtering, capturing and injecting network traffic.

	Pcap is part of Mac OS X SDK and is usually available out-of-the-box in many Linux distributions as well. On Windows systems sources, pre-compiled libraries and drivers could be downloaded from the official WinPcap website: http://www.winpcap.org

* LibUSB

	LibUSB is required to build ``axl_io_usb`` module (C++ wrappers for LibUSB). If LibUSB is missing, ``axl_io_usb`` will not be build.

	LibUSB is a library providing applications with access for controlling data transfer to and from USB devices on Unix and non-Unix systems, without the need for kernel-mode drivers.

	LibUSB is available in official repositories of most Linux distributions. Sources and Windows binaries could be downloaded from the official website: http://libusb.info

	Note that pre-compiled Windows libraries are built with /MD option (dynamic version of C runtime library). If you need to use LibUSB in a project built with /MT rather than /MD option, you need to re-compile LibUSB from sources.

* OpenSSL

	OpenSSL is required to build ``axl_cry`` module (C++ wrappers for OpenSSL in general and ECC in particular). If OpenSSL is missing, ``axl_cry`` will not be build.

	OpenSSL is available in official repositories of most Linux distributions. Alternatively, it can be built from sources available at the official website: http://www.openssl.org

	There is also a number of unofficial projects on the web offering pre-compiled libraries for OpenSSL.

* LibExpat

	LibExpat is required to build ``axl_xml`` module (C++ wrappers for LibExpat). If LibExpat is missing, ``axl_xml`` will not be build.

	LibExpat is an XML parser library written in C. It is a stream-oriented parser in which an application registers handlers for things the parser might find in the XML document (like start tags).

	LibExpat is available in official repositories of most Linux distributions. Sources and Windows binaries could be downloaded from the official project page: https://sourceforge.net/projects/expat

* QT 5.x

	QT is required to build ``axl_gui_qt`` module, which serves as a driver for ``axl_gui`` lightweight GUI abstraction framework.

	The idea behind this ``axl_gui`` abstraction framework is to allow writing simple widgets which could use QT, GTK or talk directly to the lower-level subsystems such as GDI or X11.

	Any QT version 5.x should be fine. However, newer versions of QT may not be compatible with Visual Studio 2010. Here in Tibbo, we use QT 5.3.2 to build official `IO Ninja <http://tibbo.com/ninja>`_.

	QT official download archive is available at: http://download.qt.io/archive/qt
