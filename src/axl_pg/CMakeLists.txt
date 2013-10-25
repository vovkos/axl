# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2013. All rights reserved
# Author: Vladimir Gladkov

project_virtual_folder (axl_pg)

#..............................................................................
#
# pg folder
#

set (
	PG_H_LIST
	axl_pg_ClassMgr.h
	axl_pg_Config.h
	axl_pg_DefineMgr.h
	axl_pg_Generator.h
	axl_pg_LaDfaBuilder.h
	axl_pg_Lexer.h
	axl_pg_Module.h
	axl_pg_Node.h
	axl_pg_NodeMgr.h
	axl_pg_Parser.h
	axl_pg_ParseTableBuilder.h
	axl_pg_ProductionBuilder.h
	)

set (
	PG_CPP_LIST
	axl_pg.cpp
	axl_pg_ClassMgr.cpp
	axl_pg_DefineMgr.cpp
	axl_pg_Generator.cpp
	axl_pg_LaDfaBuilder.cpp
	axl_pg_Lexer.rl.cpp
	axl_pg_Module.cpp
	axl_pg_Node.cpp
	axl_pg_NodeMgr.cpp
	axl_pg_Parser.cpp
	axl_pg_ParseTableBuilder.cpp
	axl_pg_ProductionBuilder.cpp
	)

set (
	PG_RL_LIST
	axl_pg_Lexer.rl
	)

push_and_set (CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_ragel_step (
	axl_pg_Lexer.rl.cpp
	axl_pg_Lexer.rl
	)

pop (CMAKE_CURRENT_BINARY_DIR)

source_virtual_folder (
	pg
	${PG_H_LIST}
	${PG_CPP_LIST}
	${PG_RL_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pg_frames folder
#

set (
	PG_FRAME_LIST
	${AXL_PG_FRAME_DIR}/axl_llk_ParserFrame.h.luaf
	)

source_virtual_folder (
	pg_frames
	${PG_FRAME_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set (PCH_H   pch.h)
set (PCH_CPP pch.cpp)

source_virtual_folder (
	pch
	${PCH_H}
	${PCH_CPP}
	)

#..............................................................................
#
# till parser generator tool
#

include_directories (${PATH_LUA_INC_DIR})

link_directories (${PATH_LUA_LIB_DIR})

add_executable (
	axl_pg
	${PG_H_LIST}
	${PG_CPP_LIST}
	${PG_RL_LIST}
	${PG_FRAME_LIST}
	${PCH_H}
	${PCH_CPP}
	)

target_link_libraries (
	axl_pg
	axl_core
	axl_io
	axl_lua
	axl_parse
	)

if (MSVC)
	target_link_libraries (axl_pg lua)
endif ()

if (UNIX)
	target_link_libraries (
		axl_pg
		lua52
		dl
		pthread
		)
endif ()

if (AXL_USE_PCH)
	set_pch (
		axl_pg
		${PCH_H}
		${PCH_CPP}
		)
endif ()

#..............................................................................
