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

unset(RE2S_FOUND)
unset(re2_DIR CACHE)

if(RE2S_CMAKE_DIR)
	find_package(re2 QUIET PATHS ${RE2S_CMAKE_DIR} NO_DEFAULT_PATH)
endif()

if(re2_FOUND)
	set(RE2S_FOUND TRUE)
	set(RE2S_CMAKE_DIR ${re2_DIR})
	set(RE2S_VERSION ${re2_VERSION})
	get_target_property(RE2S_INC_DIR re2::re2 INTERFACE_INCLUDE_DIRECTORIES)

	axl_message("RE2S ${RE2S_VERSION} paths:")
	axl_message("    CMake files:"  "${RE2S_CMAKE_DIR}")
	axl_message("    Includes:"     "${RE2S_INC_DIR}")
endif()

#...............................................................................
