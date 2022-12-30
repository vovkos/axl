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
	${AXL_INC_DIR}/axl_io_usbmon/axl_io_UsbMonTransfer.h
	${AXL_INC_DIR}/axl_io_usbmon/axl_io_UsbMonTransferParser.h
	${AXL_INC_DIR}/axl_io_usbmon/axl_io_UsbMonEnumerator.h
)

set(
	LIB_CPP_LIST
	axl_io_UsbMonTransfer.cpp
)

source_group(
	lib
	FILES
	${LIB_H_LIST}
	${LIB_CPP_LIST}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

set(
	LIB_WIN_H_LIST
	${AXL_INC_DIR}/axl_io_usbmon/axl_io_win_UsbPcap.h
	${AXL_INC_DIR}/axl_io_usbmon/axl_io_win_UsbPcapTransferParser.h
)

set(
	LIB_WIN_CPP_LIST
	axl_io_win_UsbPcap.cpp
	axl_io_win_UsbPcapTransferParser.cpp
	axl_io_win_UsbMonEnumerator.cpp
)

source_group(
	lib_win
	FILES
	${LIB_WIN_H_LIST}
	${LIB_WIN_CPP_LIST}
)

if(NOT WIN32)
	axl_exclude_from_build(${LIB_WIN_CPP_LIST})
endif()

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

set(
	LIB_LNX_H_LIST
	${AXL_INC_DIR}/axl_io_usbmon/axl_io_lnx_UsbMon.h
	${AXL_INC_DIR}/axl_io_usbmon/axl_io_lnx_UsbMonTransferParser.h
)

set(
	LIB_LNX_CPP_LIST
	axl_io_lnx_UsbMon.cpp
	axl_io_lnx_UsbMonTransferParser.cpp
	axl_io_lnx_UsbMonEnumerator.cpp
)

source_group(
	lib_lnx
	FILES
	${LIB_LNX_H_LIST}
	${LIB_LNX_CPP_LIST}
)

if(NOT LINUX)
	axl_exclude_from_build(${LIB_LNX_CPP_LIST})
endif()

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set(PCH_H   pch.h)
set(PCH_CPP pch.cpp)

set(
	PCH_H_LIST
	${PCH_H}
)

source_group(
	pch
	FILES
	${PCH_H_LIST}
	${PCH_CPP}
)

#...............................................................................
#
# axl_io_usbmon library
#

include_directories(${AXL_INC_DIR})

if(WIN32)
	include_directories(${USBSDK_INC_PATH})
elseif(LINUX)
	include_directories(${LIBUSB_INC_DIR})
endif()

add_library(
	axl_io_usbmon
	${PCH_H_LIST}
	${PCH_CPP}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
	${LIB_WIN_H_LIST}
	${LIB_WIN_CPP_LIST}
	${LIB_LNX_H_LIST}
	${LIB_LNX_CPP_LIST}
)

axl_set_pch(
	axl_io_usbmon
	${PCH_H}
	${PCH_CPP}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

axl_install_redirected_includes(
	${AXL_INSTALL_INC_SUBDIR}
	axl_io_usbmon
	${LIB_H_LIST}
)

install(
	TARGETS axl_io_usbmon
	DESTINATION ${AXL_INSTALL_LIB_SUBDIR}
)

#...............................................................................
