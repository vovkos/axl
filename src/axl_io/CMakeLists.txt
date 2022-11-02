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
	${AXL_INC_DIR}/axl_io/axl_io_File.h
	${AXL_INC_DIR}/axl_io/axl_io_FilePathUtils.h
	${AXL_INC_DIR}/axl_io/axl_io_FileEnumerator.h
	${AXL_INC_DIR}/axl_io/axl_io_Mapping.h
	${AXL_INC_DIR}/axl_io/axl_io_MappedFile.h
	${AXL_INC_DIR}/axl_io/axl_io_SharedMemoryTransport.h
	${AXL_INC_DIR}/axl_io/axl_io_Serial.h
	${AXL_INC_DIR}/axl_io/axl_io_SerialSettings.h
	${AXL_INC_DIR}/axl_io/axl_io_SockAddr.h
	${AXL_INC_DIR}/axl_io/axl_io_Socket.h
	${AXL_INC_DIR}/axl_io/axl_io_NetworkAdapter.h
	axl_io_SockAddrParser.h
)

set(
	LIB_CPP_LIST
	axl_io_File.cpp
	axl_io_FilePathUtils.cpp
	axl_io_FileEnumerator.cpp
	axl_io_Mapping.cpp
	axl_io_MappedFile.cpp
	axl_io_Serial.cpp
	axl_io_SerialSettings.cpp
	axl_io_SharedMemoryTransport.cpp
	axl_io_SockAddrParser.cpp
	axl_io_SockAddr.cpp
	axl_io_Socket.cpp
	axl_io_NetworkAdapter.cpp
)

source_group(
	lib
	FILES
	${LIB_H_LIST}
	${LIB_CPP_LIST}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# lib_psx folder
#

set(
	LIB_PSX_H_LIST
	${AXL_INC_DIR}/axl_io/axl_io_psx_File.h
	${AXL_INC_DIR}/axl_io/axl_io_psx_Pty.h
	${AXL_INC_DIR}/axl_io/axl_io_psx_Mapping.h
	${AXL_INC_DIR}/axl_io/axl_io_psx_Pipe.h
	${AXL_INC_DIR}/axl_io/axl_io_psx_Serial.h
	${AXL_INC_DIR}/axl_io/axl_io_psx_Socket.h
)

set(
	LIB_PSX_CPP_LIST
	axl_io_psx_File.cpp
	axl_io_psx_Pty.cpp
	axl_io_psx_Mapping.cpp
	axl_io_psx_Pipe.cpp
	axl_io_psx_Serial.cpp
	axl_io_psx_Socket.cpp
	axl_io_psx_NetworkAdapterEnumerator.cpp
)

source_group(
	lib_psx
	FILES
	${LIB_PSX_H_LIST}
	${LIB_PSX_CPP_LIST}
)

if(NOT UNIX)
	axl_exclude_from_build(${LIB_PSX_CPP_LIST})
endif()

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# lib_win folder
#

set(
	LIB_WIN_H_LIST
	${AXL_INC_DIR}/axl_io/axl_io_win_File.h
	${AXL_INC_DIR}/axl_io/axl_io_win_Iocp.h
	${AXL_INC_DIR}/axl_io/axl_io_win_Mapping.h
	${AXL_INC_DIR}/axl_io/axl_io_win_NamedPipe.h
	${AXL_INC_DIR}/axl_io/axl_io_win_Serial.h
	${AXL_INC_DIR}/axl_io/axl_io_win_Socket.h
)

set(
	LIB_WIN_CPP_LIST
	axl_io_win_File.cpp
	axl_io_win_Iocp.cpp
	axl_io_win_Mapping.cpp
	axl_io_win_NamedPipe.cpp
	axl_io_win_Serial.cpp
	axl_io_win_SerialPortEnumerator.cpp
	axl_io_win_Socket.cpp
	axl_io_win_NetworkAdapterEnumerator.cpp
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
#
# lib_lnx folder
#

set(
	LIB_LNX_H_LIST
)

set(
	LIB_LNX_CPP_LIST
	axl_io_lnx_SerialPortEnumerator.cpp
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
# lib_drw folder
#

set(
	LIB_DRW_H_LIST
)

set(
	LIB_DRW_CPP_LIST
	axl_io_drw_SerialPortEnumerator.cpp
)

source_group(
	lib_drw
	FILES
	${LIB_DRW_H_LIST}
	${LIB_DRW_CPP_LIST}
)

if(NOT APPLE)
	axl_exclude_from_build(${LIB_DRW_CPP_LIST})
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
	${AXL_INC_DIR}/axl_io/axl_io_Pch.h
	${AXL_INC_DIR}/axl_io/axl_io_psx_Pch.h
	${AXL_INC_DIR}/axl_io/axl_io_win_Pch.h
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

include_directories(${AXL_INC_DIR})

add_library(
	axl_io
	${PCH_H_LIST}
	${PCH_CPP}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
	${LIB_WIN_H_LIST}
	${LIB_WIN_CPP_LIST}
	${LIB_PSX_H_LIST}
	${LIB_PSX_CPP_LIST}
	${LIB_LNX_H_LIST}
	${LIB_LNX_CPP_LIST}
	${LIB_DRW_H_LIST}
	${LIB_DRW_CPP_LIST}
)

axl_set_pch(
	axl_io
	${PCH_H}
	${PCH_CPP}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

axl_install_redirected_includes(
	${AXL_INSTALL_INC_SUBDIR}
	axl_io
	${LIB_H_LIST}
)

if(WIN32)
	axl_install_redirected_includes(
		${AXL_INSTALL_INC_SUBDIR}
		axl_io_win
		${LIB_WIN_H_LIST}
	)
endif()

if(UNIX)
	axl_install_redirected_includes(
		${AXL_INSTALL_INC_SUBDIR}
		axl_io_psx
		${LIB_PSX_H_LIST}
	)
endif()

install(
	TARGETS axl_io
	DESTINATION ${AXL_INSTALL_LIB_SUBDIR}
)

#...............................................................................
