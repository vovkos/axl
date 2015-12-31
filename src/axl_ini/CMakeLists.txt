# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2015. All rights reserved
# Author: Vladimir Gladkov

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
	axl_ini_GenericParser.cpp
	)

set (
	INI_RL_LIST
	axl_ini_Lexer.rl
	)

source_group (
	lib
	FILES
	${INI_H_LIST}
	${INI_CPP_LIST}
	${INI_RL_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# gen folder
#

set (GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen)
file (MAKE_DIRECTORY ${GEN_DIR})

push_and_set (CMAKE_CURRENT_BINARY_DIR ${GEN_DIR})

add_ragel_step (
	axl_ini_Lexer.rl.cpp
	axl_ini_Lexer.rl
	)

pop (CMAKE_CURRENT_BINARY_DIR)

set (
	GEN_RL_CPP_LIST
	${GEN_DIR}/axl_ini_Lexer.rl.cpp
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
# axl_ini library
#

include_directories (
	${AXL_INC_DIR}
	${GEN_DIR}
	)

add_library (
	axl_ini
	${PCH_H}
	${PCH_CPP}
	${INI_H_LIST}
	${INI_CPP_LIST}
	${INI_RL_LIST}
	${GEN_RL_CPP_LIST}
	)

set_pch (
	axl_ini
	${PCH_H}
	${PCH_CPP}
	)

if (CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	add_target_string_property (
		axl_ini
		COMPILE_FLAGS
		"-fPIC"
		)
endif ()

#..............................................................................

