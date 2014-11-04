# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2014. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................

project (axl)

cmake_minimum_required (VERSION 2.8)

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/cmake/std/master.cmake")
	include ("${CMAKE_CURRENT_LIST_DIR}/cmake/std/master.cmake")
elseif (EXISTS "${CMAKE_CURRENT_LIST_DIR}/../cmake/master.cmake")
	include ("${CMAKE_CURRENT_LIST_DIR}/../cmake/master.cmake")
else ()
	message (FATAL_ERROR "master.cmake could not be found")
endif ()

print_std_settings ()

check_and_print_libs (
	OPTIONAL
		RAGEL
		LUA
		PCAP
		LIBUSB
		OPENSSL
		QT
	)

if (QT_FOUND)
	unset (Qt5Core_DIR    CACHE)
	unset (Qt5Gui_DIR     CACHE)
	unset (Qt5Widgets_DIR CACHE)

	include ("${QT_CMAKE_DIR}/Qt5Core/Qt5CoreConfig.cmake")
	include ("${QT_CMAKE_DIR}/Qt5Widgets/Qt5WidgetsConfig.cmake")
endif ()

#..............................................................................

set (AXL_ROOT_DIR     ${CMAKE_CURRENT_LIST_DIR})
set (AXL_LIB_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR}/lib)
set (AXL_PACKAGE_DIR  ${CMAKE_CURRENT_BINARY_DIR}/package)

add_subdirectory (cmake)

include (${CMAKE_CURRENT_BINARY_DIR}/cmake/axl.cmake)
include (version.cmake)

include_directories (${AXL_INC_DIR})

add_subdirectory (src)

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/test/CMakeLists.txt")
	add_subdirectory (test)
endif ()	

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/samples/CMakeLists.txt")
	add_subdirectory (samples)
endif ()	
	
if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/package/CMakeLists.txt")
	add_subdirectory (package)
endif ()	

#..............................................................................
