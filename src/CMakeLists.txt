# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2013. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................

project (axl)
cmake_minimum_required (VERSION 2.8)

# set (TARGET_CPU "x86")

if (NOT TARGET_CPU)
	if (CMAKE_SIZEOF_VOID_P EQUAL 8)
		set (TARGET_CPU "amd64")
	else ()
		set (TARGET_CPU "x86")
	endif ()		
endif ()

message (STATUS "Target CPU: ${TARGET_CPU}")

include (CMakeUtils.cmake)
include (CMakePaths.cmake)

#..............................................................................
#
# Available configurations
#

set (
	CMAKE_CONFIGURATION_TYPES 
	Debug
	Release
	)

# Microsoft Visual C++ is a multi-configuration generator
# otherwise, create a setting for choosing current configuration

if (NOT MSVC)
	create_setting (
		CMAKE_BUILD_TYPE
		Debug
		STRING
		"Configuration type"
		${CMAKE_CONFIGURATION_TYPES}
		)
endif ()

#..............................................................................
#
# Settings for Microsoft Visual C++ compiler flags 
#

if (MSVC)
	create_flag_setting (
		MSVC_FLAG_EH
		"" # -- use default
		CMAKE_CXX_FLAGS
		"Microsoft Visual C++ EH (exception handling) model"
		"/EHsc" "/EHs-c-"
		)

	create_flag_setting (
		MSVC_FLAG_RTTI
		"/GR-" # -- use default
		CMAKE_CXX_FLAGS
		"Microsoft Visual C++ RTTI (run-time type information) support"
		"/GR" "/GR-"
		)

	create_flag_setting (
		MSVC_FLAG_CRT_DEBUG
		"/MTd"
		CMAKE_CXX_FLAGS_DEBUG
		"Microsoft Visual C++ CRT (Debug configuration)"
		"/MT" "/MTd" "/MD" "/MDd"
		)

	create_flag_setting (
		MSVC_FLAG_CRT_RELEASE
		"/MT"
		CMAKE_CXX_FLAGS_RELEASE
		"Microsoft Visual C++ CRT (Release configuration)"
		"/MT" "/MTd" "/MD" "/MDd"
		)

	create_flag_setting (
		MSVC_FLAG_DEBUG_INFO_DEBUG
		"/Zi"
		CMAKE_CXX_FLAGS_DEBUG
		"Microsoft Visual C++ debug information format (Debug configuration)"
		"/Z7" "/Zi" "/ZI"  
		)

	create_flag_setting (
		MSVC_FLAG_DEBUG_INFO_RELEASE
		"/Zi" 
		CMAKE_CXX_FLAGS_RELEASE
		"Microsoft Visual C++ debug information format (Release configuration)"
		"/Z7" "/Zi" "/ZI"  
		)

endif ()

#..............................................................................
#
# Settings for GNU C++ compiler flags 
#

if (CMAKE_COMPILER_IS_GNUCXX)

	if ("${TARGET_CPU}" STREQUAL "amd64")
		set (DEFAULT_GCC_FLAG_CPU "-m64")
	else ()
		set (DEFAULT_GCC_FLAG_CPU "-m32")
	endif ()
	
	create_flag_setting (
		GCC_FLAG_CPU
		"${DEFAULT_GCC_FLAG_CPU}"
		CMAKE_CXX_FLAGS
		"GNU C++ target CPU arch"
		"-m32" "-m64"
		)

	option (
		GCC_SHOW_INCLUDES
		"GNU C++ shows include stack during compilation"
		OFF
		)

	if (${GCC_SHOW_INCLUDES})
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -H")
	endif ()

	# forcefully disable some warnings	

	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-format -Wno-multichar")
endif ()

#..............................................................................
#
# Ragel path
#

