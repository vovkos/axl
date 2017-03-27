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

string (REGEX REPLACE "[a-z]+" ".." AXL_INSTALL_ROOT_SUBDIR ${AXL_INSTALL_CMAKE_SUBDIR})

configure_file (
	axl_config_build.cmake.in
	axl_config.cmake
	@ONLY
	)

configure_file (
	axl_config_install.cmake.in
	axl_config_install.cmake
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

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

install (
	DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/
	DESTINATION ${AXL_INSTALL_CMAKE_SUBDIR}
	FILES_MATCHING PATTERN *.cmake
	)

install (
	FILES ${CMAKE_CURRENT_BINARY_DIR}/axl_config_install.cmake
	RENAME axl_config.cmake
	DESTINATION ${AXL_INSTALL_CMAKE_SUBDIR}
	)

install (
	FILES ${AXL_ROOT_DIR}/version.cmake
	RENAME axl_version.cmake
	DESTINATION ${AXL_INSTALL_CMAKE_SUBDIR}
	)

#...............................................................................
