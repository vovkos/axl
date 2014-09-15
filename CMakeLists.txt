# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2014. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................

project (axl)

cmake_minimum_required (VERSION 2.8)

include (${CMAKE_CURRENT_LIST_DIR}/../cmake/Master.cmake)

print_std_settings ()

check_and_print_std_libs (
	RAGEL
	LUA
	PCAP
	LIBUSB
	QT
	)

if (QT_FOUND)
	unset (Qt5Core_DIR    CACHE)
	unset (Qt5Gui_DIR     CACHE)
	unset (Qt5Widgets_DIR CACHE)

	include ("${QT_CMAKE_DIR}/Qt5Core/Qt5CoreConfig.cmake")
	include ("${QT_CMAKE_DIR}/Qt5Widgets/Qt5WidgetsConfig.cmake")
endif ()

configure_file (
	cmake/axl.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/cmake/axl.cmake
	)

configure_file (
	cmake/axl.cmake.in
	${CMAKE_CURRENT_LIST_DIR}/cmake/latest/axl-${TARGET_CPU}.cmake
	)

configure_file (
	cmake/axl-latest.cmake.in
	${CMAKE_CURRENT_LIST_DIR}/cmake/latest/axl.cmake
	)

include (${CMAKE_CURRENT_BINARY_DIR}/cmake/axl.cmake)

include_directories (${AXL_INC_DIR})

add_subdirectory (src)
add_subdirectory (tests)

#..............................................................................
