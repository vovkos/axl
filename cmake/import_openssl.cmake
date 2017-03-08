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

if (NOT OPENSSL_INC_DIR)
	axl_find_inc_dir (OPENSSL_INC_DIR openssl/obj_mac.h)
endif ()

if (NOT OPENSSL_LIB_DIR)
	axl_find_lib_dir (OPENSSL_LIB_DIR ssl)
endif ()

if (NOT OPENSSL_CRYPTO_LIB_NAME)
	if (WIN32)
		set (OPENSSL_CRYPTO_LIB_NAME libeay32)
	else ()
		set (OPENSSL_CRYPTO_LIB_NAME crypto)
	endif ()
endif ()

if (NOT OPENSSL_SSL_LIB_NAME)
	if (WIN32)
		set (OPENSSL_SSL_LIB_NAME ssleay32)
	else ()
		set (OPENSSL_SSL_LIB_NAME ssl)
	endif ()
endif ()

if (EXISTS ${OPENSSL_INC_DIR}/openssl/obj_mac.h)
	axl_message ("OpenSSL paths:")
	axl_message ("    Includes:"       "${OPENSSL_INC_DIR}")
	axl_message ("    Library dir:"    "${OPENSSL_LIB_DIR}")
	axl_message ("    Crypto library:" "${OPENSSL_CRYPTO_LIB_NAME}")
	axl_message ("    SSL library:"    "${OPENSSL_SSL_LIB_NAME}")

	set (OPENSSL_FOUND TRUE)
else ()
	set (OPENSSL_FOUND FALSE)
endif ()

#...............................................................................
