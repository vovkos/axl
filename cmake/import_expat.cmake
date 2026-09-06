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
	_EXPAT_LIB_NAME_LIST
	expat
	libexpat
)

if(NOT EXPAT_INC_DIR)
	axl_find_inc_dir(EXPAT_INC_DIR expat.h)
endif()

if(NOT EXPAT_LIB_DIR)
	if(NOT EXPAT_LIB_NAME)
		axl_find_lib_dir_ex(
			RESULT_LIB_DIR EXPAT_LIB_DIR
			RESULT_LIB_NAME EXPAT_LIB_NAME
			LIB_NAME ${_EXPAT_LIB_NAME_LIST}
		)
	else()
		axl_find_lib_dir_ex(
			RESULT_LIB_DIR EXPAT_LIB_DIR
			LIB_NAME ${EXPAT_LIB_NAME}
		)
	endif()
elseif(NOT EXPAT_LIB_NAME)
	axl_find_lib_dir_ex(
		RESULT_LIB_NAME EXPAT_LIB_NAME
		LIB_DIR ${EXPAT_LIB_DIR}
		LIB_NAME ${_EXPAT_LIB_NAME_LIST}
	)
endif()

if(EXISTS ${EXPAT_INC_DIR}/expat.h AND EXPAT_LIB_DIR AND EXPAT_LIB_NAME)
	axl_message("Expat paths:")
	axl_message("    Includes:"  "${EXPAT_INC_DIR}")
	axl_message("    Libraries:" "${EXPAT_LIB_DIR}")

	if(EXPAT_DLL_DIR)
		axl_message("    DLLs:"  "${EXPAT_DLL_DIR}")
	endif()

	axl_message("    Library name:" "${EXPAT_LIB_NAME}")

	set(EXPAT_FOUND TRUE)
else()
	set(EXPAT_FOUND FALSE)
endif()

#...............................................................................
