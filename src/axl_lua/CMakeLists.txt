# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2013. All rights reserved
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
	axl_lua_StringTemplateLexer.rl.cpp
	axl_lua_StringTemplate.cpp
	)

set (
	LUA_RL_LIST
	axl_lua_StringTemplateLexer.rl
	)

push_and_set (CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_ragel_step (
	axl_lua_StringTemplateLexer.rl.cpp
	axl_lua_StringTemplateLexer.rl
	) 

pop (CMAKE_CURRENT_BINARY_DIR)

source_group (
	lua
	FILES 
	${LUA_H_LIST}
	${LUA_CPP_LIST}
	${LUA_RL_LIST}
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

include_directories (${PATH_LUA_INC_DIR})

add_library (
	axl_lua
	${LUA_H_LIST}
	${LUA_CPP_LIST}
	${LUA_RL_LIST}
	${PCH_H}
	${PCH_CPP}
	)

if (AXL_USE_PCH)
	set_pch (
		axl_lua
		${PCH_H} 
		${PCH_CPP}
		)
endif ()

#..............................................................................
