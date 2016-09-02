# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2014. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................

set (
	AXL_PATH_LIST

	LUA_INC_DIR
	LUA_LIB_DIR
	PCAP_INC_DIR
	PCAP_LIB_DIR
	LIBSSH2_INC_DIR
	LIBSSH2_LIB_DIR
	LIBUSB_INC_DIR
	LIBUSB_LIB_DIR
	OPENSSL_INC_DIR
	OPENSSL_LIB_DIR
	EXPAT_INC_DIR
	EXPAT_LIB_DIR
	QT_CMAKE_DIR
	QT_DLL_DIR
	RAGEL_EXE
	7Z_EXE
	DOXYGEN_EXE
	SPHINX_BUILD_EXE
	QDOC_EXE
	)

set (
	AXL_IMPORT_LIST
	
	OPTIONAL
		ragel 
		lua
		pcap
		libusb
		openssl
		expat
		doxygen
		doxyrest
		sphinx
		qt
		7z
	)

if (UNIX AND NOT APPLE)
	set (AXL_IMPORT_LIST ${AXL_IMPORT_LIST} OPTIONAL libudev)
endif ()

if (DOXYREST_CMAKE_DIR)
	set (_DOXYREST_CMAKE_DIR ${DOXYREST_CMAKE_DIR})
else ()
	set (_DOXYREST_CMAKE_DIR ${CMAKE_CURRENT_LIST_DIR}/../doxyrest/cmake)
endif ()

set (
	AXL_IMPORT_DIR_LIST
	${_DOXYREST_CMAKE_DIR}
	)
	
#..............................................................................
