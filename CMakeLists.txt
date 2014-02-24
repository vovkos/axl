# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2014. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................

project (axl)

cmake_minimum_required (VERSION 2.8)

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../cmake")

include (CMakeUtils)

#..............................................................................
#
# Standard settings
#

include (CMakeStdSettings)

create_std_settings ()

message (STATUS "Target CPU:                 ${TARGET_CPU}")

if (HAS_BUILD_TYPE_SETTING)
	message (STATUS "Build type:                 ${CMAKE_BUILD_TYPE}")
endif ()

message (STATUS "C++ flags (Debug):         ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
message (STATUS "C++ flags (Release):       ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")

get_directory_property (_CXX_DEFINITIONS COMPILE_DEFINITIONS)

if (_CXX_DEFINITIONS)
	message (STATUS "C++ definitions:            ${_CXX_DEFINITIONS}")
endif ()

#..............................................................................
#
# Paths to libs & tools
#

ensure_file_exists (
	${CMAKE_MODULE_PATH}/CMakePaths.cmake
	${CMAKE_MODULE_PATH}/CMakePaths.template
	)

include (CMakePaths)

message (STATUS "Path to RAGEL executable:   ${PATH_RAGEL_EXE}")
message (STATUS "Path to LUA includes:       ${PATH_LUA_INC_DIR}")
message (STATUS "Path to LUA libraries:      ${PATH_LUA_LIB_DIR}")

if (NOT "${PATH_PCAP_INC_DIR}" STREQUAL "")
	message (STATUS "Path to PCap includes:      ${PATH_PCAP_INC_DIR}")
	message (STATUS "Path to PCap libraries:     ${PATH_PCAP_LIB_DIR}")
	set (PCAP_FOUND TRUE)
endif ()

if (NOT "${PATH_QT_CMAKE_DIR}" STREQUAL "")
	message (STATUS "Path to QT cmake modules:   ${PATH_QT_CMAKE_DIR}")

	unset (Qt5Core_DIR    CACHE)
	unset (Qt5Gui_DIR     CACHE)
	unset (Qt5Widgets_DIR CACHE)

	include ("${PATH_QT_CMAKE_DIR}/Qt5Core/Qt5CoreConfig.cmake")
	include ("${PATH_QT_CMAKE_DIR}/Qt5Widgets/Qt5WidgetsConfig.cmake")
	set (QT_FOUND TRUE)
endif ()

#..............................................................................
#
# main
#

set (AXL_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
set (AXL_INC_DIR  ${AXL_ROOT_DIR}/include)

option (
	AXL_USE_PCH
	"Use precompiled headers when building AXL project"
	ON
	)

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

enable_testing ()
include_directories (${AXL_INC_DIR})

add_subdirectory (src)
add_subdirectory (tests)

#..............................................................................
