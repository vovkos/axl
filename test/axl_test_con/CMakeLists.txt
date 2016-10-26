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
	test_Crypto.cpp
	test_CmdLineParser.cpp
	test_Error.cpp
	test_File.cpp
	test_Finalizer.cpp
	test_ForEach.cpp
	test_Function.cpp
	test_HandleTable.cpp
	test_HashTable.cpp
	test_IniParser.cpp
	test_List.cpp
	test_Log.cpp
	test_Packer.cpp
	test_Packetizer.cpp
	test_Parse.cpp
	test_RbTree.cpp
	test_Ref.cpp
	test_Scheduler.cpp
	test_Sock.cpp
	test_String.cpp
	test_StringTemplate.cpp
	test_Timer.cpp
	test_Usb.cpp
	test_Vso.cpp
	test_Window.cpp
	test_WorkerThread.cpp
	)

source_group (
	src
	FILES
	${SRC_H_LIST}
	${SRC_CPP_LIST}
	)

axl_exclude_from_build (
	test_Log.cpp
	test_Packer.cpp
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
# axl_test_con compiler test
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
	axl_test_con
	${PCH_H}
	${PCH_CPP}
	${SRC_H_LIST}
	${SRC_CPP_LIST}
	)

target_link_libraries (
	axl_test_con
	axl_core
	axl_gui
	axl_ini
	axl_io
	axl_lua
	axl_cry
	lua
	libeay32MTd
	)

if (WIN32)
	target_link_libraries (
		axl_test_con
		libusb-1.0
		)
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
