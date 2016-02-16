#..............................................................................

if ("${LIBSSH2_INC_DIR}" STREQUAL "")
	set (LIBSSH2_FOUND FALSE)
	message (STATUS "LibSSH2:                    <not-found>")
else ()
	set (LIBSSH2_FOUND TRUE)
	message (STATUS "Path to LibSSH2 includes:   ${LIBSSH2_INC_DIR}")
	message (STATUS "Path to LibSSH2 libraries:  ${LIBSSH2_LIB_DIR}")
endif ()

#..............................................................................
