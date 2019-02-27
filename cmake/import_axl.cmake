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

axl_find_file(
	_CONFIG_CMAKE
	axl_config.cmake
	${AXL_CMAKE_DIR}
	)

if(_CONFIG_CMAKE)
	include(${_CONFIG_CMAKE})

	axl_message("AXL ${AXL_VERSION_FULL} paths:")
	axl_message("    CMake files:" "${AXL_CMAKE_DIR}")
	axl_message("    Includes:"    "${AXL_INC_DIR}")
	axl_message("    Libraries:"   "${AXL_LIB_DIR}")

	set(AXL_FOUND TRUE)
else()
	set(AXL_FOUND FALSE)
endif()

#...............................................................................
