# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2014. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................
#
# lua folder
#

set (
	LUA_H_LIST
	${AXL_INC_DIR}/axl_lua_LuaState.h
	${AXL_INC_DIR}/axl_lua_StringTemplate.h
	axl_lua_StringTemplateLexer.h
	)

set (
	LUA_CPP_LIST
	axl_lua_LuaState.cpp
	axl_lua_StringTemplate.cpp
	axl_lua_StringTemplateLexer.cpp
	)

set (
	LUA_RL_LIST
	axl_lua_StringTemplateLexer.rl
	)

source_group (
	lua
	FILES
	${LUA_H_LIST}
	${LUA_CPP_LIST}
	${LUA_RL_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# gen folder
#

set (GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen)
file (MAKE_DIRECTORY ${GEN_DIR})

push_and_set (CMAKE_CURRENT_BINARY_DIR ${GEN_DIR})

add_ragel_step (
	axl_lua_StringTemplateLexer.rl.cpp
	axl_lua_StringTemplateLexer.rl
	)

pop (CMAKE_CURRENT_BINARY_DIR)

set (
	GEN_RL_CPP_LIST
	${GEN_DIR}/axl_lua_StringTemplateLexer.rl.cpp
	)

exclude_from_build (${GEN_RL_CPP_LIST})  # include "*.rl.cpp" manually

source_group (
	gen
	FILES
	${GEN_RL_CPP_LIST}
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
# axl_lua library
#

include_directories (
	${LUA_INC_DIR}
	${AXL_INC_DIR}
	${GEN_DIR}
	)

add_library (
	axl_lua
	${PCH_H}
	${PCH_CPP}
	${LUA_H_LIST}
	${LUA_CPP_LIST}
	${LUA_RL_LIST}
	${GEN_RL_CPP_LIST}
	)

set_pch (
	axl_lua
	${PCH_H}
	${PCH_CPP}
	)

#..............................................................................

