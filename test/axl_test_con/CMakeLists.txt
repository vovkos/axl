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

source_group(
	pch
	FILES
	${PCH_H}
	REGULAR_EXPRESSION cmake_pch
)

#...............................................................................
#
# axl_test_con app
#

include_directories(${AXL_INC_DIR})
link_directories(${AXL_LIB_DIR})

add_definitions(
	-DAXL_ROOT_DIR="${AXL_ROOT_DIR}"
	-DAXL_SHARE_DIR="${AXL_SHARE_DIR}"
)

set(OPT_LIB_LIST)

if(BUILD_AXL_LUA)
	include_directories(${LUA_INC_DIR})
	link_directories(${LUA_LIB_DIR})
	add_definitions(-D_AXL_LUA=1)
	list(APPEND OPT_LIB_LIST axl_lua)
endif()

if(BUILD_AXL_PY)
	include_directories(${PYTHON_INC_DIR})
	link_directories(${PYTHON_LIB_DIR})
	add_definitions(-D_AXL_PY=1)
	list(APPEND OPT_LIB_LIST axl_py ${PYTHON_LIB_NAME})
	configure_file(pythonPaths.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/pythonPaths.h)
	include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)
endif()

if(BUILD_AXL_IO_SHMT)
	add_definitions(-D_AXL_IO_SHMT=1)
	list(APPEND OPT_LIB_LIST axl_io_shmt)
endif()

if(BUILD_AXL_IO_USB)
	include_directories(${LIBUSB_INC_DIR})
	link_directories(${LIBUSB_LIB_DIR})
	add_definitions(-D_AXL_IO_USB=1)
	list(APPEND OPT_LIB_LIST axl_io_usb ${LIBUSB_LIB_NAME})
endif()

if(BUILD_AXL_IO_USBMON)
	if(${CMAKE_GENERATOR} MATCHES "Visual Studio 10")
		include_directories(
			${USBSDK_INC_DIR}
			${HIDSDK_INC_DIR}
		)

		link_directories(${HIDSDK_LIB_DIR})
	endif()

	include_directories(
		${PCAP_INC_PATH}
		${AXL_ROOT_DIR}/src/axl_io_usbmon/include
	)

	add_definitions(-D_AXL_IO_USBMON=1)
	list(APPEND OPT_LIB_LIST axl_io_usbmon)

	if(LINUX)
		list(
			APPEND OPT_LIB_LIST
			axl_io_pcap
			${LIBUSB_LIB_NAME}
			pcap
		)
	endif()
endif()

if(BUILD_AXL_IO_HID)
	include_directories(${HIDAPI_INC_DIR})
	link_directories(${HIDAPI_LIB_DIR})
	add_definitions(-D_AXL_IO_HID=1)
	list(APPEND OPT_LIB_LIST axl_io_hid ${HIDAPI_LIB_NAME})

	if(BUILD_AXL_IO_USBMON AND WIN32)
		list(APPEND OPT_LIB_LIST hid.lib)
	endif()
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

if(BUILD_AXL_RE)
	add_definitions(-D_AXL_RE=1)
	list(APPEND OPT_LIB_LIST axl_re axl_lex)
endif()

if(BUILD_AXL_RE2)
	include_directories(${RE2S_INC_DIR})
	link_directories(${RE2S_LIB_DIR})
	add_definitions(-D_AXL_RE2=1)
	list(
		APPEND OPT_LIB_LIST
		axl_re2 ${RE2S_LIB_NAME}
	)
endif()

if(BUILD_AXL_ZIP)
	add_definitions(-D_AXL_ZIP=1)
	list(APPEND OPT_LIB_LIST axl_zip)
endif()

if(BUILD_AXL_ABR)
	add_definitions(-D_AXL_ABR=1)
	list(APPEND OPT_LIB_LIST axl_abr)
endif()

add_executable(
	axl_test_con
	${PCH_H}
	${APP_H_LIST}
	${APP_CPP_LIST}
)

target_precompile_headers(
	axl_test_con
	PRIVATE
	${PCH_H}
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
	if(LINUX AND BUILD_AXL_IO_USB)
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

		if(BUILD_AXL_IO_HID)
			find_library(APPKIT_FRAMEWORK AppKit)

			target_link_libraries(
				axl_test_con
				${APPKIT_FRAMEWORK}
			)
		endif()
	else()
		target_link_libraries(
			axl_test_con
			pthread
			dl
			rt
		)
	endif()
endif()

if(WIN32)
	set(_DLL_LIST)

	if(BUILD_AXL_LUA AND LUA_DLL_DIR)
		list(APPEND _DLL_LIST ${LUA_DLL_DIR}/${LUA_LIB_NAME}.dll)
	endif()

	if(BUILD_AXL_PY AND PYTHON_DLL_DIR)
		list(APPEND _DLL_LIST ${PYTHON_DLL_DIR}/${PYTHON_LIB_NAME}.dll)
	endif()

	if(BUILD_AXL_IO_USB AND LIBUSB_DLL_DIR)
		list(APPEND _DLL_LIST ${LIBUSB_DLL_DIR}/libusb-1.0.dll)
	endif()

	if(BUILD_AXL_IO_HID AND HIDAPI_DLL_DIR)
		list(APPEND _DLL_LIST ${HIDAPI_DLL_DIR}/hidapi.dll)
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

	axl_copy_dlls_for_target(axl_test_con ${_DLL_LIST})
endif()

#...............................................................................
