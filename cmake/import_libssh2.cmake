#..............................................................................

if (NOT LIBSSH2_INC_DIR)
	axl_find_inc_dir (LIBSSH2_INC_DIR libssh2.h)
endif ()

if (NOT LIBSSH2_LIB_DIR)
	axl_find_lib_dir (LIBSSH2_LIB_DIR ssh2)
endif ()

if (EXISTS ${LIBSSH2_INC_DIR}/libssh2.h)
	message (STATUS "LibSSH2 paths:")
	message (STATUS "    LibSSH2 includes:  ${LIBSSH2_INC_DIR}")
	message (STATUS "    LibSSH2 libraries: ${LIBSSH2_LIB_DIR}")

	set (LIBSSH2_FOUND TRUE)
else ()
	set (LIBSSH2_FOUND FALSE)
endif ()

#..............................................................................
