# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2013. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................

add_subdirectory (axl_core)

#..............................................................................

option (
	BUILD_AXL_EXE
	"Build axl_exe library"
	OFF
	)

if (BUILD_AXL_EXE)
	add_subdirectory (axl_exe)
endif ()

#..............................................................................

if (WIN32)
	option (
		BUILD_AXL_GUI_GDI
		"Build GDI drivers for axl_gui library"
		OFF
		)
endif ()

if (${QT_FOUND})
	option (
		BUILD_AXL_GUI_QT
		"Build QT drivers for axl_gui library"
		ON
		)
else ()
	unset (BUILD_AXL_GUI_QT CACHE)
endif ()

option (
	BUILD_AXL_GUI
	"Build axl_gui library"
	ON
	)

if (BUILD_AXL_GUI)
	add_subdirectory (axl_gui)
endif ()

#..............................................................................

if (${RAGEL_FOUND})
	option (
		BUILD_AXL_INI
		"Build axl_ini library"
		ON
		)
else ()
	unset (BUILD_AXL_INI CACHE)
endif ()

if (BUILD_AXL_INI)
	add_subdirectory (axl_ini)
endif ()

#..............................................................................

option (
	BUILD_AXL_IO
	"Build axl_io library"
	ON
	)

if (${PCAP_FOUND})
	option (
		BUILD_AXL_IO_PCAP
		"Build PCap wrappers for axl_io library"
		ON
		)
else ()
	unset (BUILD_AXL_IO_PCAP CACHE)
endif ()

if (${LIBUSB_FOUND})
	option (
		BUILD_AXL_IO_USB
		"Build LibUSB wrappers for axl_io library"
		ON
		)
else ()
	unset (BUILD_AXL_IO_USB CACHE)
endif ()

if (BUILD_AXL_IO)
	add_subdirectory (axl_io)
endif ()

#..............................................................................

if (${RAGEL_FOUND} AND ${LUA_FOUND})
	option (
		BUILD_AXL_LUA
		"Build axl_lua library"
		ON
		)
else ()
	unset (BUILD_AXL_LUA CACHE)
endif ()

if (BUILD_AXL_LUA)
	add_subdirectory (axl_lua)
endif ()

#..............................................................................

option (
	BUILD_AXL_ZIP
	"Build axl_zip library (miniz wrapper)"
	ON
	)

if (BUILD_AXL_ZIP)
	add_subdirectory (axl_zip)
endif ()

#..............................................................................

if (${OPENSSL_FOUND})
	option (
		BUILD_AXL_CRY
		"Build axl_cry library"
		ON
		)
else ()
	unset (BUILD_AXL_CRY CACHE)
endif ()

if (BUILD_AXL_CRY)
	add_subdirectory (axl_cry)
endif ()

#..............................................................................
