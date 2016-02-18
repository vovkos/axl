# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2015. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................
#
# dbg folder
#

set (
	DBG_H_LIST
	${AXL_INC_DIR}/axl_dbg_Trace.h
	)

set (
	DBG_CPP_LIST
	axl_dbg_Trace.cpp
	)

source_group (
	dbg
	FILES
	${DBG_H_LIST}
	${DBG_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# err folder
#

set (
	ERR_H_LIST
	${AXL_INC_DIR}/axl_err_Errno.h
	${AXL_INC_DIR}/axl_err_Error.h
	${AXL_INC_DIR}/axl_err_ErrorMgr.h
	${AXL_INC_DIR}/axl_err_NtError.h
	${AXL_INC_DIR}/axl_err_WinError.h		
	)

set (
	ERR_CPP_LIST
	axl_err_Errno.cpp
	axl_err_Error.cpp
	axl_err_ErrorMgr.cpp
	axl_err_NtError.cpp
	axl_err_WinError.cpp
	)

source_group (
	err
	FILES
	${ERR_H_LIST}
	${ERR_CPP_LIST}
	)

if (NOT WIN32)
	axl_exclude_from_build (
		axl_err_NtError.cpp
		axl_err_WinError.cpp
		)
endif ()

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# g folder
#

set (
	G_H_LIST
	${AXL_INC_DIR}/axl_g_Def.h
	${AXL_INC_DIR}/axl_g_Module.h
	${AXL_INC_DIR}/axl_g_Pch.h
	${AXL_INC_DIR}/axl_g_WarningSuppression.h
	)

set (
	G_CPP_LIST
	axl_g_Module.cpp
	)

source_group (
	g
	FILES
	${G_H_LIST}
	${G_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# mem folder
#

set (
	MEM_H_LIST
	${AXL_INC_DIR}/axl_mem_StdAllocator.h
	${AXL_INC_DIR}/axl_mem_Block.h
	${AXL_INC_DIR}/axl_mem_New.h
	${AXL_INC_DIR}/axl_mem_Malloc.h
	${AXL_INC_DIR}/axl_mem_Tracker.h
	)

set (
	MEM_CPP_LIST
	axl_mem_Tracker.cpp
	)

source_group (
	mem
	FILES
	${MEM_H_LIST}
	${MEM_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# sys folder
#

set (
	SYS_H_LIST
	${AXL_INC_DIR}/axl_sys_Atomic.h
	${AXL_INC_DIR}/axl_sys_Event.h
	${AXL_INC_DIR}/axl_sys_Lock.h
	${AXL_INC_DIR}/axl_sys_LongJmpTry.h
	${AXL_INC_DIR}/axl_sys_Semaphore.h
	${AXL_INC_DIR}/axl_sys_InvSemaphore.h
	${AXL_INC_DIR}/axl_sys_Thread.h
	${AXL_INC_DIR}/axl_sys_TlsMgr.h
	${AXL_INC_DIR}/axl_sys_TlsSlot.h
	${AXL_INC_DIR}/axl_sys_DynamicLibrary.h
	${AXL_INC_DIR}/axl_sys_Process.h
	${AXL_INC_DIR}/axl_sys_Time.h
	)

set (
	SYS_CPP_LIST
	axl_sys_TlsMgr.cpp
	axl_sys_Time.cpp
	)

source_group (
	sys
	FILES
	${SYS_H_LIST}
	${SYS_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# sys_psx folder
#

set (
	SYS_PSX_H_LIST
	${AXL_INC_DIR}/axl_sys_psx_Cond.h
	${AXL_INC_DIR}/axl_sys_psx_Mutex.h
	${AXL_INC_DIR}/axl_sys_psx_Sem.h
	${AXL_INC_DIR}/axl_sys_psx_Thread.h
	${AXL_INC_DIR}/axl_sys_psx_DynamicLibrary.h
	)

set (
	SYS_PSX_CPP_LIST
	axl_sys_psx_Cond.cpp
	axl_sys_psx_Event.cpp
	axl_sys_psx_Mutex.cpp
	axl_sys_psx_Sem.cpp
	axl_sys_psx_Thread.cpp
	axl_sys_psx_DynamicLibrary.cpp
	)

source_group (
	sys_psx
	FILES
	${SYS_PSX_H_LIST}
	${SYS_PSX_CPP_LIST}
	)

if (NOT UNIX)
	axl_exclude_from_build (${SYS_PSX_CPP_LIST})
endif ()

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# sys_win folder
#

set (
	SYS_WIN_H_LIST
	${AXL_INC_DIR}/axl_sys_win_CriticalSection.h
	${AXL_INC_DIR}/axl_sys_win_Event.h
	${AXL_INC_DIR}/axl_sys_win_Semaphore.h
	${AXL_INC_DIR}/axl_sys_win_Thread.h
	${AXL_INC_DIR}/axl_sys_win_WaitableHandle.h
	${AXL_INC_DIR}/axl_sys_win_WaitableTimer.h
	${AXL_INC_DIR}/axl_sys_win_Handle.h
	${AXL_INC_DIR}/axl_sys_win_PerfCounter.h
	${AXL_INC_DIR}/axl_sys_win_JobObject.h
	${AXL_INC_DIR}/axl_sys_win_Library.h
	${AXL_INC_DIR}/axl_sys_win_Service.h
	${AXL_INC_DIR}/axl_sys_win_ServiceMgr.h
	${AXL_INC_DIR}/axl_sys_win_DeviceInfo.h
	${AXL_INC_DIR}/axl_sys_win_Window.h
	${AXL_INC_DIR}/axl_sys_win_Process.h
	${AXL_INC_DIR}/axl_sys_win_AccessToken.h
	${AXL_INC_DIR}/axl_sys_win_Sid.h
	${AXL_INC_DIR}/axl_sys_win_HeapAlloc.h
	${AXL_INC_DIR}/axl_sys_win_VirtualMemory.h
	)

set (
	SYS_WIN_CPP_LIST
	axl_sys_win_Thread.cpp
	axl_sys_win_WaitableHandle.cpp
	axl_sys_win_JobObject.cpp
	axl_sys_win_Library.cpp
	axl_sys_win_Service.cpp
	axl_sys_win_ServiceMgr.cpp
	axl_sys_win_DeviceInfo.cpp
	axl_sys_win_Process.cpp
	axl_sys_win_AccessToken.cpp
	axl_sys_win_Sid.cpp
	axl_sys_win_VirtualMemory.cpp
	)

source_group (
	sys_win
	FILES
	${SYS_WIN_H_LIST}
	${SYS_WIN_CPP_LIST}
	)

if (NOT WIN32)
	axl_exclude_from_build (${SYS_WIN_CPP_LIST})
endif ()

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# sys_lnx folder
#

set (
	SYS_LNX_H_LIST
	${AXL_INC_DIR}/axl_sys_lnx_Udev.h
	)

set (
	SYS_LNX_LIBUDEV_CPP_LIST
	axl_sys_lnx_Udev.cpp
	)

set (
	SYS_LNX_CPP_LIST
	${SYS_LNX_LIBUDEV_CPP_LIST}
	)

source_group (
	sys_lnx
	FILES
	${SYS_LNX_H_LIST}
	${SYS_LNX_CPP_LIST}
	)

if (NOT LINUX)
	axl_exclude_from_build (${SYS_LNX_CPP_LIST})
endif ()

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# sys_drw folder
#

set (
	SYS_DRW_H_LIST
	${AXL_INC_DIR}/axl_sys_drw_MachError.h
	${AXL_INC_DIR}/axl_sys_drw_Semaphore.h
	)

set (
	SYS_DRW_CPP_LIST
	axl_sys_drw_MachError.cpp
	axl_sys_drw_Semaphore.cpp
	)

source_group (
	sys_drw
	FILES
	${SYS_DRW_H_LIST}
	${SYS_DRW_CPP_LIST}
	)

if (NOT APPLE)
	axl_exclude_from_build (${SYS_DRW_CPP_LIST})
endif ()

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# ref folder
#

set (
	REF_H_LIST
	${AXL_INC_DIR}/axl_ref_Buf.h
	${AXL_INC_DIR}/axl_ref_Child.h
	${AXL_INC_DIR}/axl_ref_New.h
	${AXL_INC_DIR}/axl_ref_Ptr.h
	${AXL_INC_DIR}/axl_ref_RefCount.h
	${AXL_INC_DIR}/axl_ref_RtFactory.h
	${AXL_INC_DIR}/axl_ref_WeakPtr.h
	)

set (
	REF_CPP_LIST
	axl_ref_RefCount.cpp
	)

source_group (
	ref
	FILES
	${REF_H_LIST}
	${REF_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# sl folder
#

set (
	SL_H_LIST
	${AXL_INC_DIR}/axl_sl_CallOnce.h
	${AXL_INC_DIR}/axl_sl_Singleton.h
	${AXL_INC_DIR}/axl_sl_SimpleSingleton.h
	${AXL_INC_DIR}/axl_sl_Array.h
	${AXL_INC_DIR}/axl_sl_ArraySlice.h
	${AXL_INC_DIR}/axl_sl_ArrayList.h
	${AXL_INC_DIR}/axl_sl_AutoPtrArray.h
	${AXL_INC_DIR}/axl_sl_BinTree.h
	${AXL_INC_DIR}/axl_sl_BitIdx.h
	${AXL_INC_DIR}/axl_sl_BitMap.h
	${AXL_INC_DIR}/axl_sl_BmhFind.h
	${AXL_INC_DIR}/axl_sl_BoyerMooreFind.h
	${AXL_INC_DIR}/axl_sl_BoxList.h
	${AXL_INC_DIR}/axl_sl_BuddyAllocMap.h
	${AXL_INC_DIR}/axl_sl_ByteOrder.h
	${AXL_INC_DIR}/axl_sl_CharLiteral.h
	${AXL_INC_DIR}/axl_sl_CmdLineParser.h
	${AXL_INC_DIR}/axl_sl_ForEach.h
	${AXL_INC_DIR}/axl_sl_Func.h
	${AXL_INC_DIR}/axl_sl_Guid.h
	${AXL_INC_DIR}/axl_sl_Handle.h
	${AXL_INC_DIR}/axl_sl_HandleTable.h
	${AXL_INC_DIR}/axl_sl_HashTable.h
	${AXL_INC_DIR}/axl_sl_List.h
	${AXL_INC_DIR}/axl_sl_ListBase.h
	${AXL_INC_DIR}/axl_sl_Pack.h
	${AXL_INC_DIR}/axl_sl_Packer.h
	${AXL_INC_DIR}/axl_sl_Packetizer.h
	${AXL_INC_DIR}/axl_sl_Protocol.h
	${AXL_INC_DIR}/axl_sl_RbTree.h
	${AXL_INC_DIR}/axl_sl_RuntimeType.h
	${AXL_INC_DIR}/axl_sl_String.h
	${AXL_INC_DIR}/axl_sl_StringSlice.h
	${AXL_INC_DIR}/axl_sl_StringCache.h
	${AXL_INC_DIR}/axl_sl_StringHashTable.h
	${AXL_INC_DIR}/axl_sl_Type.h
	${AXL_INC_DIR}/axl_sl_Unpacker.h
	)

set (
	SL_CPP_LIST
	axl_sl_BitMap.cpp	
	axl_sl_BoyerMooreAccessor.h
	axl_sl_BoyerMooreFind.cpp
	axl_sl_BmhFind.cpp
	axl_sl_BuddyAllocMap.cpp
	axl_sl_CmdLineParser.cpp
	axl_sl_Func.cpp
	axl_sl_Guid.cpp
	axl_sl_Packer.cpp
	axl_sl_Packetizer.cpp
	axl_sl_Unpacker.cpp
	)

source_group (
	sl
	FILES
	${SL_H_LIST}
	${SL_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# enc folder
#

set (
	ENC_H_LIST
	${AXL_INC_DIR}/axl_enc_Base32Encoding.h
	${AXL_INC_DIR}/axl_enc_CharCodec.h
	${AXL_INC_DIR}/axl_enc_EscapeEncoding.h
	${AXL_INC_DIR}/axl_enc_HexEncoding.h
	${AXL_INC_DIR}/axl_enc_Utf.h
	)

set (
	ENC_CPP_LIST
	axl_enc_Base32Encoding.cpp
	axl_enc_CharCodec.cpp
	axl_enc_EscapeEncoding.cpp
	axl_enc_HexEncoding.cpp
	axl_enc_Utf.cpp
	)

source_group (
	enc
	FILES
	${ENC_H_LIST}
	${ENC_CPP_LIST}
	)

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
# axl_core library
#

include_directories (${AXL_INC_DIR})

if (BUILD_AXL_SYS_LNX_LIBUDEV)
	add_definitions (-D_AXL_SYS_LNX_LIBUDEV)
else ()
	axl_exclude_from_build (${SYS_LNX_LIBUDEV_CPP_LIST})
endif ()

add_library (
	axl_core
	${PCH_H}
	${PCH_CPP}
	${DBG_H_LIST}
	${DBG_CPP_LIST}
	${ERR_H_LIST}
	${ERR_CPP_LIST}
	${G_H_LIST}
	${G_CPP_LIST}
	${MEM_H_LIST}
	${MEM_CPP_LIST}
	${SYS_H_LIST}
	${SYS_CPP_LIST}
	${SYS_PSX_H_LIST}
	${SYS_PSX_CPP_LIST}
	${SYS_WIN_H_LIST}
	${SYS_WIN_CPP_LIST}
	${SYS_LNX_H_LIST}
	${SYS_LNX_CPP_LIST}
	${SYS_DRW_H_LIST}
	${SYS_DRW_CPP_LIST}
	${OBJ_H_LIST}
	${OBJ_CPP_LIST}
	${REF_H_LIST}
	${REF_CPP_LIST}
	${SL_H_LIST}
	${SL_CPP_LIST}
	${ENC_H_LIST}
	${ENC_CPP_LIST}
	)

axl_set_pch (
	axl_core
	${PCH_H}
	${PCH_CPP}
	)

if (GCC)
	axl_add_target_string_property (
		axl_core
		COMPILE_FLAGS
		"-fPIC -fvisibility=hidden"
		)
endif ()

if (WIN32)
	axl_add_target_string_property (
		axl_core
		STATIC_LIBRARY_FLAGS 
		"/IGNORE:4221"
		)
endif ()

#..............................................................................
