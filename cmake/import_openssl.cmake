#..............................................................................

if (EXISTS ${OPENSSL_INC_DIR}/openssl/obj_mac.h)
	set (OPENSSL_FOUND TRUE)
else ()
	set (OPENSSL_FOUND FALSE)
endif ()

#..............................................................................
