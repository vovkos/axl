# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2014. All rights reserved
# Author: Vladimir Gladkov

project_virtual_folder (axl_ini)

#..............................................................................
#
# ini folder
#

set (
	INI_H_LIST
	${AXL_INC_DIR}/axl_ini_Lexer.h
	${AXL_INC_DIR}/axl_ini_Parser.h
	${AXL_INC_DIR}/axl_ini_GenericParser.h
	)

set (
	INI_CPP_LIST
	axl_ini_Lexer.cpp
	axl_ini_Parser.cpp
	axl_ini_GenericParser.cpp
	)

set (
	INI_RL_LIST
	axl_ini_Lexer.rl
	)

source_virtual_folder (
	ini
	${INI_H_LIST}
	${INI_CPP_LIST}
	${INI_RL_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# gen folder
#

set (INI_GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen)

push_and_set (CMAKE_CURRENT_BINARY_DIR ${INI_GEN_DIR})

add_ragel_step (
	axl_ini_Lexer.rl.cpp
	axl_ini_Lexer.rl
	)

pop (CMAKE_CURRENT_BINARY_DIR)

set (
	GEN_RL_CPP_LIST
	${INI_GEN_DIR}/axl_ini_Lexer.rl.cpp
	)

exclude_from_build (${GEN_RL_CPP_LIST})  # include "*.rl.cpp" manually

source_virtual_folder (
	gen
	${GEN_RL_CPP_LIST}
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
# axl_ini library
#

include_directories (${INI_GEN_DIR})

add_library (
	axl_ini
	${PCH_H}
	${PCH_CPP}
	${INI_H_LIST}
	${INI_CPP_LIST}
	${INI_RL_LIST}
	${GEN_RL_CPP_LIST}
	)

if (AXL_USE_PCH)
	set_pch (
		axl_ini
		${PCH_H}
		${PCH_CPP}
		)
endif ()

#..............................................................................

