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

# make AXL_CMAKE_DIR available to any project within the same source tree

set (
	AXL_CMAKE_DIR
	${CMAKE_CURRENT_LIST_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	CACHE INTERNAL
	"In-source-tree AXL CMake directories"
	)

install (
	DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/
	DESTINATION ${INSTALL_CMAKE_DIR}
	FILES_MATCHING PATTERN *.cmake
	)

install (
	FILES ${CMAKE_CURRENT_BINARY_DIR}/axl_config.cmake
	DESTINATION ${INSTALL_CMAKE_DIR}
	)

install (
	FILES ${AXL_ROOT_DIR}/version.cmake
	RENAME axl_version.cmake
	DESTINATION ${INSTALL_CMAKE_DIR}
	)

#...............................................................................
