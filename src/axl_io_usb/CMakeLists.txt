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
	${AXL_INC_DIR}/axl_io_usb/axl_io_UsbTransfer.h
	${AXL_INC_DIR}/axl_io_usb/axl_io_UsbError.h
)

set(
	LIB_CPP_LIST
	axl_io_UsbContext.cpp
	axl_io_UsbEventThread.cpp
	axl_io_UsbDevice.cpp
	axl_io_UsbTransfer.cpp
	axl_io_UsbError.cpp
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
set(PCH_CPP pch.cpp)

set(
	PCH_H_LIST
	${PCH_H}
	${AXL_INC_DIR}/axl_io_usb/axl_io_UsbPch.h
)

source_group(
	pch
	FILES
	${PCH_H_LIST}
	${PCH_CPP}
)

#...............................................................................
#
# axl_io library
#

include_directories(
	${LIBUSB_INC_DIR}
	${AXL_INC_DIR}
)

add_library(
	axl_io_usb
	${PCH_H_LIST}
	${PCH_CPP}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
)

axl_set_pch(
	axl_io_usb
	${PCH_H}
	${PCH_CPP}
)

axl_install_redirected_includes(
	${AXL_INSTALL_INC_SUBDIR}
	axl_io_usb
	${LIB_H_LIST}
)

install(
	TARGETS axl_io_usb
	DESTINATION ${AXL_INSTALL_LIB_SUBDIR}
)

#...............................................................................
