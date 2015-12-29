# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2015. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................
#
# app folder
#

set (
	APP_H_LIST
	)

set (
	APP_CPP_LIST
	main.cpp
	)

source_group (
	app
	FILES
	${APP_H_LIST}
	${APP_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set (PCH_H   pch.h)
set (PCH_CPP pch.cpp)

source_group (
	pch
	FILES
	${PCH_H}
	${PCH_CPP}
	)
	
#..............................................................................
#
# test_mini app
#

include_directories (${AXL_INC_DIR})
link_directories (${AXL_LIB_DIR})

if (BUILD_AXL_IO_USB)	
	include_directories (${LIBUSB_INC_DIR})
	link_directories (${LIBUSB_LIB_DIR})
	add_definitions (-D_AXL_IO_USB)
endif ()

add_executable (
	test_mini
	${PCH_H}
	${PCH_CPP}
	${APP_H_LIST}
	${APP_CPP_LIST}
	)

set_target_properties (
	test_mini
	PROPERTIES
	FOLDER test
	)
	
target_link_libraries (
	test_mini
	axl_io
	axl_core
	axl_zip
	)

if (WIN32)
	target_link_libraries (
		test_mini
		setupapi
		)
endif ()

if (UNIX)
	target_link_libraries (
		test_mini
		pthread
		dl
		)

	if (NOT APPLE)
		target_link_libraries (
			test_mini
			rt
			)
	endif ()
endif ()

if (BUILD_AXL_IO_USB)
	if (UNIX)
		target_link_libraries (
			test_mini
			usb-1.0
			)
	elseif (WIN32)
		target_link_libraries (
			test_mini
			libusb-1.0
			)
	else ()
		target_link_libraries (
			test_mini
			usb-1.0
			)
	endif ()
endif ()

set_pch (
	test_mini
	${PCH_H}
	${PCH_CPP}
	)

#..............................................................................
