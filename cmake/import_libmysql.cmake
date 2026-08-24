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

set(
	_LIBMYSQL_LIB_NAME_LIST
	mariadb
	libmariadb
	mariadbclient
	libmysql
	mysqlclient
)

if(NOT LIBMYSQL_INC_DIR)
	axl_find_inc_dir(
		LIBMYSQL_INC_DIR mysql.h
		PATH_SUFFIXES . mariadb mysql
	)
endif()

if(NOT LIBMYSQL_LIB_DIR)
	if(NOT LIBMYSQL_LIB_NAME)
		axl_find_lib_dir_ex(
			RESULT_LIB_DIR LIBMYSQL_LIB_DIR
			RESULT_LIB_NAME LIBMYSQL_LIB_NAME
			LIB_NAME ${_LIBMYSQL_LIB_NAME_LIST}
		)
	else()
		axl_find_lib_dir_ex(
			RESULT_LIB_DIR LIBMYSQL_LIB_DIR
			LIB_NAME ${LIBMYSQL_LIB_NAME}
		)
	endif()
elseif(NOT LIBMYSQL_LIB_NAME)
	axl_find_lib_dir_ex(
		RESULT_LIB_NAME LIBMYSQL_LIB_NAME
		LIB_DIR ${LIBMYSQL_LIB_DIR}
		LIB_NAME ${_LIBMYSQL_LIB_NAME_LIST}
	)
endif()

if(EXISTS ${LIBMYSQL_INC_DIR}/mysql.h AND LIBMYSQL_LIB_DIR AND LIBMYSQL_LIB_NAME)
	axl_message("MySQL Connector paths:")
	axl_message("    Includes:"  "${LIBMYSQL_INC_DIR}")
	axl_message("    Libraries:" "${LIBMYSQL_LIB_DIR}")

	if(LIBMYSQL_DLL_DIR)
		axl_message("    DLLs:"  "${LIBMYSQL_DLL_DIR}")
	endif()

	axl_message("    Library name:" "${LIBMYSQL_LIB_NAME}")

	set(LIBMYSQL_FOUND TRUE)
else()
	set(LIBMYSQL_FOUND FALSE)
endif()

#...............................................................................
