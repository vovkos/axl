# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2015. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................
#
# lib folder
#

set (
	LIB_H_LIST
	${AXL_INC_DIR}/axl_lua_LuaState.h
	)

set (
	LIB_CPP_LIST
	axl_lua_LuaState.cpp
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
	${AXL_INC_DIR}/axl_lua_Pch.h
	)

source_group (
	pch
	FILES
	${PCH_H_LIST}
	${PCH_CPP}
	)

#..............................................................................
#
# axl_lua library
#

include_directories (
	${LUA_INC_DIR}
	${AXL_INC_DIR}
	)

add_library (
	axl_lua
	${PCH_H_LIST}
	${PCH_CPP}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
	${LIB_RL_LIST}
	)

axl_set_pch (
	axl_lua
	${PCH_H}
	${PCH_CPP}
	)

if (GCC)
	axl_append_target_string_property (
		axl_lua
		COMPILE_FLAGS
		"-fPIC"
		)
endif ()

#..............................................................................

