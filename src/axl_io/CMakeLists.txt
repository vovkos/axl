# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2015. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................
#
# io folder
#

set (
	IO_H_LIST
	${AXL_INC_DIR}/axl_io_ConnectionSock.h
	${AXL_INC_DIR}/axl_io_DgramSock.h
	${AXL_INC_DIR}/axl_io_File.h
	${AXL_INC_DIR}/axl_io_FilePathUtils.h
	${AXL_INC_DIR}/axl_io_FileTransport.h
	${AXL_INC_DIR}/axl_io_ListenerSock.h
	${AXL_INC_DIR}/axl_io_Mapping.h
	${AXL_INC_DIR}/axl_io_MappedFile.h
	${AXL_INC_DIR}/axl_io_SharedMemoryTransport.h
	${AXL_INC_DIR}/axl_io_Pipe.h
	${AXL_INC_DIR}/axl_io_Serial.h
	${AXL_INC_DIR}/axl_io_SockAddr.h
	${AXL_INC_DIR}/axl_io_Socket.h
	${AXL_INC_DIR}/axl_io_SocketTransport.h
	${AXL_INC_DIR}/axl_io_Transport.h
	${AXL_INC_DIR}/axl_io_NetworkAdapter.h
	axl_io_SockAddrParser.h
	)

set (
	IO_CPP_LIST
	axl_io_ConnectionSock.cpp
	axl_io_DgramSock.cpp
	axl_io_File.cpp
	axl_io_FilePathUtils.cpp
	axl_io_ListenerSock.cpp
	axl_io_Mapping.cpp
	axl_io_MappedFile.cpp
	axl_io_Pipe.cpp
	axl_io_Serial.cpp
	axl_io_SharedMemoryTransport.cpp
	axl_io_SockAddrParser.cpp
	axl_io_SockAddr.cpp
	axl_io_Socket.cpp
	axl_io_Transport.cpp
	axl_io_NetworkAdapter.cpp
	)

# temporarily exclude until i port axl_exe

axl_exclude_from_build (
	axl_io_ConnectionSock.cpp
	axl_io_DgramSock.cpp
	axl_io_ListenerSock.cpp
	axl_io_Pipe.cpp
	axl_io_Transport.cpp
	)

