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

set(
	LIB_H_LIST
	${AXL_INC_DIR}/axl_lex/axl_lex_SrcPos.h
	${AXL_INC_DIR}/axl_lex/axl_lex_Token.h
	${AXL_INC_DIR}/axl_lex/axl_lex_Lexer.h
	${AXL_INC_DIR}/axl_lex/axl_lex_Ragel.h
	${AXL_INC_DIR}/axl_lex/axl_lex_RagelLexer.h
	${AXL_INC_DIR}/axl_lex/axl_lex_ParseError.h
	${AXL_INC_DIR}/axl_lex/axl_lex_QtRagelSyntaxHighlighter.h
)

set(
	LIB_CPP_LIST
	axl_lex_SrcPos.cpp
	axl_lex_Ragel.cpp
	axl_lex_ParseError.cpp
)

source_group(
	lex
	FILES
	${LIB_H_LIST}
	${LIB_CPP_LIST}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set(PCH_H   pch.h)

set(
	PCH_H_LIST
	${PCH_H}
	${AXL_INC_DIR}/axl_lex/axl_lex_Pch.h
)

source_group(
	pch
	FILES
	${PCH_H_LIST}
)

#...............................................................................
#
# axl_lex library
#

include_directories(${AXL_INC_DIR})

add_library(
	axl_lex
	${PCH_H_LIST}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
)

target_precompile_headers(
	axl_lex
	PRIVATE
	${PCH_H}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

axl_install_redirected_includes(
	${AXL_INSTALL_INC_SUBDIR}
	${PCH_H_LIST}
	${LIB_H_LIST}
)

install(
	TARGETS axl_lex
	DESTINATION ${AXL_INSTALL_LIB_SUBDIR}
)

#...............................................................................
