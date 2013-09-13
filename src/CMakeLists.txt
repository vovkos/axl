# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2013. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................

project (axl)

cmake_minimum_required (VERSION 2.8)

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../cmake")

include (CMakeUtils)

#..............................................................................
#
# Target CPU
#

# set (TARGET_CPU "x86") 

if (NOT TARGET_CPU) 
	detect_cpu (TARGET_CPU) 
endif ()       

message (STATUS "Target CPU:                 ${TARGET_CPU}")

#..............................................................................
# 
# Standard settings
# 

include (CMakeStdSettings) 

create_build_type_setting ()

if (${CMAKE_BUILD_TYPE})
	message (STATUS "Build type: ${CMAKE_BUILD_TYPE}")
endif ()

if (MSVC)
	create_msvc_flag_eh_setting ()
	create_msvc_flag_rtti_setting ()
	create_msvc_flag_crt_setting ()
	create_msvc_flag_debug_info_setting ()
	create_msvc_flag_unicode_setting ()

	message (STATUS "C++ flags (Debug):         ${CMAKE_CXX_FLAGS}${CMAKE_CXX_FLAGS_DEBUG}")
	message (STATUS "C++ flags (Release):       ${CMAKE_CXX_FLAGS}${CMAKE_CXX_FLAGS_RELEASE}")
endif ()

if (CMAKE_COMPILER_IS_GNUCXX)
	create_gcc_flag_cpu_setting ()
	create_gcc_flag_show_includes_setting ()
	create_gcc_flag_format_warning_setting ()
	create_gcc_flag_multichar_wharning_setting ()

	message (STATUS "C++ flags: ${COMPILE_DEFINITIONS}${CMAKE_CXX_FLAGS}")
endif ()

get_directory_property (_CXX_DEFINITIONS COMPILE_DEFINITIONS)

if (_CXX_DEFINITIONS)
	message (STATUS "C++ definitions:            ${_CXX_DEFINITIONS}")
endif ()

#..............................................................................
#
# Paths to libs & tools 
#
                    
include (CMakePaths)

message (STATUS "Path to RAGEL executable:   ${PATH_RAGEL_EXE}")
message (STATUS "Path to LUA includes:       ${PATH_LUA_INC_DIR}")
message (STATUS "Path to LUA libraries:      ${PATH_LUA_LIB_DIR}")
message (STATUS "Path to LLVM cmake modules: ${PATH_LLVM_CMAKE_DIR}")
message (STATUS "Path to LLVM includes:      ${PATH_LLVM_INC_DIR}")
message (STATUS "Path to LLVM includes (2):  ${PATH_LLVM_INC_DIR_2}")
message (STATUS "Path to LLVM libraries:     ${PATH_LLVM_LIB_DIR}")

include ("${PATH_LLVM_CMAKE_DIR}/LLVMConfig.cmake")

# QT is optional

if (NOT "${PATH_QT_CMAKE_DIR}" STREQUAL "")	
	message (STATUS "Path to QT cmake modules:   ${PATH_QT_CMAKE_DIR}")
	include ("${PATH_QT_CMAKE_DIR}/Qt5Core/Qt5CoreConfig.cmake")
	include ("${PATH_QT_CMAKE_DIR}/Qt5Widgets/Qt5WidgetsConfig.cmake")
	set (QT_FOUND true)
endif ()

#..............................................................................
#
# Project-specific settings
#

option (
	AXL_USE_PCH
	"Use precompiled headers when building AXL project"
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

if (${QT_FOUND})
	option (
		AXL_BUILD_GUI_QT
		"Build QT drivers for axl_gui library"
		ON
		)
endif ()

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
	"Build axl_jncc (Jancy command-line compiler)"
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
	AXL_BUILD_CON_TEST
	"Build ConTest"
	OFF
	)

if (${QT_FOUND})
	option (
		AXL_BUILD_QT_TEST
		"Build QtTest"
		OFF
		)
endif ()

if (WIN32)
	option (
		AXL_BUILD_JNC_MFC_TEST
		"Build JncMfcTest"
		OFF
		)
endif ()

if (${QT_FOUND})
	option (
		AXL_BUILD_JNC_QT_TEST
		"Build JncQtTest"
		ON
		)
endif ()

#..............................................................................
#
# main
#

set (AXL_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
set (AXL_INC_DIR  ${AXL_ROOT_DIR}/include)
set (AXL_SRC_DIR  ${AXL_ROOT_DIR}/src)
set (AXL_PG_FRAME_DIR ${AXL_ROOT_DIR}/pgframe)

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

include_directories (${AXL_INC_DIR})

#
# add targets
#

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

if (AXL_BUILD_JNCC)
	add_subdirectory (axl_jncc)
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

if (AXL_BUILD_CON_TEST)
	add_subdirectory (tests/ConTest)
endif ()

if (AXL_BUILD_QT_TEST)
	add_subdirectory (tests/QtTest)
endif ()

if (AXL_BUILD_JNC_MFC_TEST)
	add_subdirectory (tests/JncMfcTest)
endif ()

if (AXL_BUILD_JNC_QT_TEST)
	add_subdirectory (tests/JncQtTest)
endif ()

#..............................................................................
