# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2015. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................

project (axl)

cmake_minimum_required (VERSION 2.8)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

set (AXL_ROOT_DIR     ${CMAKE_CURRENT_LIST_DIR})
set (AXL_BIN_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR}/bin)
set (AXL_LIB_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR}/lib)
set (AXL_DOC_DIR      ${CMAKE_CURRENT_BINARY_DIR}/doc)
set (AXL_PACKAGE_DIR  ${CMAKE_CURRENT_BINARY_DIR}/package)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

add_subdirectory (cmake)

include (${CMAKE_CURRENT_BINARY_DIR}/cmake/axl_config.cmake)
include (cmake/axl_init.cmake NO_POLICY_SCOPE)

axl_find_file_recurse_parent_dirs (_PATHS_CMAKE paths.cmake ${CMAKE_CURRENT_LIST_DIR})

if (NOT _PATHS_CMAKE)
	include (dependencies.cmake)
	axl_create_empty_setting_file (
		${CMAKE_SOURCE_DIR}/paths.cmake
		${AXL_PATH_LIST}
		)

	message (FATAL_ERROR "please fill the newly generated ${CMAKE_SOURCE_DIR}/paths.cmake")
endif ()

include (${_PATHS_CMAKE})
include (dependencies.cmake)

axl_find_file_recurse_parent_dirs (_SETTINGS_CMAKE settings.cmake ${CMAKE_CURRENT_LIST_DIR})

if (_SETTINGS_CMAKE)
	include (${_SETTINGS_CMAKE})
endif ()

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

axl_print_std_settings ()

get_cmake_property (_VARIABLE_LIST VARIABLES)
string (REPLACE ";" ":" _FILTER "${AXL_PATH_LIST}")
string (REPLACE ":" "\;|\;" _FILTER "${_FILTER}")
set (_FILTER "\;${_FILTER}\;")

string (REGEX MATCHALL ";${_FILTER};" _FILTERED_VARIABLE_LIST ";${_VARIABLE_LIST};")

message(STATUS "Path defintions in ${_PATHS_CMAKE}:")

axl_print_variable_list ("    " ${_FILTERED_VARIABLE_LIST})

axl_import (${AXL_IMPORT_LIST})

#..............................................................................

add_subdirectory (src)
add_subdirectory (doc)

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/test/CMakeLists.txt")
	add_subdirectory (test)
endif ()

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/samples/CMakeLists.txt")
	add_subdirectory (samples)
endif ()
	
if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/package/CMakeLists.txt")
	add_subdirectory (package)
endif ()

#..............................................................................
