# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2013. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................

root_virtual_folder (${CMAKE_CURRENT_BINARY_DIR}/src.virtual)

add_subdirectory (axl_core)

#..............................................................................

option (
	AXL_BUILD_EXE_LIB
	"Build axl_exe library"
	OFF
	)

if (AXL_BUILD_EXE_LIB)
	add_subdirectory (axl_exe)
endif ()

#..............................................................................

if (WIN32)
	option (
		AXL_BUILD_GUI_GDI
		"Build GDI drivers for axl_gui library"
		ON
		)
endif ()

#..............................................................................

if (${QT_FOUND})
	option (
		AXL_BUILD_GUI_QT
		"Build QT drivers for axl_gui library"
		ON
		)
endif ()

#..............................................................................

option (
	AXL_BUILD_GUI_LIB
	"Build axl_gui library"
	ON
	)

if (AXL_BUILD_GUI_LIB)
	add_subdirectory (axl_gui)
endif ()

#..............................................................................

option (
	AXL_BUILD_INI_LIB
	"Build axl_ini library"
	ON
	)

if (AXL_BUILD_INI_LIB)
	add_subdirectory (axl_ini)
endif ()

#..............................................................................

option (
	AXL_BUILD_IO_LIB
	"Build axl_io library"
	ON
	)

if (${PCAP_FOUND})
	option (
		AXL_BUILD_IO_PCAP
		"Build PCap wrappers for axl_io library"
		ON
		)
endif ()

if (AXL_BUILD_IO_LIB)
	add_subdirectory (axl_io)
endif ()

#..............................................................................

option (
	AXL_BUILD_LUA_LIB
	"Build axl_lua library"
	ON
	)

if (AXL_BUILD_LUA_LIB)
	add_subdirectory (axl_lua)
endif ()

#..............................................................................
