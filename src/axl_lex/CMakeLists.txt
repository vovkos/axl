# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2015. All rights reserved
# Author: Vladimir Gladkov

#...............................................................................
#
# lib folder
#

set (
	LIB_H_LIST
	${AXL_INC_DIR}/axl_lex_Lexer.h
	${AXL_INC_DIR}/axl_lex_RagelLexer.h
	${AXL_INC_DIR}/axl_lex_Token.h
	${AXL_INC_DIR}/axl_lex_ParseError.h
	)

set (
	LIB_CPP_LIST
	axl_lex_ParseError.cpp
	axl_lex_Token.cpp
	)

source_group (
	lex
	FILES
	${LIB_H_LIST}
	${LIB_CPP_LIST}
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
	${AXL_INC_DIR}/axl_lex_Pch.h
	)

source_group (
	pch
	FILES
	${PCH_H_LIST}
	${PCH_CPP}
	)

#...............................................................................
#
# axl_lex library
#

include_directories (${AXL_INC_DIR})

add_library (
	axl_lex
	${PCH_H_LIST}
	${PCH_CPP}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
	)

axl_set_pch (
	axl_lex
	${PCH_H}
	${PCH_CPP}
	)

#...............................................................................
