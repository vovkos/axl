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

if(EXISTS ${RE2S_INC_DIR}/re2/sm.h AND RE2S_LIB_DIR)
	set(RE2S_FOUND TRUE)
	axl_find_file(
		_VERSION_CMAKE
		re2ConfigVersion.cmake
		${RE2S_CMAKE_DIR}
		${RE2S_CMAKE_DIR}/re2
	)

	if(_VERSION_CMAKE)
		include(${_VERSION_CMAKE})
		set(RE2S_VERSION ${PACKAGE_VERSION})
	endif()
elseif(RE2S_CMAKE_DIR)
	unset(re2_DIR CACHE)
	find_package(re2 QUIET PATHS ${RE2S_CMAKE_DIR} NO_DEFAULT_PATH)

	if(re2_FOUND)
		set(RE2S_FOUND TRUE)
		set(RE2S_CMAKE_DIR ${re2_DIR})
		set(RE2S_VERSION ${re2_VERSION})
		get_target_property(RE2S_INC_DIR re2::re2 INTERFACE_INCLUDE_DIRECTORIES)
		get_target_property(_ re2::re2 IMPORTED_LOCATION_DEBUG)
	endif()
endif()

if(RE2S_FOUND)
	if(RE2S_VERSION)
		axl_message("RE2S ${RE2S_VERSION} paths:")
	else()
		axl_message("RE2S paths:")
	endif()
	if(RE2S_CMAKE_DIR)
		axl_message("    CMake files:"  "${RE2S_CMAKE_DIR}")
	endif()
	axl_message("    Includes:"     "${RE2S_INC_DIR}")
	if(RE2S_LIB_DIR)
		axl_message("    Libraries:"    "${RE2S_LIB_DIR}")
	endif()
endif()

#...............................................................................
