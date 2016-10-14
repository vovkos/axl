#..............................................................................

if (NOT OPENSSL_INC_DIR)
	axl_find_inc_dir (OPENSSL_INC_DIR openssl/obj_mac.h)
endif ()

if (NOT OPENSSL_LIB_DIR)
	axl_find_lib_dir (OPENSSL_LIB_DIR ssl)
endif ()

if (EXISTS ${OPENSSL_INC_DIR}/openssl/obj_mac.h)
	message (STATUS "OpenSSL paths:")
	axl_message ("    Includes:"  "${OPENSSL_INC_DIR}")
	axl_message ("    Libraries:" "${OPENSSL_LIB_DIR}")

	set (OPENSSL_FOUND TRUE)
else ()
	set (OPENSSL_FOUND FALSE)
endif ()

#..............................................................................
