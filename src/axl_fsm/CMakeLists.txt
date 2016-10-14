# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2015. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................
#
# lib folder
#

set (
	LIB_H_LIST
	${AXL_INC_DIR}/axl_fsm_Nfa.h
	${AXL_INC_DIR}/axl_fsm_Dfa.h
	${AXL_INC_DIR}/axl_fsm_RegExp.h
	${AXL_INC_DIR}/axl_fsm_StdRegExpNameMgr.h
	)

set (
	LIB_CPP_LIST
	axl_fsm_Nfa.cpp
	axl_fsm_Dfa.cpp
	axl_fsm_RegExp.cpp
	axl_fsm_StdRegExpNameMgr.cpp
	)

source_group (
	lib
	FILES
	${LIB_H_LIST}
	${LIB_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set (PCH_H   pch.h)
set (PCH_CPP pch.cpp)

set (
	PCH_H_LIST
	${PCH_H}
	${AXL_INC_DIR}/axl_fsm_Pch.h
	)

source_group (
	pch
	FILES
	${PCH_H_LIST}
	${PCH_CPP}
	)

#..............................................................................
#
# axl_fsm library (finite state machines)
#

include_directories (${AXL_INC_DIR})

add_library (
	axl_fsm
	${PCH_H_LIST}
	${PCH_CPP}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
	)

axl_set_pch (
	axl_fsm
	${PCH_H}
	${PCH_CPP}
	)

#..............................................................................
