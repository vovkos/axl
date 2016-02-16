# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2015. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................

project (axl)

cmake_minimum_required (VERSION 2.8)

set (AXL_ROOT_DIR     ${CMAKE_CURRENT_LIST_DIR})
set (AXL_BIN_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR}/bin)
set (AXL_LIB_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR}/lib)
set (AXL_PACKAGE_DIR  ${CMAKE_CURRENT_BINARY_DIR}/package)

add_subdirectory (cmake)

include (${CMAKE_CURRENT_BINARY_DIR}/cmake/axl_config.cmake)

include (cmake/axl_init.cmake NO_POLICY_SCOPE)
include (paths.cmake)

axl_print_std_settings ()

axl_import (
	OPTIONAL
		ragel 
		lua
		pcap
		libusb
		openssl
		qt
		7z
	)

if (UNIX AND NOT APPLE)
	axl_import (OPTIONAL libudev)
endif ()

#..............................................................................

add_subdirectory (src)

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/test/CMakeLists.txt")
	add_subdirectory (test)
endif ()

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/tools/CMakeLists.txt")
	add_subdirectory (tools)
endif ()

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/samples/CMakeLists.txt")
	add_subdirectory (samples)
endif ()
	
if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/package/CMakeLists.txt")
	add_subdirectory (package)
endif ()

#..............................................................................
