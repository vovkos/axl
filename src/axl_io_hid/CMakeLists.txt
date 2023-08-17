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
# lib folder
#

set(
	LIB_H_LIST
	${AXL_INC_DIR}/axl_io_hid/axl_io_HidDevice.h
	${AXL_INC_DIR}/axl_io_hid/axl_io_HidEnumerator.h
	${AXL_INC_DIR}/axl_io_hid/axl_io_HidRd.h
	${AXL_INC_DIR}/axl_io_hid/axl_io_HidDb.h
)

set(
	LIB_CPP_LIST
	axl_io_HidDevice.cpp
	axl_io_HidRd.cpp
	axl_io_HidRdParser.h
	axl_io_HidRdParser.cpp
	axl_io_HidDb.cpp
	axl_io_HidDbParser.h
	axl_io_HidDbParser.cpp
)

source_group(
	lib
	FILES
	${LIB_H_LIST}
	${LIB_CPP_LIST}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set(PCH_H   pch.h)

set(
	PCH_H_LIST
	${PCH_H}
	${AXL_INC_DIR}/axl_io_hid/axl_io_HidPch.h
)

source_group(
	pch
	FILES
	${PCH_H_LIST}
	REGULAR_EXPRESSION cmake_pch
)

#...............................................................................
#
# axl_io_hid library
#

include_directories(
	${HIDAPI_INC_DIR}
	${AXL_INC_DIR}
)

add_library(
	axl_io_hid
	${PCH_H_LIST}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

axl_install_redirected_includes(
	${AXL_INSTALL_INC_SUBDIR}
	${PCH_H_LIST}
	${LIB_H_LIST}
)

install(
	TARGETS axl_io_hid
	DESTINATION ${AXL_INSTALL_LIB_SUBDIR}
)

#...............................................................................
