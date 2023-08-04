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
	_HIDAPI_LIB_NAME_LIST
	hidapi
	hidapi-hidraw
	hidapi-libusb
)

if(NOT HIDAPI_INC_DIR)
	axl_find_inc_dir(HIDAPI_INC_DIR hidapi/hidapi.h)
endif()

if(NOT HIDAPI_LIB_DIR)
	if(NOT HIDAPI_LIB_NAME)
		axl_find_lib_dir_ex(
			RESULT_LIB_DIR HIDAPI_LIB_DIR
			RESULT_LIB_NAME HIDAPI_LIB_NAME
			LIB_NAME ${_HIDAPI_LIB_NAME_LIST}
		)
	else()
		axl_find_lib_dir_ex(
			RESULT_LIB_DIR HIDAPI_LIB_DIR
			LIB_NAME ${HIDAPI_LIB_NAME}
		)
	endif()
elseif(NOT HIDAPI_LIB_NAME)
	axl_find_lib_dir_ex(
		RESULT_LIB_NAME HIDAPI_LIB_NAME
		LIB_DIR ${HIDAPI_LIB_DIR}
		LIB_NAME ${_HIDAPI_LIB_NAME_LIST}
	)
endif()

if(EXISTS ${HIDAPI_INC_DIR}/hidapi/hidapi.h AND HIDAPI_LIB_DIR)

	axl_message("HIDAPI paths:")
	axl_message("    Includes:"  "${HIDAPI_INC_DIR}")
	axl_message("    Libraries:" "${HIDAPI_LIB_DIR}")

	if(HIDAPI_DLL_DIR)
		axl_message("    DLLs:" "${HIDAPI_DLL_DIR}")
	endif()

	axl_message("    Library name:" "${HIDAPI_LIB_NAME}")

	set(HIDAPI_FOUND TRUE)
else()
	set(HIDAPI_FOUND FALSE)
endif()

#...............................................................................
