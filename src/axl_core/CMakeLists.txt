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
# g folder
#

set(
	G_H_LIST
	${AXL_INC_DIR}/axl_g/axl_g_Module.h
	)

set(
	G_CPP_LIST
	axl_g_Module.cpp
	)

source_group(
	g
	FILES
	${G_H_LIST}
	${G_CPP_LIST}
	)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# mem folder
#

set(
	MEM_H_LIST
	${AXL_INC_DIR}/axl_mem/axl_mem_Allocator.h
	${AXL_INC_DIR}/axl_mem/axl_mem_Block.h
	${AXL_INC_DIR}/axl_mem/axl_mem_New.h
	${AXL_INC_DIR}/axl_mem/axl_mem_Pool.h
	${AXL_INC_DIR}/axl_mem/axl_mem_Tracker.h
	${AXL_INC_DIR}/axl_mem/axl_mem_TrackerBlock.h
	${AXL_INC_DIR}/axl_mem/axl_mem_ScatterGather.h
	${AXL_INC_DIR}/axl_mem/axl_mem_ExecutableBlockArena.h
	)

set(
	MEM_CPP_LIST
	axl_mem_Tracker.cpp
	axl_mem_ScatterGather.cpp
	axl_mem_ExecutableBlockArena.cpp
	)

source_group(
	mem
	FILES
	${MEM_H_LIST}
	${MEM_CPP_LIST}
	)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# ref folder
#

set(
	REF_H_LIST
	${AXL_INC_DIR}/axl_ref/axl_ref_Buf.h
	${AXL_INC_DIR}/axl_ref/axl_ref_Child.h
	${AXL_INC_DIR}/axl_ref/axl_ref_New.h
	${AXL_INC_DIR}/axl_ref/axl_ref_Ptr.h
	${AXL_INC_DIR}/axl_ref/axl_ref_RefCount.h
	${AXL_INC_DIR}/axl_ref/axl_ref_RtFactory.h
	${AXL_INC_DIR}/axl_ref/axl_ref_WeakPtr.h
	)

set(
	REF_CPP_LIST
	axl_ref_RefCount.cpp
	)

source_group(
	ref
	FILES
	${REF_H_LIST}
	${REF_CPP_LIST}
	)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# err folder
#

set(
	ERR_H_LIST
	${AXL_INC_DIR}/axl_err/axl_err_Error.h
	${AXL_INC_DIR}/axl_err/axl_err_ErrorMgr.h
	${AXL_INC_DIR}/axl_err/axl_err_SystemError.h
	${AXL_INC_DIR}/axl_err/axl_err_Errno.h
	)

set(
	ERR_CPP_LIST
	axl_err_Errno.cpp
	axl_err_Error.cpp
	axl_err_ErrorMgr.cpp
	)

source_group(
	err
	FILES
	${ERR_H_LIST}
	${ERR_CPP_LIST}
	)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# sl folder
#

