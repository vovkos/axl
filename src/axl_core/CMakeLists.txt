# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2014. All rights reserved
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
	${AXL_INC_DIR}/axl_err_ParseError.h
	${AXL_INC_DIR}/axl_err_WinError.h
	)

set (
	ERR_CPP_LIST
	axl_err_Errno.cpp
	axl_err_Error.cpp
	axl_err_ErrorMgr.cpp
	axl_err_NtError.cpp
	axl_err_ParseError.cpp
	axl_err_WinError.cpp
	)

source_group (
	err
	FILES
	${ERR_H_LIST}
	${ERR_CPP_LIST}
	)

if (NOT WIN32)
	exclude_from_build (
		axl_err_NtError.cpp
		axl_err_WinError.cpp
		)
endif ()

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# axl_g folder
#

set (
	G_H_LIST
	${AXL_INC_DIR}/axl_g_Def.h
	${AXL_INC_DIR}/axl_g_Module.h
	${AXL_INC_DIR}/axl_g_Pch.h
	${AXL_INC_DIR}/axl_g_Time.h
	${AXL_INC_DIR}/axl_g_WarningSuppression.h
	)

set (
	G_CPP_LIST
	axl_g_Module.cpp
	axl_g_Time.cpp
	)

source_group (
	g
	FILES
	${G_H_LIST}
	${G_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# axl_g_win folder
#

set (
	G_WIN_H_LIST
	${AXL_INC_DIR}/axl_g_win_Handle.h
	${AXL_INC_DIR}/axl_g_win_PerfCounter.h
	${AXL_INC_DIR}/axl_g_win_Window.h
	)

set (
	G_WIN_CPP_LIST
	axl_g_win_Handle.cpp
	)

source_group (
	g_win
	FILES
	${G_WIN_H_LIST}
	${G_WIN_CPP_LIST}
	)

if (NOT WIN32)
	exclude_from_build (${G_WIN_CPP_LIST})
endif ()

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# lex folder
#

set (
	LEX_H_LIST
	${AXL_INC_DIR}/axl_lex_Lexer.h
	${AXL_INC_DIR}/axl_lex_RagelLexer.h
	${AXL_INC_DIR}/axl_lex_Token.h
	)

set (LEX_CPP_LIST)

source_group (
	lex
	FILES
	${LEX_H_LIST}
	${LEX_CPP_LIST}
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
	${AXL_INC_DIR}/axl_mem_HeapAlloc.h
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
# mem_win folder
#

set (
	MEM_WIN_H_LIST
	${AXL_INC_DIR}/axl_mem_win_VirtualMemory.h
	)

set (
	MEM_WIN_CPP_LIST
	axl_mem_win_VirtualMemory.cpp
	)

source_group (
	mem_win
	FILES
	${MEM_WIN_H_LIST}
	${MEM_WIN_CPP_LIST}
	)

if (NOT WIN32)
	exclude_from_build (${MEM_WIN_CPP_LIST})
endif ()

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# mt folder
#

set (
	MT_H_LIST
	${AXL_INC_DIR}/axl_mt_Atomic.h
	${AXL_INC_DIR}/axl_mt_CallOnce.h
	${AXL_INC_DIR}/axl_mt_DynamicLibrary.h
	${AXL_INC_DIR}/axl_mt_Event.h
	${AXL_INC_DIR}/axl_mt_InvSemaphore.h
	${AXL_INC_DIR}/axl_mt_Lock.h
	${AXL_INC_DIR}/axl_mt_LongJmpTry.h
	${AXL_INC_DIR}/axl_mt_Semaphore.h
	${AXL_INC_DIR}/axl_mt_Thread.h
	${AXL_INC_DIR}/axl_mt_TlsMgr.h
	${AXL_INC_DIR}/axl_mt_TlsSlot.h
	)

set (
	MT_CPP_LIST
	axl_mt_TlsMgr.cpp
	)

source_group (
	mt
	FILES
	${MT_H_LIST}
	${MT_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# mt_psx folder
#

set (
	MT_PSX_H_LIST
	${AXL_INC_DIR}/axl_mt_psx_Cond.h
	${AXL_INC_DIR}/axl_mt_psx_DynamicLibrary.h
	${AXL_INC_DIR}/axl_mt_psx_Mutex.h
	${AXL_INC_DIR}/axl_mt_psx_Sem.h
	${AXL_INC_DIR}/axl_mt_psx_Thread.h
	)

set (
	MT_PSX_CPP_LIST
	axl_mt_psx_Cond.cpp
	axl_mt_psx_DynamicLibrary.cpp
	axl_mt_psx_Event.cpp
	axl_mt_psx_Mutex.cpp
	axl_mt_psx_Sem.cpp
	axl_mt_psx_Thread.cpp
	)

source_group (
	mt_psx
	FILES
	${MT_PSX_H_LIST}
	${MT_PSX_CPP_LIST}
	)

if (NOT UNIX)
	exclude_from_build (${MT_PSX_CPP_LIST})
endif ()

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# mt_win folder
#

set (
	MT_WIN_H_LIST
	${AXL_INC_DIR}/axl_mt_win_CriticalSection.h
	${AXL_INC_DIR}/axl_mt_win_Event.h
	${AXL_INC_DIR}/axl_mt_win_JobObject.h
	${AXL_INC_DIR}/axl_mt_win_Library.h
	${AXL_INC_DIR}/axl_mt_win_Semaphore.h
	${AXL_INC_DIR}/axl_mt_win_Thread.h
	${AXL_INC_DIR}/axl_mt_win_WaitableHandle.h
	)

set (
	MT_WIN_CPP_LIST
	axl_mt_win_JobObject.cpp
	axl_mt_win_Thread.cpp
	axl_mt_win_WaitableHandle.cpp
	)

source_group (
	mt_win
	FILES
	${MT_WIN_H_LIST}
	${MT_WIN_CPP_LIST}
	)

if (NOT WIN32)
	exclude_from_build (${MT_WIN_CPP_LIST})
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
# rtl folder
#

set (
	RTL_H_LIST
	${AXL_INC_DIR}/axl_rtl_Array.h
	${AXL_INC_DIR}/axl_rtl_ArrayList.h
	${AXL_INC_DIR}/axl_rtl_AutoPtrArray.h
	${AXL_INC_DIR}/axl_rtl_BinTree.h
	${AXL_INC_DIR}/axl_rtl_BitIdx.h
	${AXL_INC_DIR}/axl_rtl_BitMap.h
	${AXL_INC_DIR}/axl_rtl_BmhFind.h
	${AXL_INC_DIR}/axl_rtl_BoyerMooreFind.h
	${AXL_INC_DIR}/axl_rtl_BoxList.h
	${AXL_INC_DIR}/axl_rtl_BuddyAllocMap.h
	${AXL_INC_DIR}/axl_rtl_ByteOrder.h
	${AXL_INC_DIR}/axl_rtl_CharLiteral.h
	${AXL_INC_DIR}/axl_rtl_CmdLineParser.h
	${AXL_INC_DIR}/axl_rtl_ForEach.h
	${AXL_INC_DIR}/axl_rtl_Func.h
	${AXL_INC_DIR}/axl_rtl_Guid.h
	${AXL_INC_DIR}/axl_rtl_Handle.h
	${AXL_INC_DIR}/axl_rtl_HandleTable.h
	${AXL_INC_DIR}/axl_rtl_HashTable.h
	${AXL_INC_DIR}/axl_rtl_List.h
	${AXL_INC_DIR}/axl_rtl_ListBase.h
	${AXL_INC_DIR}/axl_rtl_Pack.h
	${AXL_INC_DIR}/axl_rtl_Packer.h
	${AXL_INC_DIR}/axl_rtl_Packetizer.h
	${AXL_INC_DIR}/axl_rtl_Protocol.h
	${AXL_INC_DIR}/axl_rtl_RbTree.h
	${AXL_INC_DIR}/axl_rtl_RuntimeType.h
	${AXL_INC_DIR}/axl_rtl_SimpleSingleton.h
	${AXL_INC_DIR}/axl_rtl_Singleton.h
	${AXL_INC_DIR}/axl_rtl_String.h
	${AXL_INC_DIR}/axl_rtl_StringCache.h
	${AXL_INC_DIR}/axl_rtl_StringHashTable.h
	${AXL_INC_DIR}/axl_rtl_Type.h
	${AXL_INC_DIR}/axl_rtl_Unpacker.h
	)

set (
	RTL_CPP_LIST
	axl_rtl_BitMap.cpp	
	axl_rtl_BoyerMooreAccessor.h
	axl_rtl_BoyerMooreFind.cpp
	axl_rtl_BmhFind.cpp
	axl_rtl_BuddyAllocMap.cpp
	axl_rtl_CmdLineParser.cpp
	axl_rtl_Func.cpp
	axl_rtl_Guid.cpp
	axl_rtl_Packer.cpp
	axl_rtl_Packetizer.cpp
	axl_rtl_Unpacker.cpp
	)

source_group (
	rtl
	FILES
	${RTL_H_LIST}
	${RTL_CPP_LIST}
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
# fsm folder
#

set (
	FSM_H_LIST
	${AXL_INC_DIR}/axl_fsm_Nfa.h
	${AXL_INC_DIR}/axl_fsm_Dfa.h
	${AXL_INC_DIR}/axl_fsm_RegExp.h
	${AXL_INC_DIR}/axl_fsm_StdRegExpNameMgr.h
	)

set (
	FSM_CPP_LIST
	axl_fsm_Nfa.cpp
	axl_fsm_Dfa.cpp
	axl_fsm_RegExp.cpp
	axl_fsm_StdRegExpNameMgr.cpp
	)

source_group (
	fsm
	FILES
	${FSM_H_LIST}
	${FSM_CPP_LIST}
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
	${G_WIN_H_LIST}
	${G_WIN_CPP_LIST}
	${LEX_H_LIST}
	${LEX_CPP_LIST}
	${MEM_H_LIST}
	${MEM_CPP_LIST}
	${MEM_WIN_H_LIST}
	${MEM_WIN_CPP_LIST}
	${MT_H_LIST}
	${MT_CPP_LIST}
	${MT_PSX_H_LIST}
	${MT_PSX_CPP_LIST}
	${MT_WIN_H_LIST}
	${MT_WIN_CPP_LIST}
	${OBJ_H_LIST}
	${OBJ_CPP_LIST}
	${REF_H_LIST}
	${REF_CPP_LIST}
	${RTL_H_LIST}
	${RTL_CPP_LIST}
	${ENC_H_LIST}
	${ENC_CPP_LIST}
	${FSM_H_LIST}
	${FSM_CPP_LIST}
	)

set_pch (
	axl_core
	${PCH_H}
	${PCH_CPP}
	)

if (WIN32)
	add_target_string_property (
		axl_core
		STATIC_LIBRARY_FLAGS 
		"/IGNORE:4221"
		)
endif ()

#..............................................................................