source_group (
	io
	FILES
	${IO_H_LIST}
	${IO_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# io_pcap folder
#

set (
	IO_PCAP_H_LIST
	${AXL_INC_DIR}/axl_io_PCap.h
	)

set (
	IO_PCAP_CPP_LIST
	axl_io_PCap.cpp
	)

source_group (
	io_pcap
	FILES
	${IO_PCAP_H_LIST}
	${IO_PCAP_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# io_usb folder
#

set (
	IO_USB_H_LIST
	${AXL_INC_DIR}/axl_io_UsbDevice.h
	${AXL_INC_DIR}/axl_io_UsbTransfer.h
	${AXL_INC_DIR}/axl_io_UsbError.h
	)

set (
	IO_USB_CPP_LIST
	axl_io_UsbDevice.cpp
	axl_io_UsbTransfer.cpp
	axl_io_UsbError.cpp
	)

source_group (
	io_usb
	FILES
	${IO_USB_H_LIST}
	${IO_USB_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# io_psx folder
#

set (
	IO_PSX_H_LIST
	${AXL_INC_DIR}/axl_io_psx_Fd.h
	${AXL_INC_DIR}/axl_io_psx_File.h
	${AXL_INC_DIR}/axl_io_psx_Mapping.h
	${AXL_INC_DIR}/axl_io_psx_Pipe.h
	${AXL_INC_DIR}/axl_io_psx_Serial.h
	${AXL_INC_DIR}/axl_io_psx_Socket.h
	)

set (
	IO_PSX_CPP_LIST
	axl_io_psx_Fd.cpp
	axl_io_psx_File.cpp
	axl_io_psx_Mapping.cpp
	axl_io_psx_Pipe.cpp
	axl_io_psx_Serial.cpp
	axl_io_psx_Socket.cpp
	axl_io_psx_NetworkAdapterEnumerator.cpp
	)

source_group (
	io_psx
	FILES
	${IO_PSX_H_LIST}
	${IO_PSX_CPP_LIST}
	)

if (NOT UNIX)
	axl_exclude_from_build (${IO_PSX_CPP_LIST})
endif ()

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# io_win folder
#

set (
	IO_WIN_H_LIST
	${AXL_INC_DIR}/axl_io_win_FileHandle.h
	${AXL_INC_DIR}/axl_io_win_File.h
	${AXL_INC_DIR}/axl_io_win_Iocp.h
	${AXL_INC_DIR}/axl_io_win_Mapping.h
	${AXL_INC_DIR}/axl_io_win_NamedPipe.h
	${AXL_INC_DIR}/axl_io_win_Serial.h
	${AXL_INC_DIR}/axl_io_win_Socket.h
	)

set (
	IO_WIN_CPP_LIST
	axl_io_win_FileHandle.cpp
	axl_io_win_File.cpp
	axl_io_win_Iocp.cpp
	axl_io_win_Mapping.cpp
	axl_io_win_NamedPipe.cpp
	axl_io_win_Serial.cpp
	axl_io_win_SerialPortEnumerator.cpp
	axl_io_win_Socket.cpp
	axl_io_win_NetworkAdapterEnumerator.cpp
	)

source_group (
	io_win
	FILES
	${IO_WIN_H_LIST}
	${IO_WIN_CPP_LIST}
	)

if (NOT WIN32)
	axl_exclude_from_build (${IO_WIN_CPP_LIST})
endif ()

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# io_lnx folder
#

set (
	IO_LNX_H_LIST
	)

set (
	IO_LNX_CPP_LIST
	axl_io_lnx_SerialPortEnumerator.cpp
	)

source_group (
	io_lnx
	FILES
	${IO_LNX_H_LIST}
	${IO_LNX_CPP_LIST}
	)

if (NOT LINUX)
	axl_exclude_from_build (${IO_LNX_CPP_LIST})
endif ()

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# io_drw folder
#

set (
	IO_DRW_H_LIST
	)

set (
	IO_DRW_CPP_LIST
	axl_io_drw_SerialPortEnumerator.cpp
	)

source_group (
	io_drw
	FILES
	${IO_DRW_H_LIST}
	${IO_DRW_CPP_LIST}
	)

if (NOT APPLE)
	axl_exclude_from_build (${IO_DRW_CPP_LIST})
endif ()

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
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

#..............................................................................
#
# axl_io library
#

include_directories (${AXL_INC_DIR})

if (BUILD_AXL_IO_PCAP)
	include_directories (${PCAP_INC_DIR})
	add_definitions (-D_AXL_IO_PCAP)
else ()
	axl_exclude_from_build (${IO_PCAP_CPP_LIST})
endif ()

if (BUILD_AXL_IO_USB)
	include_directories (${LIBUSB_INC_DIR})
	add_definitions (-D_AXL_IO_USB)
else ()
	axl_exclude_from_build (${IO_USB_CPP_LIST})
endif ()

if (BUILD_AXL_SYS_LNX_LIBUDEV)
	add_definitions (-D_AXL_SYS_LNX_LIBUDEV)
endif ()

add_library (
	axl_io
	${PCH_H}
	${PCH_CPP}
	${IO_H_LIST}
	${IO_CPP_LIST}
	${IO_PCAP_H_LIST}
	${IO_PCAP_CPP_LIST}
	${IO_USB_H_LIST}
	${IO_USB_CPP_LIST}
	${IO_WIN_H_LIST}
	${IO_WIN_CPP_LIST}
	${IO_PSX_H_LIST}
	${IO_PSX_CPP_LIST}
	${IO_LNX_H_LIST}
	${IO_LNX_CPP_LIST}
	${IO_DRW_H_LIST}
	${IO_DRW_CPP_LIST}
	)

axl_set_pch (
	axl_io
	${PCH_H}
	${PCH_CPP}
	)

if (WIN32)
	axl_add_target_string_property (
		axl_io
		STATIC_LIBRARY_FLAGS 
		"/IGNORE:4221"
		)
endif ()

if (GCC)
	axl_add_target_string_property (
		axl_io
		COMPILE_FLAGS
		"-fPIC"
		)
endif ()

#..............................................................................
