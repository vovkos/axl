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

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
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

#...............................................................................
#
# axl_test_con app
#

include_directories (${AXL_INC_DIR})
link_directories (${AXL_LIB_DIR})

if (BUILD_AXL_IO_USB)
	include_directories (${LIBUSB_INC_DIR})
	link_directories (${LIBUSB_LIB_DIR})
	add_definitions (-D_AXL_IO_USB=1)
endif ()

if (BUILD_AXL_XML)
	include_directories (${EXPAT_INC_DIR})
	link_directories (${EXPAT_LIB_DIR})
	add_definitions (-D_AXL_XML=1)
endif ()

add_executable (
	axl_test_con
	${PCH_H}
	${PCH_CPP}
	${APP_H_LIST}
	${APP_CPP_LIST}
	)

set_target_properties (
	axl_test_con
	PROPERTIES
	FOLDER test
	)

if (BUILD_AXL_XML)
	target_link_libraries (
		axl_test_con
		axl_xml
		expat
		)
endif ()

target_link_libraries (
	axl_test_con
	axl_io
	axl_fsm
	axl_zip
	axl_core
	)

if (WIN32)
	target_link_libraries (
		axl_test_con
		setupapi
		)
endif ()

if (UNIX)
	target_link_libraries (
		axl_test_con
		pthread
		dl
		)

	if (LINUX AND LIBUDEV_FOUND)
		target_link_libraries (
			axl_test_con
			udev
			)
	endif ()

	if (APPLE)
		find_library (CORE_FOUNDATION_FRAMEWORK CoreFoundation)
		find_library (IOKIT_FRAMEWORK IOKit)

		target_link_libraries (
			axl_test_con
			axl_cf
			axl_iok
			${CORE_FOUNDATION_FRAMEWORK}
			${IOKIT_FRAMEWORK}
			)
	else ()
		target_link_libraries (
			axl_test_con
			rt
			)
	endif ()
endif ()

if (BUILD_AXL_IO_USB)
	if (WIN32)
		target_link_libraries (
			axl_test_con
			libusb-1.0
			)
	else ()
		target_link_libraries (
			axl_test_con
			usb-1.0
			)
	endif ()
endif ()

axl_set_pch (
	axl_test_con
	${PCH_H}
	${PCH_CPP}
	)

set_target_properties (
	axl_test_con
	PROPERTIES
	FOLDER test
	)

#...............................................................................
