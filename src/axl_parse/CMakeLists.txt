# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2013. All rights reserved
# Author: Vladimir Gladkov

project_virtual_folder (axl_parse)

#..............................................................................
#
# err folder
#

set (
	ERR_H_LIST
	${AXL_INC_DIR}/axl_err_ParseError.h
	)

set (
	ERR_CPP_LIST
	axl_err_ParseError.cpp
	)

source_virtual_folder (
	err
	${ERR_H_LIST}
	${ERR_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# lex folder
#

set (
	LEX_H_LIST
	${AXL_INC_DIR}/axl_lex_Lexer.h
	${AXL_INC_DIR}/axl_lex_RagelLexer.h
	${AXL_INC_DIR}/axl_lex_Token.h
	)

set (LEX_CPP_LIST)

source_virtual_folder (
	lex
	${LEX_H_LIST}
	${LEX_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# llk folder
#

set (
	LLK_H_LIST
	${AXL_INC_DIR}/axl_llk_Ast.h
	${AXL_INC_DIR}/axl_llk_Node.h
	${AXL_INC_DIR}/axl_llk_Parser.h
	)

set (LLK_CPP_LIST)

source_virtual_folder (
	llk
	${LLK_H_LIST}
	${LLK_CPP_LIST}
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
# axl_parse library
#

add_library (
	axl_parse
	${ERR_H_LIST}
	${ERR_CPP_LIST}
	${LEX_H_LIST}
	${LEX_CPP_LIST}
	${LLK_H_LIST}
	${LLK_CPP_LIST}
	${PCH_H}
	${PCH_CPP}
	)

if (AXL_USE_PCH)
	set_pch (
		axl_parse
		${PCH_H}
		${PCH_CPP}
		)
endif ()

#..............................................................................

