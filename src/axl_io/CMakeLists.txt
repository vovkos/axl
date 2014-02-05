# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2013. All rights reserved
# Author: Vladimir Gladkov

project_virtual_folder (axl_io)

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
	${AXL_INC_DIR}/axl_io_MappedFile.h
	${AXL_INC_DIR}/axl_io_PCap.h
	${AXL_INC_DIR}/axl_io_Pipe.h
	${AXL_INC_DIR}/axl_io_Serial.h
	${AXL_INC_DIR}/axl_io_SockAddr.h
	${AXL_INC_DIR}/axl_io_SocketTransport.h
	${AXL_INC_DIR}/axl_io_Transport.h
	)

set (
	IO_CPP_LIST
	axl_io_ConnectionSock.cpp
	axl_io_DgramSock.cpp
	axl_io_File.cpp
	axl_io_FilePathUtils.cpp
	axl_io_ListenerSock.cpp
	axl_io_MappedFile.cpp
	axl_io_PCap.cpp
	axl_io_Pipe.cpp
	axl_io_Serial.cpp
	axl_io_Socket.cpp
	axl_io_Transport.cpp
	)

if (NOT AXL_BUILD_IO_PCAP)
	exclude_from_build (axl_io_PCap.cpp)
endif ()

# temporarily exclude until i port axl_exe

exclude_from_build (
	axl_io_ConnectionSock.cpp
	axl_io_DgramSock.cpp
	axl_io_ListenerSock.cpp
	axl_io_Pipe.cpp
	axl_io_Serial.cpp
	axl_io_Transport.cpp
	)

source_virtual_folder (
	io
	${IO_H_LIST}
	${IO_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# io_psx folder
#

set (
	IO_PSX_H_LIST
	${AXL_INC_DIR}/axl_io_psx_File.h
	${AXL_INC_DIR}/axl_io_psx_Mapping.h
	${AXL_INC_DIR}/axl_io_psx_Pipe.h
	${AXL_INC_DIR}/axl_io_psx_Serial.h
	${AXL_INC_DIR}/axl_io_psx_Socket.h
	)

set (
	IO_PSX_CPP_LIST
	axl_io_psx_File.cpp
	axl_io_psx_Mapping.cpp
	axl_io_psx_Pipe.cpp
	axl_io_psx_Serial.cpp
	axl_io_psx_Socket.cpp
	)

source_virtual_folder (
	io_psx
	${IO_PSX_H_LIST}
	${IO_PSX_CPP_LIST}
	)

if (NOT UNIX)
	exclude_from_build (${IO_PSX_CPP_LIST})
endif ()

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# io_win folder
#

set (
	IO_WIN_H_LIST
	${AXL_INC_DIR}/axl_io_win_File.h
	${AXL_INC_DIR}/axl_io_win_Iocp.h
	${AXL_INC_DIR}/axl_io_win_Mapping.h
	${AXL_INC_DIR}/axl_io_win_Pipe.h
	${AXL_INC_DIR}/axl_io_win_Serial.h
	${AXL_INC_DIR}/axl_io_win_Socket.h
	)

set (
	IO_WIN_CPP_LIST
	axl_io_win_File.cpp
	axl_io_win_Iocp.cpp
	axl_io_win_Mapping.cpp
	axl_io_win_Pipe.cpp
	axl_io_win_Serial.cpp
	axl_io_win_Socket.cpp
	)

source_virtual_folder (
	io_win
	${IO_WIN_H_LIST}
	${IO_WIN_CPP_LIST}
	)

if (NOT WIN32)
	exclude_from_build (${IO_WIN_CPP_LIST})
endif ()

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set (PCH_H   pch.h)
set (PCH_CPP pch.cpp)

source_virtual_folder (
	pch
	${PCH_H}
	${PCH_CPP}
	)

#..............................................................................
#
# axl_io library
#

if (AXL_BUILD_IO_PCAP)
	include_directories (${PATH_PCAP_INC_DIR})
	add_definitions (-D_AXL_IO_PCAP)
endif ()

add_library (
	axl_io
	${PCH_H}
	${PCH_CPP}
	${IO_H_LIST}
	${IO_CPP_LIST}
	${IO_PSX_H_LIST}
	${IO_PSX_CPP_LIST}
	${IO_WIN_H_LIST}
	${IO_WIN_CPP_LIST}
	)

if (AXL_USE_PCH)
	set_pch (
		axl_io
		${PCH_H}
		${PCH_CPP}
		)
endif ()

#..............................................................................
