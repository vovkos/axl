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

cmake_minimum_required(VERSION 3.16)

project(axl)

include(cmake/axl_cmake.cmake NO_POLICY_SCOPE)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

set(AXL_ROOT_DIR     ${CMAKE_CURRENT_LIST_DIR})
set(AXL_BIN_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(AXL_LIB_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(AXL_DOC_DIR      ${CMAKE_CURRENT_BINARY_DIR}/doc)
set(AXL_SHARE_DIR    ${CMAKE_CURRENT_BINARY_DIR}/share)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${AXL_BIN_BASE_DIR}/${CONFIGURATION_SCG})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${AXL_LIB_BASE_DIR}/${CONFIGURATION_SCG})

set(AXL_INSTALL_LIB_SUBDIR lib)

if(WIN32)
	set(AXL_INSTALL_INC_SUBDIR     include)
	set(AXL_INSTALL_CMAKE_SUBDIR   cmake)
	set(AXL_INSTALL_DOC_SUBDIR     doc)
	set(AXL_INSTALL_LICENSE_SUBDIR license)
	set(AXL_INSTALL_SHARE_SUBDIR   )
else()
	set(AXL_INSTALL_INC_SUBDIR     include/${PROJECT_NAME})
	set(AXL_INSTALL_CMAKE_SUBDIR   lib/cmake/${PROJECT_NAME})
	set(AXL_INSTALL_DOC_SUBDIR     share/${PROJECT_NAME}/doc)
	set(AXL_INSTALL_LICENSE_SUBDIR share/${PROJECT_NAME}/license)
	set(AXL_INSTALL_SHARE_SUBDIR   share/${PROJECT_NAME})
endif()

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

add_subdirectory(cmake)

include(${CMAKE_CURRENT_BINARY_DIR}/cmake/axl_config.cmake)

add_subdirectory(src)
add_subdirectory(doc)
add_subdirectory(test)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

install(
	DIRECTORY license/
	DESTINATION ${AXL_INSTALL_LICENSE_SUBDIR}
)

#...............................................................................

set(CPACK_PACKAGE_VERSION_MAJOR ${AXL_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${AXL_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${AXL_VERSION_REVISION})

if(WIN32)
	set(CPACK_SOURCE_GENERATOR 7Z)
else()
	set(CPACK_SOURCE_GENERATOR TXZ)
endif()

set(CPACK_SOURCE_IGNORE_FILES "/[.];[.]user$;/build/;/ci/;/paths.cmake$")

set(CPACK_SOURCE_PACKAGE_FILE_NAME ${PROJECT_NAME}-${AXL_VERSION_FILE_SUFFIX}-src)

if(APPLE)
	set(CPACK_PACKAGE_FILE_NAME ${PROJECT_NAME}-${AXL_VERSION_FILE_SUFFIX}-mac)
else()
	string(TOLOWER ${CMAKE_SYSTEM_NAME} OS_NAME)
	set(CPACK_PACKAGE_FILE_NAME ${PROJECT_NAME}-${AXL_VERSION_FILE_SUFFIX}-${OS_NAME}-${TARGET_CPU})
endif()

include(CPack)

#...............................................................................
