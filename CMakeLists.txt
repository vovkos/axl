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
set (AXL_DOC_DIR      ${CMAKE_CURRENT_BINARY_DIR}/doc)
set (AXL_PACKAGE_DIR  ${CMAKE_CURRENT_BINARY_DIR}/package)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${AXL_BIN_BASE_DIR}/${CONFIGURATION_SUFFIX_0})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${AXL_BIN_BASE_DIR}/${CONFIGURATION_SUFFIX_0})
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${AXL_LIB_BASE_DIR}/${CONFIGURATION_SUFFIX_0})

set (AXL_INSTALL_LIB_SUBDIR lib)

if (WIN32)
	set (AXL_INSTALL_INC_SUBDIR     include)
	set (AXL_INSTALL_CMAKE_SUBDIR   cmake)
	set (AXL_INSTALL_DOC_SUBDIR     doc)
	set (AXL_INSTALL_LICENSE_SUBDIR license)
else ()
	set (AXL_INSTALL_INC_SUBDIR     include/${CMAKE_PROJECT_NAME})
	set (AXL_INSTALL_CMAKE_SUBDIR   lib/cmake/${CMAKE_PROJECT_NAME})
	set (AXL_INSTALL_DOC_SUBDIR     share/${CMAKE_PROJECT_NAME}/doc)
	set (AXL_INSTALL_LICENSE_SUBDIR share/${CMAKE_PROJECT_NAME}/license)
endif ()

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

add_subdirectory (cmake)

include (${CMAKE_CURRENT_BINARY_DIR}/cmake/axl_config.cmake)

add_subdirectory (src)
add_subdirectory (doc)
add_subdirectory (test)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

install (
	DIRECTORY license/
	DESTINATION ${AXL_INSTALL_LICENSE_SUBDIR}
	)

#...............................................................................

set (CPACK_PACKAGE_VERSION_MAJOR ${AXL_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${AXL_VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${AXL_VERSION_REVISION})

set (CPACK_SOURCE_GENERATOR TXZ)
set (CPACK_SOURCE_IGNORE_FILES "/[.];[.]user$;/build/;/ci/;/paths.cmake$")

string (TOLOWER ${CMAKE_SYSTEM_NAME} OS_NAME)

set (CPACK_PACKAGE_FILE_NAME ${CMAKE_PROJECT_NAME}-${OS_NAME}-${AXL_VERSION_FILE_SUFFIX}-${TARGET_CPU})
set (CPACK_SOURCE_PACKAGE_FILE_NAME ${CMAKE_PROJECT_NAME}-src-${AXL_VERSION_FILE_SUFFIX})

include (CPack)

#...............................................................................
