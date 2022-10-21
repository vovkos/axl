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

set(
	APP_H_LIST
)

set(
	APP_CPP_LIST
	main.cpp
)

source_group(
	app
	FILES
	${APP_H_LIST}
	${APP_CPP_LIST}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set(PCH_H   pch.h)
set(PCH_CPP pch.cpp)

source_group(
	pch
	FILES
	${PCH_H}
	${PCH_CPP}
)

#...............................................................................
#
# axl_test_con app
#

include_directories(${AXL_INC_DIR})
link_directories(${AXL_LIB_DIR})

set(OPT_LIB_LIST)

if(BUILD_AXL_IO_USB)
	include_directories(${LIBUSB_INC_DIR})
	link_directories(${LIBUSB_LIB_DIR})
	add_definitions(-D_AXL_IO_USB=1)
	list(APPEND OPT_LIB_LIST axl_io_usb ${LIBUSB_LIB_NAME})
endif()

if(BUILD_AXL_IO_WIN_USBPCAP)
	add_definitions(-D_AXL_IO_WIN_USBPCAP=1)
	list(APPEND OPT_LIB_LIST axl_io_win_usbpcap)
endif()

if(BUILD_AXL_IO_PCAP)
	include_directories(${PCAP_INC_DIR})
	link_directories(${PCAP_LIB_DIR})
	add_definitions(-D_AXL_IO_PCAP=1)

	if(WIN32)
		set(PCAP_LIB_NAME wpcap)
	else()
		set(PCAP_LIB_NAME pcap)
	endif()

	list(APPEND OPT_LIB_LIST axl_io_pcap ${PCAP_LIB_NAME})
endif()

if(BUILD_AXL_CRY)
	include_directories(${OPENSSL_INC_DIR})
	link_directories(${OPENSSL_LIB_DIR})
	add_definitions(-D_AXL_CRY=1)

	if(BUILD_AXL_SYS_CA)
		add_definitions(-D_AXL_SYS_CA=1)
		list(APPEND OPT_LIB_LIST axl_sys_ca)
	endif()

	if(BUILD_AXL_IO_SSL)
		add_definitions(-D_AXL_IO_SSL=1)
		list(APPEND OPT_LIB_LIST axl_io_ssl ${OPENSSL_SSL_LIB_NAME})
	endif()

	list(APPEND OPT_LIB_LIST axl_cry ${OPENSSL_CRYPTO_LIB_NAME})
endif()

if(BUILD_AXL_XML)
	include_directories(${EXPAT_INC_DIR})
	link_directories(${EXPAT_LIB_DIR})
	add_definitions(-D_AXL_XML=1)
	list(APPEND OPT_LIB_LIST axl_xml expat)
endif()

if(BUILD_AXL_SPY)
	add_definitions(-D_AXL_SPY=1)
	list(APPEND OPT_LIB_LIST axl_spy)
endif()

if(BUILD_AXL_INI)
	add_definitions(-D_AXL_INI=1)
	list(APPEND OPT_LIB_LIST axl_ini)
endif()

if(BUILD_AXL_SEC)
	add_definitions(-D_AXL_SEC=1)
	list(APPEND OPT_LIB_LIST axl_sec)
endif()

if (BUILD_AXL_RE)
	add_definitions(-D_AXL_RE=1)
	list(APPEND OPT_LIB_LIST axl_re axl_lex)
endif()

if (BUILD_AXL_ZIP)
	add_definitions(-D_AXL_ZIP=1)
	list(APPEND OPT_LIB_LIST axl_zip)
endif()

add_executable(
	axl_test_con
	${PCH_H}
	${PCH_CPP}
	${APP_H_LIST}
	${APP_CPP_LIST}
)

axl_set_pch(
	axl_test_con
	${PCH_H}
	${PCH_CPP}
)

set_target_properties(
	axl_test_con
	PROPERTIES
	FOLDER test
)

target_link_libraries(
	axl_test_con
	${OPT_LIB_LIST}
	axl_io
	axl_core
)

if(WIN32)
	target_link_libraries(
		axl_test_con
		setupapi
		psapi
	)

	if(BUILD_AXL_IO_PCAP)
		set_target_properties(
			axl_test_con
			PROPERTIES
			LINK_FLAGS "/DELAYLOAD:wpcap.dll"
		)
	endif()
endif()

if(UNIX)
	target_link_libraries(
		axl_test_con
		pthread
		dl
	)

	if(LINUX AND LIBUDEV_FOUND)
		target_link_libraries(
			axl_test_con
			udev
		)
	endif()

	if(APPLE)
		find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
		find_library(IOKIT_FRAMEWORK IOKit)
		find_library(SECURITY_FRAMEWORK Security)

		target_link_libraries(
			axl_test_con
			axl_cf
			axl_iok
			${CORE_FOUNDATION_FRAMEWORK}
			${IOKIT_FRAMEWORK}
			${SECURITY_FRAMEWORK}
		)
	else()
		target_link_libraries(
			axl_test_con
			rt
		)
	endif()
endif()

if(WIN32)
	if(BUILD_AXL_IO_USB AND LIBUSB_DLL_DIR)
		list(APPEND _DLL_LIST ${LIBUSB_DLL_DIR}/libusb-1.0.dll)
	endif()

	if(BUILD_AXL_IO_SSL AND OPENSSL_DLL_DIR)
		list(
			APPEND _DLL_LIST
			${OPENSSL_DLL_DIR}/${OPENSSL_SSL_DLL_NAME}.dll
			${OPENSSL_DLL_DIR}/${OPENSSL_CRYPTO_DLL_NAME}.dll
		)
	endif()

	if(BUILD_AXL_XML AND EXPAT_DLL_DIR)
		list(APPEND _DLL_LIST ${EXPAT_DLL_DIR}/expat.dll)
	endif()

	if(_DLL_LIST)
		add_custom_command(
			TARGET axl_test_con
			POST_BUILD
			COMMAND
				echo Copying DLL files for axl_test_con...
			COMMAND
				${CMAKE_COMMAND} -E copy
				${_DLL_LIST}
				${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CONFIGURATION}
			)
	endif()
endif()

#...............................................................................
