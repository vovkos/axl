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

if(NOT LIBSSH2_INC_DIR)
	axl_find_inc_dir(LIBSSH2_INC_DIR libssh2.h)
endif()

if(NOT LIBSSH2_LIB_DIR)
	axl_find_lib_dir(LIBSSH2_LIB_DIR ssh2)
endif()

if(EXISTS ${LIBSSH2_INC_DIR}/libssh2.h)
	axl_message("LibSSH2 paths:")
	axl_message("    Includes:"  "${LIBSSH2_INC_DIR}")
	axl_message("    Libraries:" "${LIBSSH2_LIB_DIR}")

	if(LIBSSH2_DLL_DIR)
		axl_message("    DLLs:"  "${LIBSSH2_DLL_DIR}")
	endif()

	set(LIBSSH2_FOUND TRUE)
else()
	set(LIBSSH2_FOUND FALSE)
endif()

#...............................................................................
