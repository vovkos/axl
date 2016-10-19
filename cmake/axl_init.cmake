#...............................................................................

# sanity checks

if (NOT EXISTS "${AXL_DROPPER_DIR}/axl_cmake.cmake" OR
	NOT EXISTS "${AXL_PATHS_CMAKE}" OR
	NOT EXISTS "${CMAKE_SOURCE_DIR}/dependencies.cmake")

	message (FATAL_ERROR "Incorrect usage of axl_init.cmake!")
endif ()

#...............................................................................

# CMake policies

if (POLICY CMP0020)
	# Automatically link Qt executables to qtmain.lib on Windows (OLD: OFF; NEW: ON)
	cmake_policy (SET CMP0020 OLD)
endif ()

if (POLICY CMP0043)
	# Ignore COMPILE_DEFINITIONS_<Config> properties (OLD: OFF; NEW: ON)
	cmake_policy (SET CMP0043 NEW)
endif ()

if (POLICY CMP0054)
	# Only interpret if() arguments as variables or keywords when unquoted (OLD: OFF; NEW: ON)
	cmake_policy (SET CMP0054 NEW)
endif ()

#...............................................................................

include (${CMAKE_CURRENT_LIST_DIR}/axl_utils.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/axl_std_settings.cmake)

axl_create_std_settings ()

# find and include settings.cmake to give a chance for overriding std settings

axl_find_file_recurse_parent_dirs (_SETTINGS_CMAKE settings.cmake ${AXL_DROPPER_DIR})

if (_SETTINGS_CMAKE)
	include (${_SETTINGS_CMAKE})
endif ()

# apply std settings

axl_apply_std_settings ()

# re-include paths.cmake and dependencies.cmake
# this time ${CONFIGURATION_SUFFIX} and all the other settings are defined

include (${AXL_PATHS_CMAKE})
include (${CMAKE_SOURCE_DIR}/dependencies.cmake)

# do diagnostic printing

axl_print_std_settings ()

get_cmake_property (_VARIABLE_LIST VARIABLES)
string (REPLACE ";" "\$|^" _FILTER "^${AXL_PATH_LIST}\$")
axl_filter_list (_FILTERED_VARIABLE_LIST ${_FILTER} ${_VARIABLE_LIST})

message(STATUS "Path defintions in ${AXL_PATHS_CMAKE}:")

axl_print_variable_list (
	"    "
	${AXL_G_MESSAGE_ALIGN}
	${_FILTERED_VARIABLE_LIST}
	)

# import modules (if dependencies.cmake defines any imports)

set (
	AXL_IMPORT_DIR_LIST
	${AXL_IMPORT_DIR_LIST}
	${CMAKE_CURRENT_LIST_DIR}
	)

if (AXL_IMPORT_LIST)
	axl_import (${AXL_IMPORT_LIST})
endif ()

#...............................................................................

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib/${CONFIGURATION_SUFFIX_0})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin/${CONFIGURATION_SUFFIX_0})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin/${CONFIGURATION_SUFFIX_0})

enable_testing ()
