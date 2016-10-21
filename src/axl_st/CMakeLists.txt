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

set (
	LIB_H_LIST
	${AXL_INC_DIR}/axl_st_StringTemplate.h
	${AXL_INC_DIR}/axl_st_StringTemplateLexer.h
	${AXL_INC_DIR}/axl_st_LuaStringTemplate.h
	)

set (
	LIB_CPP_LIST
	axl_st_StringTemplate.cpp
	axl_st_StringTemplateLexer.cpp
	axl_st_LuaStringTemplate.cpp
	)

set (
	LIB_RL_LIST
	axl_st_StringTemplateLexer.rl
	)

source_group (
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

set (GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen)
file (MAKE_DIRECTORY ${GEN_DIR})

axl_push_and_set (CMAKE_CURRENT_BINARY_DIR ${GEN_DIR})

add_ragel_step (
	axl_st_StringTemplateLexer.rl.cpp
	axl_st_StringTemplateLexer.rl
	)

axl_pop (CMAKE_CURRENT_BINARY_DIR)

set (
	GEN_RL_CPP_LIST
	${GEN_DIR}/axl_st_StringTemplateLexer.rl.cpp
	)

axl_exclude_from_build (${GEN_RL_CPP_LIST})  # include "*.rl.cpp" manually

source_group (
	gen
	FILES
	${GEN_RL_CPP_LIST}
	)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set (PCH_H   pch.h)
set (PCH_CPP pch.cpp)

set (
	PCH_H_LIST
	${PCH_H}
	${AXL_INC_DIR}/axl_st_Pch.h
	)

source_group (
	pch
	FILES
	${PCH_H_LIST}
	${PCH_CPP}
	)

#...............................................................................
#
# axl_st library
#

include_directories (
	${LUA_INC_DIR}
	${AXL_INC_DIR}
	${GEN_DIR}
	)

add_library (
	axl_st
	${PCH_H_LIST}
	${PCH_CPP}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
	${LIB_RL_LIST}
	${GEN_RL_CPP_LIST}
	)

axl_set_pch (
	axl_st
	${PCH_H}
	${PCH_CPP}
	)

#...............................................................................
