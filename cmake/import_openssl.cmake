#..............................................................................

if ("${OPENSSL_INC_DIR}" STREQUAL "")
	set (OPENSSL_FOUND FALSE)
	message (STATUS "OpenSSL:                    <not-found>")
else ()
	set (OPENSSL_FOUND TRUE)
	message (STATUS "Path to OpenSSL includes:   ${OPENSSL_INC_DIR}")
	message (STATUS "Path to OpenSSL libraries:  ${OPENSSL_LIB_DIR}")
endif ()

#..............................................................................
