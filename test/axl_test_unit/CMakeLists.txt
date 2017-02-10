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
# src folder
#

set (
	SRC_H_LIST
	test.h
	)

set (
	SRC_CPP_LIST
	main.cpp
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
	)

source_group (
	src
	FILES
	${SRC_H_LIST}
	${SRC_CPP_LIST}
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
# axl_test_unit compiler test
#

include_directories (
	${LUA_INC_DIR}
	${LIBUSB_INC_DIR}
	${OPENSSL_INC_DIR}
	${AXL_INC_DIR}
	)

link_directories (
	${LUA_LIB_DIR}
	${LIBUSB_LIB_DIR}
	${OPENSSL_LIB_DIR}
	)

add_executable (
	axl_test_unit
	${PCH_H}
	${PCH_CPP}
	${SRC_H_LIST}
	${SRC_CPP_LIST}
	)

target_link_libraries (
	axl_test_unit
	axl_core
	axl_gui
	axl_ini
	axl_io
	axl_lua
	axl_cry
	axl_lex
	axl_st
	${LUA_LIB_NAME}
	libeay32MTd
	)

if (WIN32)
	target_link_libraries (
		axl_test_unit
		libusb-1.0
		)
endif ()

axl_set_pch (
	axl_test_unit
	${PCH_H}
	${PCH_CPP}
	)

set_target_properties (
	axl_test_unit
	PROPERTIES
	FOLDER test
	)

#...............................................................................

file (
	GLOB
	TEST_CPP_LIST
	RELATIVE ${CMAKE_CURRENT_LIST_DIR}
	test_*.cpp
	)

foreach (_FILE_NAME ${TEST_CPP_LIST})
	get_filename_component (_TEST_NAME ${_FILE_NAME} NAME_WE)

	add_test (
		NAME ${_TEST_NAME}
		WORKING_DIRECTORY ${_DIR}
		COMMAND $<TARGET_FILE:axl_test_unit> ${_TEST_NAME}
		)
endforeach ()

#...............................................................................
