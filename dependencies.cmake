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

set(
	AXL_PATH_LIST

	LUA_INC_DIR
	LUA_LIB_DIR
	LUA_LIB_NAME
	PCAP_INC_DIR
	PCAP_LIB_DIR
	LIBSSH2_INC_DIR
	LIBSSH2_LIB_DIR
	LIBUSB_INC_DIR
	LIBUSB_LIB_DIR
	USBSDK_INC_PATH
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

set(
	AXL_IMPORT_LIST

	OPTIONAL
		lua
		pcap
		libusb
		openssl
		expat
		qt
		ragel
		doxygen
		doxyrest
		sphinx
		latex
		7z
	)

set(
	AXL_IMPORT_DIR_LIST

	${DOXYREST_CMAKE_DIR}
)

#...............................................................................
