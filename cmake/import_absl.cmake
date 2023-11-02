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

unset(ABSL_FOUND)
unset(absl_DIR CACHE)

if(NOT ABSL_CMAKE_DIR)
	find_package(absl QUIET)
elseif(EXISTS ${ABSL_CMAKE_DIR}/abslConfig.cmake)
	find_package(absl QUIET PATHS ${ABSL_CMAKE_DIR} NO_DEFAULT_PATH)
endif()

if(absl_FOUND)
	set(ABSL_FOUND TRUE)
	set(ABSL_CMAKE_DIR ${absl_DIR})
	set(ABSL_VERSION ${absl_VERSION})
	get_target_property(ABSL_INC_DIR absl::base INTERFACE_INCLUDE_DIRECTORIES)

	axl_message("ABSL ${ABSL_VERSION} paths:")
	axl_message("    CMake files:"  "${ABSL_CMAKE_DIR}")
	axl_message("    Includes:"     "${ABSL_INC_DIR}")
endif()

#...............................................................................
