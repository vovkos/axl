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
	${AXL_INC_DIR}/axl_dox/axl_dox_Block.h
	${AXL_INC_DIR}/axl_dox/axl_dox_Group.h
	${AXL_INC_DIR}/axl_dox/axl_dox_Module.h
	${AXL_INC_DIR}/axl_dox/axl_dox_Parser.h
	${AXL_INC_DIR}/axl_dox/axl_dox_Host.h
	axl_dox_Lexer.h
)

set(
	LIB_CPP_LIST
	axl_dox_Block.cpp
	axl_dox_Group.cpp
	axl_dox_Module.cpp
	axl_dox_Lexer.cpp
	axl_dox_Parser.cpp
)

set(
	LIB_RL_LIST
	axl_dox_Lexer.rl
)

source_group(
	dox
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
	axl_dox_Lexer.rl.cpp
	axl_dox_Lexer.rl
)

axl_pop(CMAKE_CURRENT_BINARY_DIR)

set(
	GEN_RL_CPP_LIST
	${GEN_DIR}/axl_dox_Lexer.rl.cpp
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
	${AXL_INC_DIR}/axl_dox/axl_dox_Pch.h
)

source_group(
	pch
	FILES
	${PCH_H_LIST}
)

#...............................................................................
#
# axl_dox library
#

include_directories(
	${AXL_INC_DIR}
	${GEN_DIR}
)

add_library(
	axl_dox
	${PCH_H_LIST}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
	${LIB_RL_LIST}
	${GEN_RL_CPP_LIST}
)

target_precompile_headers(
	axl_dox
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
	TARGETS axl_dox
	DESTINATION ${AXL_INSTALL_LIB_SUBDIR}
)

#...............................................................................