set(
	SL_H_LIST
	${AXL_INC_DIR}/axl_sl/axl_sl_ArgType.h
	${AXL_INC_DIR}/axl_sl/axl_sl_Operator.h
	${AXL_INC_DIR}/axl_sl/axl_sl_Construct.h
	${AXL_INC_DIR}/axl_sl/axl_sl_Cmp.h
	${AXL_INC_DIR}/axl_sl/axl_sl_Hash.h
	${AXL_INC_DIR}/axl_sl/axl_sl_Swap.h
	${AXL_INC_DIR}/axl_sl/axl_sl_Closure.h
	${AXL_INC_DIR}/axl_sl/axl_sl_ForEach.h
	${AXL_INC_DIR}/axl_sl/axl_sl_CallOnce.h
	${AXL_INC_DIR}/axl_sl/axl_sl_Singleton.h
	${AXL_INC_DIR}/axl_sl/axl_sl_SimpleSingleton.h
	${AXL_INC_DIR}/axl_sl/axl_sl_Array.h
	${AXL_INC_DIR}/axl_sl/axl_sl_ArrayDetails.h
	${AXL_INC_DIR}/axl_sl/axl_sl_ArrayList.h
	${AXL_INC_DIR}/axl_sl/axl_sl_AutoPtrArray.h
	${AXL_INC_DIR}/axl_sl/axl_sl_BinTree.h
	${AXL_INC_DIR}/axl_sl/axl_sl_BitIdx.h
	${AXL_INC_DIR}/axl_sl/axl_sl_BitMap.h
	${AXL_INC_DIR}/axl_sl/axl_sl_BmhFind.h
	${AXL_INC_DIR}/axl_sl/axl_sl_BoyerMooreFind.h
	${AXL_INC_DIR}/axl_sl/axl_sl_MemMem.h
	${AXL_INC_DIR}/axl_sl/axl_sl_BoxList.h
	${AXL_INC_DIR}/axl_sl/axl_sl_BuddyAllocMap.h
	${AXL_INC_DIR}/axl_sl/axl_sl_ByteOrder.h
	${AXL_INC_DIR}/axl_sl/axl_sl_CharLiteral.h
	${AXL_INC_DIR}/axl_sl/axl_sl_CmdLineParser.h
	${AXL_INC_DIR}/axl_sl/axl_sl_Guid.h
	${AXL_INC_DIR}/axl_sl/axl_sl_Handle.h
	${AXL_INC_DIR}/axl_sl/axl_sl_HandleTable.h
	${AXL_INC_DIR}/axl_sl/axl_sl_HashTable.h
	${AXL_INC_DIR}/axl_sl/axl_sl_Iterator.h
	${AXL_INC_DIR}/axl_sl/axl_sl_List.h
	${AXL_INC_DIR}/axl_sl/axl_sl_MapEntry.h
	${AXL_INC_DIR}/axl_sl/axl_sl_Pack.h
	${AXL_INC_DIR}/axl_sl/axl_sl_Packer.h
	${AXL_INC_DIR}/axl_sl/axl_sl_Packetizer.h
	${AXL_INC_DIR}/axl_sl/axl_sl_Protocol.h
	${AXL_INC_DIR}/axl_sl/axl_sl_RbTree.h
	${AXL_INC_DIR}/axl_sl/axl_sl_String.h
	${AXL_INC_DIR}/axl_sl/axl_sl_StringDetails.h
	${AXL_INC_DIR}/axl_sl/axl_sl_StringHashTable.h
	${AXL_INC_DIR}/axl_sl/axl_sl_Unpacker.h
	${AXL_INC_DIR}/axl_sl/axl_sl_CircularBuffer.h
	)

set(
	SL_CPP_LIST
	axl_sl_BitMap.cpp
	axl_sl_BmhFind.cpp
	axl_sl_BoyerMooreAccessor.h
	axl_sl_BoyerMooreFind.cpp
	axl_sl_MemMem.cpp
	axl_sl_BuddyAllocMap.cpp
	axl_sl_CmdLineParser.cpp
	axl_sl_Guid.cpp
	axl_sl_Hash.cpp
	axl_sl_Packer.cpp
	axl_sl_Packetizer.cpp
	axl_sl_Unpacker.cpp
	axl_sl_CircularBuffer.cpp
	)

source_group(
	sl
	FILES
	${SL_H_LIST}
	${SL_CPP_LIST}
	)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# enc folder
#

set(
	ENC_H_LIST
	${AXL_INC_DIR}/axl_enc/axl_enc_Base32Encoding.h
	${AXL_INC_DIR}/axl_enc/axl_enc_CharCodec.h
	${AXL_INC_DIR}/axl_enc/axl_enc_EscapeEncoding.h
	${AXL_INC_DIR}/axl_enc/axl_enc_HexEncoding.h
	${AXL_INC_DIR}/axl_enc/axl_enc_Utf.h
	${AXL_INC_DIR}/axl_enc/axl_enc_Leb128.h
	icu_def.h
	icu_uchar_props_data.h
	icu_ucase_props_data.h
	)

