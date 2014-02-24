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
	)

set (
	INI_CPP_LIST
	axl_ini_Lexer.rl.cpp
	axl_ini_Parser.cpp
	)

set (
	INI_RL_LIST
	axl_ini_Lexer.rl
	)

push_and_set (CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_ragel_step (
	axl_ini_Lexer.rl.cpp
	axl_ini_Lexer.rl
	)

pop (CMAKE_CURRENT_BINARY_DIR)

source_virtual_folder (
	ini
	${INI_H_LIST}
	${INI_CPP_LIST}
	${INI_RL_LIST}
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

add_library (
	axl_ini
	${PCH_H}
	${PCH_CPP}
	${INI_H_LIST}
	${INI_CPP_LIST}
	${INI_RL_LIST}
	)

if (AXL_USE_PCH)
	set_pch (
		axl_ini
		${PCH_H}
		${PCH_CPP}
		)
endif ()

#..............................................................................

