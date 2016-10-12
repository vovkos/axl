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
	DOXYREST_CMAKE_DIR
	SPHINX_BUILD_EXE
	PDFLATEX_EXE
	)

set (
	AXL_IMPORT_LIST
	
	OPTIONAL
		lua
		pcap
		libusb
		openssl
		expat

		ragel 
		doxygen
		doxyrest
		sphinx
		latex
		qt
		7z
	)

if (UNIX AND NOT APPLE)
	set (AXL_IMPORT_LIST ${AXL_IMPORT_LIST} OPTIONAL libudev)
endif ()

set (
	AXL_IMPORT_DIR_LIST

	${DOXYREST_CMAKE_DIR}
	)
	
#..............................................................................