set(
	ENC_CPP_LIST
	axl_enc_Base32Encoding.cpp
	axl_enc_CharCodec.cpp
	axl_enc_EscapeEncoding.cpp
	axl_enc_HexEncoding.cpp
	axl_enc_Utf.cpp
	axl_enc_Leb128.cpp
	)

source_group(
	enc
	FILES
	${ENC_H_LIST}
	${ENC_CPP_LIST}
	)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# sys folder
#

set(
	SYS_H_LIST
	${AXL_INC_DIR}/axl_sys/axl_sys_Atomic.h
	${AXL_INC_DIR}/axl_sys/axl_sys_Event.h
	${AXL_INC_DIR}/axl_sys/axl_sys_Lock.h
	${AXL_INC_DIR}/axl_sys/axl_sys_SjljTry.h
	${AXL_INC_DIR}/axl_sys/axl_sys_Semaphore.h
	${AXL_INC_DIR}/axl_sys/axl_sys_InvertedSemaphore.h
	${AXL_INC_DIR}/axl_sys/axl_sys_ReadWriteLock.h
	${AXL_INC_DIR}/axl_sys/axl_sys_Thread.h
	${AXL_INC_DIR}/axl_sys/axl_sys_TlsMgr.h
	${AXL_INC_DIR}/axl_sys/axl_sys_TlsSlot.h
	${AXL_INC_DIR}/axl_sys/axl_sys_DynamicLib.h
	${AXL_INC_DIR}/axl_sys/axl_sys_Process.h
	${AXL_INC_DIR}/axl_sys/axl_sys_Time.h
	time64.h
	time64_config.h
	time64_limits.h
	)

set(
	SYS_CPP_LIST
	axl_sys_TlsMgr.cpp
	axl_sys_Time.cpp
	axl_sys_ReadWriteLock.cpp
	)

set(
	SYS_C_LIST
	time64.c
	)

axl_exclude_from_build(${SYS_C_LIST}) # include it directly

source_group(
	sys
	FILES
	${SYS_H_LIST}
	${SYS_CPP_LIST}
	)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# sys_psx folder
#

set(
	SYS_PSX_H_LIST
	${AXL_INC_DIR}/axl_sys/axl_sys_psx_Cond.h
	${AXL_INC_DIR}/axl_sys/axl_sys_psx_Mutex.h
	${AXL_INC_DIR}/axl_sys/axl_sys_psx_Sem.h
	${AXL_INC_DIR}/axl_sys/axl_sys_psx_Thread.h
	${AXL_INC_DIR}/axl_sys/axl_sys_psx_Process.h
	${AXL_INC_DIR}/axl_sys/axl_sys_psx_DynamicLib.h
	)

set(
	SYS_PSX_CPP_LIST
	axl_sys_psx_Cond.cpp
	axl_sys_psx_Mutex.cpp
	axl_sys_psx_Sem.cpp
	axl_sys_psx_Thread.cpp
	axl_sys_psx_Process.cpp
	axl_sys_psx_DynamicLib.cpp
	)

source_group(
	sys_psx
	FILES
	${SYS_PSX_H_LIST}
	${SYS_PSX_CPP_LIST}
	)

if(NOT UNIX)
	axl_exclude_from_build(${SYS_PSX_CPP_LIST})
endif()

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# sys_win folder
#

