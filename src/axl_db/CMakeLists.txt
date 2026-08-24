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
	${AXL_INC_DIR}/axl_db/axl_db_MySqlError.h
	${AXL_INC_DIR}/axl_db/axl_db_MySqlRes.h
	${AXL_INC_DIR}/axl_db/axl_db_MySqlStmt.h
	${AXL_INC_DIR}/axl_db/axl_db_MySql.h
)

set(
	LIB_CPP_LIST
	axl_db_MySql.cpp
)

source_group(
	lib
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
	${AXL_INC_DIR}/axl_db/axl_db_MySqlPch.h
)

source_group(
	pch
	FILES
	${PCH_H_LIST}
	REGULAR_EXPRESSION cmake_pch
)

#...............................................................................
#
# axl_db library
#

include_directories(
	${LIBMYSQL_INC_DIR}
	${AXL_INC_DIR}
)

add_library(
	axl_db
	${PCH_H_LIST}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
)

target_precompile_headers(
	axl_db
	PRIVATE
	${PCH_H}
)

axl_install_redirected_includes(
	${AXL_INSTALL_INC_SUBDIR}
	${PCH_H_LIST}
	${LIB_H_LIST}
)

install(
	TARGETS axl_db
	DESTINATION ${AXL_INSTALL_LIB_SUBDIR}
)

#...............................................................................
