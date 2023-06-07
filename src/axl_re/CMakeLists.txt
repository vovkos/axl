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
	${AXL_INC_DIR}/axl_re/axl_re_Nfa.h
	${AXL_INC_DIR}/axl_re/axl_re_Dfa.h
	${AXL_INC_DIR}/axl_re/axl_re_Regex.h
	${AXL_INC_DIR}/axl_re/axl_re_Exec.h
	${AXL_INC_DIR}/axl_re/axl_re_Match.h
	${AXL_INC_DIR}/axl_re/axl_re_StateInit.h
	${AXL_INC_DIR}/axl_re/axl_re_State.h
	${AXL_INC_DIR}/axl_re/axl_re_Storage.h
	axl_re_StdCharClass.h
	axl_re_Lexer.h
	axl_re_Compiler.h
	axl_re_ExecDfa.h
	axl_re_ExecNfaSp.h
	axl_re_ExecNfaVm.h
)

set(
	LIB_CPP_LIST
	axl_re_CharSet.cpp
	axl_re_Nfa.cpp
	axl_re_Dfa.cpp
	axl_re_Regex.cpp
	axl_re_Lexer.cpp
	axl_re_Compiler.cpp
	axl_re_State.cpp
	axl_re_Exec.cpp
	axl_re_ExecDfa.cpp
	axl_re_ExecNfaSp.cpp
	axl_re_ExecNfaVm.cpp
)

set(
	LIB_RL_LIST
	axl_re_Lexer.rl
)

source_group(
	lib
	FILES
	${LIB_H_LIST}
	${LIB_CPP_LIST}
	${LIB_RL_LIST}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# gen folder
#

set(GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen)
file(MAKE_DIRECTORY ${GEN_DIR})

axl_push_and_set(CMAKE_CURRENT_BINARY_DIR ${GEN_DIR})

add_ragel_step(
	axl_re_Lexer.rl.cpp
	axl_re_Lexer.rl
)

axl_pop(CMAKE_CURRENT_BINARY_DIR)

set(
	GEN_RL_CPP_LIST
	${GEN_DIR}/axl_re_Lexer.rl.cpp
)

axl_exclude_from_build(${GEN_RL_CPP_LIST})  # include "*.rl.cpp" manually

source_group(
	gen
	FILES
	${GEN_RL_CPP_LIST}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set(PCH_H   pch.h)

set(
	PCH_H_LIST
	${PCH_H}
	${AXL_INC_DIR}/axl_re/axl_re_Pch.h
)

source_group(
	pch
	FILES
	${PCH_H_LIST}
	REGULAR_EXPRESSION cmake_pch
)

#...............................................................................
#
# axl_re library (finite state machines)
#

include_directories(
	${AXL_INC_DIR}
	${GEN_DIR}
)

add_library(
	axl_re
	${PCH_H_LIST}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
	${LIB_RL_LIST}
	${GEN_RL_CPP_LIST}
)

target_precompile_headers(
	axl_re
	PRIVATE
	${PCH_H}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

axl_install_redirected_includes(
	${AXL_INSTALL_INC_SUBDIR}
	${PCH_H_LIST}
	${LIB_H_LIST}
)

install(
	TARGETS axl_re
	DESTINATION ${AXL_INSTALL_LIB_SUBDIR}
)

#...............................................................................
