# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2013. All rights reserved
# Author: Vladimir Gladkov

set (VIRTUAL_FOLDER_SUB_DIR axl_exe)

#..............................................................................
#
# exe folder
#

set (
	EXE_H_LIST
	${AXL_INC_DIR}/axl_exe_Arg.h
	${AXL_INC_DIR}/axl_exe_ArgPacker.h
	${AXL_INC_DIR}/axl_exe_Function.h
	${AXL_INC_DIR}/axl_exe_InvokeDirect.h
	${AXL_INC_DIR}/axl_exe_InvokeList.h
	${AXL_INC_DIR}/axl_exe_ProxyFunction.h
	${AXL_INC_DIR}/axl_exe_ScheduledFunction.h
	${AXL_INC_DIR}/axl_exe_Scheduler.h
	${AXL_INC_DIR}/axl_exe_Synchronizer.h
	${AXL_INC_DIR}/axl_exe_Thunk.h
	${AXL_INC_DIR}/axl_exe_Timer.h
	${AXL_INC_DIR}/axl_exe_WorkerThread.h
	${AXL_INC_DIR}/axl_exe_WorkerThreadPool.h
	${AXL_INC_DIR}/axl_exe_WorkerWindow.h
	)

set (
	EXE_CPP_LIST
	axl_exe_ArgPacker.cpp
	axl_exe_InvokeList.cpp
	axl_exe_Scheduler.cpp
	axl_exe_Synchronizer.cpp
	axl_exe_Thunk.cpp
	axl_exe_Timer.cpp
	axl_exe_WorkerThread.cpp
	axl_exe_WorkerThreadPool.cpp
	axl_exe_WorkerWindow.cpp
	)

set (
	EXE_ASM_LIST
	axl_exe_InvokeDirect_x64.asm
	axl_exe_InvokeDirect_x86.asm
	)

virtual_folder (
	exe
	${EXE_H_LIST}
	${EXE_CPP_LIST}
	${EXE_ASM_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set (PCH_H   pch.h)
set (PCH_CPP pch.cpp)

virtual_folder (
	pch
	${PCH_H}
	${PCH_CPP}
	)

#..............................................................................
#
# axl_exe library
#

add_library (
	axl_exe
	${PCH_H}
	${PCH_CPP}
	${EXE_H_LIST}
	${EXE_CPP_LIST}
	${EXE_ASM_LIST}
	)

set_pch (
	axl_exe
	${PCH_H}
	${PCH_CPP}
	)

#..............................................................................
