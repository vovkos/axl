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
	test.h
)

set(
	APP_CPP_LIST
	main.cpp
	test_BitMap.cpp
	test_BitMask.cpp
	test_BoyerMooreFind.cpp
	test_CmdLineParser.cpp
	test_Crypto.cpp
	test_Error.cpp
	test_Finalizer.cpp
	test_HandleTable.cpp
	test_HashTable.cpp
	test_IniParser.cpp
	test_List.cpp
	test_Packer.cpp
	test_Packetizer.cpp
	test_RbTree.cpp
	test_Ref.cpp
	test_StringTemplate.cpp
	test_Usb.cpp
	test_Bstr.cpp
)

source_group(
	app
	FILES
	${APP_H_LIST}
	${APP_CPP_LIST}
)

if(NOT WIN32)
	axl_exclude_from_build(test_Bstr.cpp)
endif()

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
# axl_test_auto automated testing app
#

include_directories(${AXL_INC_DIR})
link_directories(${AXL_LIB_DIR})

set(OPT_LIB_LIST)

if(BUILD_AXL_CRY)
	include_directories(${OPENSSL_INC_DIR})
	link_directories(${OPENSSL_LIB_DIR})
	add_definitions(-D_AXL_CRY=1)
	list(APPEND OPT_LIB_LIST axl_cry ${OPENSSL_CRYPTO_LIB_NAME})
else()
	axl_exclude_from_build(test_Crypto.cpp)
endif()

if(BUILD_AXL_IO_USB)
	include_directories(${LIBUSB_INC_DIR})
	link_directories(${LIBUSB_LIB_DIR})
	add_definitions(-D_AXL_IO_USB=1)
	list(APPEND OPT_LIB_LIST axl_io_usb ${LIBUSB_LIB_NAME})
else()
	axl_exclude_from_build(test_Usb.cpp)
endif()

if(BUILD_AXL_ST)
	include_directories(${LUA_INC_DIR})
	link_directories(${LUA_LIB_DIR})
	add_definitions(-D_AXL_LUA=1)
	list(APPEND OPT_LIB_LIST axl_st axl_lua ${LUA_LIB_NAME})
else()
	axl_exclude_from_build(test_StringTemplate.cpp)
endif()

if(BUILD_AXL_INI)
	list(APPEND OPT_LIB_LIST axl_ini)
else()
	axl_exclude_from_build(test_IniParser.cpp)
endif()

add_executable(
	axl_test_auto
	${PCH_H}
	${APP_H_LIST}
	${APP_CPP_LIST}
)

target_precompile_headers(
	axl_test_auto
	PRIVATE
	${PCH_H}
)

set_target_properties(
	axl_test_auto
	PROPERTIES
	FOLDER test
)

target_link_libraries(
	axl_test_auto
	${OPT_LIB_LIST}
	axl_lex
	axl_io
	axl_core
)

if(UNIX)
	if(LINUX AND BUILD_AXL_IO_USB)
		target_link_libraries(
			axl_test_auto
			udev
		)
	endif()

	if(APPLE)
		find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
		find_library(IOKIT_FRAMEWORK IOKit)
		find_library(SECURITY_FRAMEWORK Security)

		target_link_libraries(
			axl_test_auto
			axl_cf
			axl_iok
			${CORE_FOUNDATION_FRAMEWORK}
			${IOKIT_FRAMEWORK}
			${SECURITY_FRAMEWORK}
		)
	else()
		target_link_libraries(
			axl_test_auto
			pthread
			dl
			rt
		)
	endif()
endif()

if(WIN32)
	set(_DLL_LIST)

	if(BUILD_AXL_CRY AND OPENSSL_DLL_DIR)
		list(APPEND _DLL_LIST ${OPENSSL_DLL_DIR}/${OPENSSL_CRYPTO_DLL_NAME}.dll)
	endif()

	if(BUILD_AXL_IO_USB AND LIBUSB_DLL_DIR)
		list(APPEND _DLL_LIST ${LIBUSB_DLL_DIR}/libusb-1.0.dll)
	endif()

	if(BUILD_AXL_LUA AND LUA_DLL_DIR)
		list(APPEND _DLL_LIST ${LUA_DLL_DIR}/${LUA_LIB_NAME}.dll)
	endif()

	axl_copy_dlls_for_target(axl_test_auto ${_DLL_LIST})
endif()

#...............................................................................

file(
	GLOB
	TEST_CPP_LIST
	RELATIVE ${CMAKE_CURRENT_LIST_DIR}
	test_*.cpp
)

foreach(_FILE_NAME ${TEST_CPP_LIST})
	axl_is_excluded_from_build(_IS_EXCLUDED ${_FILE_NAME})

	if(NOT _IS_EXCLUDED)
		get_filename_component(_TEST_NAME ${_FILE_NAME} NAME_WE)

		add_test(
			NAME axl-${_TEST_NAME}
			WORKING_DIRECTORY ${_DIR}
			COMMAND axl_test_auto ${_TEST_NAME}
		)
	endif()
endforeach()

#...............................................................................
