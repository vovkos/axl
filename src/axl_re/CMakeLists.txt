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
	${AXL_INC_DIR}/axl_re/axl_re_CharSet.h
	${AXL_INC_DIR}/axl_re/axl_re_CharRangeMap.h
	${AXL_INC_DIR}/axl_re/axl_re_FsmMatchCondition.h
	${AXL_INC_DIR}/axl_re/axl_re_Nfa.h
	${AXL_INC_DIR}/axl_re/axl_re_Dfa.h
	${AXL_INC_DIR}/axl_re/axl_re_Regex.h
	${AXL_INC_DIR}/axl_re/axl_re_RegexNameMgr.h
	${AXL_INC_DIR}/axl_re/axl_re_RegexCompiler.h
	${AXL_INC_DIR}/axl_re/axl_re_RegexState.h
)

set(
	LIB_CPP_LIST
	axl_re_CharSet.cpp
	axl_re_FsmMatchCondition.cpp
	axl_re_Nfa.cpp
	axl_re_Dfa.cpp
	axl_re_Regex.cpp
	axl_re_RegexCompiler.cpp
	axl_re_RegexState.cpp
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
	${AXL_INC_DIR}/axl_re/axl_re_Pch.h
)

source_group(
	pch
	FILES
	${PCH_H_LIST}
	${PCH_CPP}
)

#...............................................................................
#
# axl_re library (finite state machines)
#

include_directories(${AXL_INC_DIR})

add_library(
	axl_re
	${PCH_H_LIST}
	${PCH_CPP}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
)

axl_set_pch(
	axl_re
	${PCH_H}
	${PCH_CPP}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

axl_install_redirected_includes(
	${AXL_INSTALL_INC_SUBDIR}
	axl_re
	${LIB_H_LIST}
)

install(
	TARGETS axl_re
	DESTINATION ${AXL_INSTALL_LIB_SUBDIR}
)

#...............................................................................