set(
	SYS_WIN_H_LIST
	${AXL_INC_DIR}/axl_sys/axl_sys_win_Bstr.h
	${AXL_INC_DIR}/axl_sys/axl_sys_win_WinError.h
	${AXL_INC_DIR}/axl_sys/axl_sys_win_NtStatus.h
	${AXL_INC_DIR}/axl_sys/axl_sys_win_NtDll.h
	${AXL_INC_DIR}/axl_sys/axl_sys_win_CriticalSection.h
	${AXL_INC_DIR}/axl_sys/axl_sys_win_Event.h
	${AXL_INC_DIR}/axl_sys/axl_sys_win_Semaphore.h
	${AXL_INC_DIR}/axl_sys/axl_sys_win_Thread.h
	${AXL_INC_DIR}/axl_sys/axl_sys_win_WaitableHandle.h
	${AXL_INC_DIR}/axl_sys/axl_sys_win_WaitableTimer.h
	${AXL_INC_DIR}/axl_sys/axl_sys_win_Handle.h
	${AXL_INC_DIR}/axl_sys/axl_sys_win_PerfCounter.h
	${AXL_INC_DIR}/axl_sys/axl_sys_win_JobObject.h
	${AXL_INC_DIR}/axl_sys/axl_sys_win_Library.h
	${AXL_INC_DIR}/axl_sys/axl_sys_win_Service.h
	${AXL_INC_DIR}/axl_sys/axl_sys_win_ServiceMgr.h
	${AXL_INC_DIR}/axl_sys/axl_sys_win_DeviceInfo.h
	${AXL_INC_DIR}/axl_sys/axl_sys_win_Process.h
	${AXL_INC_DIR}/axl_sys/axl_sys_win_AccessToken.h
	${AXL_INC_DIR}/axl_sys/axl_sys_win_Sid.h
	${AXL_INC_DIR}/axl_sys/axl_sys_win_VirtualMemory.h
	${AXL_INC_DIR}/axl_sys/axl_sys_win_WinTrust.h
	${AXL_INC_DIR}/axl_sys/axl_sys_win_WinCrypt.h
	${AXL_INC_DIR}/axl_sys/axl_sys_win_Certificate.h
	${AXL_INC_DIR}/axl_sys/axl_sys_win_CryptMsg.h
	${AXL_INC_DIR}/axl_sys/axl_sys_win_Authenticode.h
	)

set(
	SYS_WIN_CPP_LIST
	axl_sys_win_Bstr.cpp
	axl_sys_win_WinError.cpp
	axl_sys_win_NtStatus.cpp
	axl_sys_win_NtDll.cpp
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
	axl_sys_win_WinTrust.cpp
	axl_sys_win_WinCrypt.cpp
	axl_sys_win_Certificate.cpp
	axl_sys_win_CryptMsg.cpp
	axl_sys_win_Authenticode.cpp
	)

source_group(
	sys_win
	FILES
	${SYS_WIN_H_LIST}
	${SYS_WIN_CPP_LIST}
	)

if(NOT WIN32)
	axl_exclude_from_build(${SYS_WIN_CPP_LIST})
endif()

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# sys_lnx folder
#

set(
	SYS_LNX_H_LIST
	${AXL_INC_DIR}/axl_sys/axl_sys_lnx_Udev.h
	${AXL_INC_DIR}/axl_sys/axl_sys_lnx_UdevContext.h
	${AXL_INC_DIR}/axl_sys/axl_sys_lnx_UdevDevice.h
	${AXL_INC_DIR}/axl_sys/axl_sys_lnx_UdevMonitor.h
	${AXL_INC_DIR}/axl_sys/axl_sys_lnx_UdevEnumerator.h
	${AXL_INC_DIR}/axl_sys/axl_sys_lnx_Plt.h
	)

set(
	SYS_LNX_LIBUDEV_CPP_LIST
	axl_sys_lnx_Udev.cpp
	)

if(NOT BUILD_AXL_SYS_LNX_LIBUDEV)
	axl_exclude_from_build(${SYS_LNX_LIBUDEV_CPP_LIST})
elseif(BUILD_AXL_SYS_LNX_LIBUDEV_NEW_API)
	add_definitions(-D_AXL_SYS_LNX_LIBUDEV_NEW_API=1)
endif()

set(
	SYS_LNX_CPP_LIST
	${SYS_LNX_LIBUDEV_CPP_LIST}
	axl_sys_lnx_Plt.cpp
	)

source_group(
	sys_lnx
	FILES
	${SYS_LNX_H_LIST}
	${SYS_LNX_CPP_LIST}
	)

if(NOT LINUX)
	axl_exclude_from_build(${SYS_LNX_CPP_LIST})
endif()

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# sys_drw folder
#

