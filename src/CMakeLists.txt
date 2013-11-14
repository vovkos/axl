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

option (
	AXL_BUILD_GUI_LIB
	"Build axl_gui library"
	ON
	)

if (AXL_BUILD_GUI_LIB)
	add_subdirectory (axl_gui)
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

if (AXL_BUILD_IO_LIB)
	add_subdirectory (axl_io)
endif ()

#..............................................................................

option (
	AXL_BUILD_JNC_LIB
	"Build axl_jnc library"
	ON
	)

if (AXL_BUILD_JNC_LIB)
	add_subdirectory (axl_jnc)

	add_subdirectory (
		${AXL_ROOT_DIR}/samples/jnc/demos
		${CMAKE_CURRENT_BINARY_DIR}/jnc_demos
		)
endif ()

#..............................................................................

option (
	AXL_BUILD_JNCC
	"Build axl_jncc (Jancy command-line compiler)"
	ON
	)

if (AXL_BUILD_JNCC)
	add_subdirectory (axl_jncc)
endif ()

#..............................................................................

option (
	AXL_BUILD_LOG_LIB
	"Build axl_log library"
	ON
	)

option (
	AXL_BUILD_LOG_JANCY
	"Build Jancy plugin drivers for axl_log library"
	ON
	)

if (AXL_BUILD_LOG_LIB)
	add_subdirectory (axl_log)
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

option (
	AXL_BUILD_PARSE_LIB
	"Build axl_parse library"
	ON
	)

if (AXL_BUILD_PARSE_LIB)
	add_subdirectory (axl_parse)
endif ()

#..............................................................................

option (
	AXL_BUILD_PG
	"Build axl_pg (LL(k) parser generator)"
	ON
	)

if (AXL_BUILD_PG)
	add_subdirectory (axl_pg)
endif ()

#..............................................................................
