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
	_YAJL_LIB_NAME_LIST
	yajl_s
	yajl
)

if(NOT YAJL_INC_DIR)
	axl_find_inc_dir(YAJL_INC_DIR yajl/yajl_parse.h)
endif()

if(NOT YAJL_LIB_DIR)
	if(NOT YAJL_LIB_NAME)
		axl_find_lib_dir_ex(
			RESULT_LIB_DIR YAJL_LIB_DIR
			RESULT_LIB_NAME YAJL_LIB_NAME
			LIB_NAME ${_YAJL_LIB_NAME_LIST}
		)
	else()
		axl_find_lib_dir_ex(
			RESULT_LIB_DIR YAJL_LIB_DIR
			LIB_NAME ${YAJL_LIB_NAME}
		)
	endif()
elseif(NOT YAJL_LIB_NAME)
	axl_find_lib_dir_ex(
		RESULT_LIB_NAME YAJL_LIB_NAME
		LIB_DIR ${YAJL_LIB_DIR}
		LIB_NAME ${_YAJL_LIB_NAME_LIST}
	)
endif()

if(EXISTS ${YAJL_INC_DIR}/yajl/yajl_parse.h AND YAJL_LIB_DIR AND YAJL_LIB_NAME)
	axl_message("YAJL paths:")
	axl_message("    Includes:"  "${YAJL_INC_DIR}")
	axl_message("    Libraries:" "${YAJL_LIB_DIR}")

	if(YAJL_DLL_DIR)
		axl_message("    DLLs:"  "${YAJL_DLL_DIR}")
	endif()

	axl_message("    Library name:" "${YAJL_LIB_NAME}")

	set(YAJL_FOUND TRUE)
else()
	set(YAJL_FOUND FALSE)
endif()

#...............................................................................
