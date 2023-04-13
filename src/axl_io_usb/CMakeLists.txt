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
# io_usb folder
#

set(
	LIB_H_LIST
	${AXL_INC_DIR}/axl_io_usb/axl_io_UsbContext.h
	${AXL_INC_DIR}/axl_io_usb/axl_io_UsbEventThread.h
	${AXL_INC_DIR}/axl_io_usb/axl_io_UsbDevice.h
	${AXL_INC_DIR}/axl_io_usb/axl_io_UsbDeviceList.h
	${AXL_INC_DIR}/axl_io_usb/axl_io_UsbDeviceStrings.h
	${AXL_INC_DIR}/axl_io_usb/axl_io_UsbEnumerator.h
	${AXL_INC_DIR}/axl_io_usb/axl_io_UsbTransfer.h
	${AXL_INC_DIR}/axl_io_usb/axl_io_UsbError.h
)

set(
	LIB_CPP_LIST
	axl_io_UsbContext.cpp
	axl_io_UsbEventThread.cpp
	axl_io_UsbDevice.cpp
	axl_io_UsbDeviceStrings.cpp
	axl_io_UsbEnumerator.cpp
	axl_io_UsbTransfer.cpp
	axl_io_UsbError.cpp
	axl_io_win_UsbHub.cpp
)

set(
	LIB_WIN_H_LIST
	${AXL_INC_DIR}/axl_io_usb/axl_io_win_UsbHub.h
)

set(
	LIB_WIN_CPP_LIST
	axl_io_win_UsbHub.cpp
)

set(
	LIB_LNX_H_LIST
)

set(
	LIB_LNX_CPP_LIST
)

source_group(
	lib
	FILES
	${LIB_H_LIST}
	${LIB_CPP_LIST}
	${LIB_WIN_H_LIST}
	${LIB_WIN_CPP_LIST}
)

if(NOT WIN32)
	axl_exclude_from_build(${LIB_WIN_CPP_LIST})
endif()

if(NOT LINUX)
	axl_exclude_from_build(${LIB_LNX_CPP_LIST})
endif()

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set(PCH_H   pch.h)

set(
	PCH_H_LIST
	${PCH_H}
	${AXL_INC_DIR}/axl_io_usb/axl_io_UsbPch.h
)

source_group(
	pch
	FILES
	${PCH_H_LIST}
)

#...............................................................................
#
# axl_io library
#

include_directories(
	${LIBUSB_INC_DIR}
	${AXL_INC_DIR}
)

if(WIN32)
	include_directories(${USBSDK_INC_PATH})
endif()

add_library(
	axl_io_usb
	${PCH_H_LIST}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
	${LIB_WIN_H_LIST}
	${LIB_WIN_CPP_LIST}
	${LIB_LNX_H_LIST}
	${LIB_LNX_CPP_LIST}
)

target_precompile_headers(
	axl_io_usb
	PRIVATE
	${PCH_H}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

add_library(
	axl_io_usb_error
	axl_io_UsbError.cpp
)

target_precompile_headers(
	axl_io_usb_error
	REUSE_FROM
	axl_io_usb
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

axl_install_redirected_includes(
	${AXL_INSTALL_INC_SUBDIR}
	axl_io_usb
	${LIB_H_LIST}
)

install(
	TARGETS
		axl_io_usb
		axl_io_usb_error
	DESTINATION ${AXL_INSTALL_LIB_SUBDIR}
)

#...............................................................................
