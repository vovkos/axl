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

	.. expand-macro:: lua-common-info

* Pcap/WinPcap

	Pcap (called WinPcap on Windows) library is required to build ``axl_io_pcap`` module (C++ wrappers for Pcap). If Pcap is missing, ``axl_io_pcap`` will not be built.

	.. expand-macro:: pcap-common-info

* LibUSB

	LibUSB is required to build ``axl_io_usb`` module (C++ wrappers for LibUSB). If LibUSB is missing, ``axl_io_usb`` will not be build.

	.. expand-macro:: libusb-common-info

* OpenSSL

	OpenSSL is required to build ``axl_cry`` module (C++ wrappers for OpenSSL in general and ECC in particular). If OpenSSL is missing, ``axl_cry`` will not be build.

	.. expand-macro:: openssl-common-info

* Expat

	Expat is required to build ``axl_xml`` module (C++ wrappers for Expat). If Expat is missing, ``axl_xml`` will not be build.

	.. expand-macro:: expat-common-info

* QT 5.x

	QT is required to build ``axl_gui_qt`` module, which serves as a driver for ``axl_gui`` lightweight GUI abstraction framework.

	The idea behind this ``axl_gui`` abstraction framework is to allow writing simple widgets which could use QT, GTK or talk directly to the lower-level subsystems such as GDI or X11.

	.. expand-macro:: qt-common-info
