#...............................................................................
#
#  This file is part of the AXL library.
#
#  AXL is distributed under the MIT license.
#  For details see accompanying license.txt file,
#  the public copy of which is also available at:
#  http://tibbo.com/downloads/archive/axl/license.txt
#
#...............................................................................

# sanity checks

get_filename_component(_FILE_NAME ${CMAKE_PARENT_LIST_FILE} NAME)

if(NOT "${_FILE_NAME}" STREQUAL "axl_cmake.cmake" OR
	NOT EXISTS "${AXL_CMAKE_ORIGIN_DIR}/dependencies.cmake")

	message(FATAL_ERROR "Incorrect usage of axl_init.cmake!")
endif()

#...............................................................................

# CMake policies

# as of now, we have CMake version requirement of 3.3, and all the default
# policy values work well with AXL CMake infrastructure.

# when in need to adjust a policy, use the following template:

# if (POLICY CMPNNNN)
#	cmake_policy (SET CMPNNNN OLD)
# endif ()

#...............................................................................

include(${CMAKE_CURRENT_LIST_DIR}/axl_utils.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/axl_std_settings.cmake)

axl_create_std_settings()

# find paths.cmake

axl_find_file_recurse_parent_dirs(
	AXL_PATHS_CMAKE
	paths.cmake
	${AXL_CMAKE_ORIGIN_DIR}
	)

# find and include settings.cmake to give a chance for overriding std settings...

axl_find_file_recurse_parent_dirs(
	AXL_SETTINGS_CMAKE
	settings.cmake
	${AXL_CMAKE_ORIGIN_DIR}
	)

if(AXL_SETTINGS_CMAKE)
	include(${AXL_SETTINGS_CMAKE})
endif()

# ...then apply and do diagnostic print

axl_apply_std_settings()
axl_print_std_settings()

# re-include paths.cmake and dependencies.cmake -- this time ${CONFIGURATION}
# and all the other settings are defined

if(AXL_PATHS_CMAKE)
	include(${AXL_PATHS_CMAKE})
endif()

include(${AXL_CMAKE_ORIGIN_DIR}/dependencies.cmake)

get_cmake_property(_VARIABLE_LIST VARIABLES)
string(REPLACE ";" "\$|^" _FILTER "^${AXL_PATH_LIST}\$")
axl_filter_list(_FILTERED_VARIABLE_LIST ${_FILTER} ${_VARIABLE_LIST})

if(_FILTERED_VARIABLE_LIST)
	message(STATUS "Dependency path definitions:")

	axl_print_variable_list(
		"    "
		${AXL_G_MESSAGE_ALIGN}
		${_FILTERED_VARIABLE_LIST}
		)
endif()

# import modules (if dependencies.cmake defines any imports)

set(
	AXL_IMPORT_DIR_LIST
	${AXL_IMPORT_DIR_LIST}
	${CMAKE_CURRENT_LIST_DIR}
	)

if(AXL_IMPORT_LIST)
	axl_import(${AXL_IMPORT_LIST})
endif()

#...............................................................................

enable_testing()
