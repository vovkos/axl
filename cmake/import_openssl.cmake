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

set (
	_OPENSSL_CRYPTO_LIB_NAME_LIST
	crypto
	libcrypto
	libeay32
	)

set (
	_OPENSSL_SSL_LIB_NAME_LIST
	ssl
	libssl
	ssleay32
	)

if (NOT OPENSSL_INC_DIR)
	axl_find_inc_dir (OPENSSL_INC_DIR openssl/obj_mac.h)
endif ()

if (NOT OPENSSL_LIB_DIR)
	if (NOT OPENSSL_CRYPTO_LIB_NAME)
		axl_find_lib_dir_ex (
			RESULT_LIB_DIR OPENSSL_LIB_DIR
			RESULT_LIB_NAME OPENSSL_CRYPTO_LIB_NAME
			LIB_NAME ${_OPENSSL_CRYPTO_LIB_NAME_LIST}
			)
	else ()
		axl_find_lib_dir_ex (
			RESULT_LIB_DIR OPENSSL_LIB_DIR
			LIB_NAME ${OPENSSL_CRYPTO_LIB_NAME}
			)
	endif ()
elseif (NOT OPENSSL_CRYPTO_LIB_NAME)
	axl_find_lib_dir_ex (
		RESULT_LIB_NAME OPENSSL_CRYPTO_LIB_NAME
		LIB_DIR ${OPENSSL_LIB_DIR}
		LIB_NAME ${_OPENSSL_CRYPTO_LIB_NAME_LIST}
		)
endif ()

if (NOT OPENSSL_SSL_LIB_NAME)
	axl_find_lib_dir_ex (
		RESULT_LIB_NAME OPENSSL_SSL_LIB_NAME
		LIB_DIR ${OPENSSL_LIB_DIR}
		LIB_NAME ${_OPENSSL_SSL_LIB_NAME_LIST}
		)
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
