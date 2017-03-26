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

project (axl)

cmake_minimum_required (VERSION 3.3)

include (cmake/axl_cmake.cmake NO_POLICY_SCOPE)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

set (AXL_ROOT_DIR     ${CMAKE_CURRENT_LIST_DIR})
set (AXL_BIN_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR}/bin)
set (AXL_LIB_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR}/lib)
set (AXL_PACKAGE_DIR  ${CMAKE_CURRENT_BINARY_DIR}/package)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${AXL_BIN_BASE_DIR}/${CONFIGURATION_SUFFIX_0})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${AXL_BIN_BASE_DIR}/${CONFIGURATION_SUFFIX_0})
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${AXL_LIB_BASE_DIR}/${CONFIGURATION_SUFFIX_0})

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

axl_create_setting (
	INSTALL_INCLUDE_DIR
	DESCRIPTION "Installation directory for header files"
	DEFAULT "include/${CMAKE_PROJECT_NAME}"
	"include"
	"include/${CMAKE_PROJECT_NAME}"
	)

axl_create_setting (
	INSTALL_LIB_DIR
	DESCRIPTION "Installation directory for library files"
	DEFAULT "lib"
	"lib"
	"lib/${CMAKE_PROJECT_NAME}"
	)

axl_create_setting (
	INSTALL_CMAKE_DIR
	DESCRIPTION "Installation directory for CMake files"
	DEFAULT lib/cmake/${CMAKE_PROJECT_NAME}
	"cmake"
	"share/${CMAKE_PROJECT_NAME}/cmake"
	"lib/cmake/${CMAKE_PROJECT_NAME}"
	)

axl_create_setting (
	INSTALL_DOC_DIR
	DESCRIPTION "Installation directory for documentation files"
	DEFAULT "share/${CMAKE_PROJECT_NAME}/doc"
	"doc"
	"share/doc/${CMAKE_PROJECT_NAME}"
	"share/${CMAKE_PROJECT_NAME}/doc"
	)

axl_create_setting (
	INSTALL_LICENSE_DIR
	DESCRIPTION "Installation directory for license files"
	DEFAULT "share/${CMAKE_PROJECT_NAME}/license"
	"license"
	"share/licenses/${CMAKE_PROJECT_NAME}"
	"share/${CMAKE_PROJECT_NAME}/license"
	)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

add_subdirectory (cmake)

include (${CMAKE_CURRENT_BINARY_DIR}/cmake/axl_config.cmake)

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

install (
	DIRECTORY license/
	DESTINATION ${INSTALL_LICENSE_DIR}
	)

#...............................................................................

set (CPACK_PACKAGE_VERSION_MAJOR ${AXL_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${AXL_VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${AXL_VERSION_REVISION})

set (CPACK_SOURCE_GENERATOR TXZ)
set (CPACK_SOURCE_IGNORE_FILES "/\\.;\\.user$;/build/;/travis/;/paths.cmake$")

string (TOLOWER ${CMAKE_SYSTEM_NAME} OS_NAME)

set (CPACK_PACKAGE_FILE_NAME axl-${OS_NAME}-${AXL_VERSION_MAJOR}.${AXL_VERSION_MINOR}.${AXL_VERSION_REVISION}-${AXL_CPU})
set (CPACK_SOURCE_PACKAGE_FILE_NAME axl-src-${AXL_VERSION_MAJOR}.${AXL_VERSION_MINOR}.${AXL_VERSION_REVISION})

include (CPack)

#...............................................................................
