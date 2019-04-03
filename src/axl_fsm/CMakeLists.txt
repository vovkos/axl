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
	${AXL_INC_DIR}/axl_fsm/axl_fsm_Nfa.h
	${AXL_INC_DIR}/axl_fsm/axl_fsm_Dfa.h
	${AXL_INC_DIR}/axl_fsm/axl_fsm_Regex.h
	${AXL_INC_DIR}/axl_fsm/axl_fsm_StdRegexNameMgr.h
	)

set(
	LIB_CPP_LIST
	axl_fsm_Nfa.cpp
	axl_fsm_Dfa.cpp
	axl_fsm_Regex.cpp
	axl_fsm_StdRegexNameMgr.cpp
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
	${AXL_INC_DIR}/axl_fsm/axl_fsm_Pch.h
	)

source_group(
	pch
	FILES
	${PCH_H_LIST}
	${PCH_CPP}
	)

#...............................................................................
#
# axl_fsm library (finite state machines)
#

include_directories(${AXL_INC_DIR})

add_library(
	axl_fsm
	${PCH_H_LIST}
	${PCH_CPP}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
	)

axl_set_pch(
	axl_fsm
	${PCH_H}
	${PCH_CPP}
	)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

axl_install_redirected_includes(
	${AXL_INSTALL_INC_SUBDIR}
	axl_fsm
	${LIB_H_LIST}
	)

install(
	TARGETS axl_fsm
	DESTINATION ${AXL_INSTALL_LIB_SUBDIR}
	)

#...............................................................................