create_setting (
	PATH_RAGEL_EXE
	${DEFAULT_PATH_RAGEL_EXE}
	FILEPATH
	"Path to Ragel executable"
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# LUA path
#

create_setting (
	PATH_LUA_INC_DIR
	${DEFAULT_PATH_LUA_INC_DIR}
	PATH
	"Path to LUA include files directory"
	)

create_setting (
	PATH_LUA_LIB_DIR
	${DEFAULT_PATH_LUA_LIB_DIR}
	PATH
	"Path to LUA library files directory"
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# LLVM path
#

create_setting (
	PATH_LLVM_CMAKE_DIR
	${DEFAULT_PATH_LLVM_CMAKE_DIR}
	PATH
	"Path to LLVM CMake module files directory"
	)

create_setting (
	PATH_LLVM_INC_DIR
	${DEFAULT_PATH_LLVM_INC_DIR}
	PATH
	"Path to LLVM primary include files directory"
	)

create_setting (
	PATH_LLVM_INC_DIR_2
	${DEFAULT_PATH_LLVM_INC_DIR_2}
	PATH
	"Path to LLVM secondary (generated) include files directory"
	)

create_setting (
	PATH_LLVM_LIB_DIR
	${DEFAULT_PATH_LLVM_LIB_DIR}
	PATH
	"Path to LLVM library files directory"
	)

include ("${PATH_LLVM_CMAKE_DIR}/LLVMConfig.cmake")

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# QT path
#

create_setting (
	PATH_QT_CMAKE_DIR
	${DEFAULT_PATH_QT_CMAKE_DIR}
	PATH
	"Path to QT CMake module files directory"
	)

include ("${PATH_QT_CMAKE_DIR}/Qt5Core/Qt5CoreConfig.cmake")

#..............................................................................
#
# AXL-specific settings
#

option (
	AXL_USE_PCH
	"Use precompiled headers"
	ON
	)

option (
	AXL_BUILD_EXE_LIB
	"Build axl_exe library"
	OFF
	)

option (
	AXL_BUILD_GUI_LIB
	"Build axl_gui library"
	ON
	)

if (WIN32)
	option (
		AXL_BUILD_GUI_GDI
		"Build GDI drivers for axl_gui library"
		ON
		)
endif ()

option (
	AXL_BUILD_GUI_QT
	"Build QT drivers for axl_gui library"
	ON
	)

option (
	AXL_BUILD_IO_LIB
	"Build axl_io library"
	ON
	)

option (
	AXL_BUILD_JNC_LIB
	"Build axl_jnc library"
	ON
	)

option (
	AXL_BUILD_JNCC
	"Build axl_jncc (Jancy compiler)"
	ON
	)

option (
	AXL_BUILD_LOG_LIB
	"Build axl_log library"
	ON
	)

option (
	AXL_BUILD_LUA_LIB
	"Build axl_lua library"
	ON
	)

option (
	AXL_BUILD_PARSE_LIB
	"Build axl_parse library"
	ON
	)

option (
	AXL_BUILD_PG
	"Build axl_pg (LL(k) parser generator)"
	ON
	)

option (
	AXL_BUILD_TESTS
	"Build tests (ConTest, QtTest, JncMfcTest, JncQtTest)"
	ON
	)

if (MSVC)
	option (
		AXL_UNICODE_CHARSET
		"Compile for UNICODE character set rather than multibyte"
		ON
		)

	if (${AXL_UNICODE_CHARSET})
		add_definitions(-DUNICODE -D_UNICODE)
	endif ()
endif ()

#..............................................................................
#
# main
#

set (AXL_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set (AXL_INC_DIR  ${AXL_ROOT_DIR}/include)
set (AXL_SRC_DIR  ${AXL_ROOT_DIR}/src)
set (AXL_PG_FRAME_DIR ${AXL_ROOT_DIR}/pgframe)

include_directories (${AXL_INC_DIR})

# libraries

add_subdirectory (axl_core)

if (AXL_BUILD_EXE_LIB)
	add_subdirectory (axl_exe)
endif ()

if (AXL_BUILD_GUI_LIB)
	add_subdirectory (axl_gui)
endif ()

if (AXL_BUILD_IO_LIB)
	add_subdirectory (axl_io)
endif ()

if (AXL_BUILD_JNC_LIB)
	add_subdirectory (axl_jnc)
endif ()

if (AXL_BUILD_LOG_LIB)
	add_subdirectory (axl_log)
endif ()

if (AXL_BUILD_LUA_LIB)
	add_subdirectory (axl_lua)
endif ()

if (AXL_BUILD_PARSE_LIB)
	add_subdirectory (axl_parse)
endif ()

if (AXL_BUILD_PG)
	add_subdirectory (axl_pg)
endif ()

if (AXL_BUILD_TESTS)
	# add_subdirectory (tests/ConTest)

	if (WIN32)
		add_subdirectory (tests/JncMfcTest)
	endif ()

	# add_subdirectory (tests/QtTest)
	add_subdirectory (tests/JncQtTest)
endif ()

#..............................................................................
