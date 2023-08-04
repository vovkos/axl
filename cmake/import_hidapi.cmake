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

if(NOT HIDAPI_INC_DIR)
	axl_find_inc_dir(HIDAPI_INC_DIR hidapi/hidapi.h)
endif()

if(NOT HIDAPI_LIB_DIR)
	axl_find_lib_dir(HIDAPI_LIB_DIR hidapi)
endif()

if(EXISTS ${HIDAPI_INC_DIR}/hidapi/hidapi.h AND HIDAPI_LIB_DIR)

	axl_message("HIDAPI paths:")
	axl_message("    Includes:"  "${HIDAPI_INC_DIR}")
	axl_message("    Libraries:" "${HIDAPI_LIB_DIR}")

	if(HIDAPI_DLL_DIR)
		axl_message("    DLLs:" "${HIDAPI_DLL_DIR}")
	endif()

	set(HIDAPI_FOUND TRUE)
else()
	set(HIDAPI_FOUND FALSE)
endif()

#...............................................................................
