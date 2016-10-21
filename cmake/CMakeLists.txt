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

configure_file (
	axl_config.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/axl_config.cmake
	@ONLY
	)

#...............................................................................