set(
	SYS_DRW_H_LIST
	${AXL_INC_DIR}/axl_sys/axl_sys_drw_MachError.h
	${AXL_INC_DIR}/axl_sys/axl_sys_drw_Semaphore.h
	${AXL_INC_DIR}/axl_sys/axl_sys_drw_Dyld.h
	)

set(
	SYS_DRW_CPP_LIST
	axl_sys_drw_MachError.cpp
	axl_sys_drw_Semaphore.cpp
	axl_sys_drw_Dyld.cpp
	)

source_group(
	sys_drw
	FILES
	${SYS_DRW_H_LIST}
	${SYS_DRW_CPP_LIST}
	)

if(NOT APPLE)
	axl_exclude_from_build(${SYS_DRW_CPP_LIST})
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
	${AXL_INC_DIR}/axl_g/axl_g_Pch.h
	)

source_group(
	pch
	FILES
	${PCH_H_LIST}
	${PCH_CPP}
	)

#...............................................................................
#
# axl_core library
#

include_directories(${AXL_INC_DIR})

add_library(
	axl_core
	${PCH_H_LIST}
	${PCH_CPP}
	${G_H_LIST}
	${G_CPP_LIST}
	${MEM_H_LIST}
	${MEM_CPP_LIST}
	${REF_H_LIST}
	${REF_CPP_LIST}
	${ERR_H_LIST}
	${ERR_CPP_LIST}
	${SL_H_LIST}
	${SL_CPP_LIST}
	${ENC_H_LIST}
	${ENC_CPP_LIST}
	${SYS_H_LIST}
	${SYS_CPP_LIST}
	${SYS_C_LIST}
	${SYS_WIN_H_LIST}
	${SYS_WIN_CPP_LIST}
	${SYS_PSX_H_LIST}
	${SYS_PSX_CPP_LIST}
	${SYS_LNX_H_LIST}
	${SYS_LNX_CPP_LIST}
	${SYS_DRW_H_LIST}
	${SYS_DRW_CPP_LIST}
	)

axl_set_pch(
	axl_core
	${PCH_H}
	${PCH_CPP}
	)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

axl_install_redirected_includes(
	${AXL_INSTALL_INC_SUBDIR}
	axl_g
	${G_H_LIST}
	)

axl_install_redirected_includes(
	${AXL_INSTALL_INC_SUBDIR}
	axl_err
	${ERR_H_LIST}
	)

axl_install_redirected_includes(
	${AXL_INSTALL_INC_SUBDIR}
	axl_mem
	${MEM_H_LIST}
	)

axl_install_redirected_includes(
	${AXL_INSTALL_INC_SUBDIR}
	axl_ref
	${REF_H_LIST}
	)

axl_install_redirected_includes(
	${AXL_INSTALL_INC_SUBDIR}
	axl_sl
	${SL_H_LIST}
	)

axl_install_redirected_includes(
	${AXL_INSTALL_INC_SUBDIR}
	axl_enc
	${ENC_H_LIST}
	)

axl_install_redirected_includes(
	${AXL_INSTALL_INC_SUBDIR}
	axl_sys
	${SYS_H_LIST}
	)

if(WIN32)
	axl_install_redirected_includes(
		${AXL_INSTALL_INC_SUBDIR}
		axl_sys_win
		${SYS_WIN_H_LIST}
		)
endif()

if(UNIX)
	axl_install_redirected_includes(
		${AXL_INSTALL_INC_SUBDIR}
		axl_sys_psx
		${SYS_PSX_H_LIST}
		)
endif()

if(LINUX)
	axl_install_redirected_includes(
		${AXL_INSTALL_INC_SUBDIR}
		axl_sys_lnx
		${SYS_LNX_H_LIST}
		)
endif()

if(APPLE)
	axl_install_redirected_includes(
		${AXL_INSTALL_INC_SUBDIR}
		axl_sys_drw
		${SYS_DRW_H_LIST}
		)
endif()

install(
	TARGETS axl_core
	DESTINATION ${AXL_INSTALL_LIB_SUBDIR}
	)

#...............................................................................
