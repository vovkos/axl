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

#
# cry folder
#

set(
	LIB_H_LIST
	${AXL_INC_DIR}/axl_cry/axl_cry_CryptoError.h
	${AXL_INC_DIR}/axl_cry/axl_cry_AppStoreReceipt.h
	${AXL_INC_DIR}/axl_cry/axl_cry_Asn1.h
	${AXL_INC_DIR}/axl_cry/axl_cry_Bio.h
	${AXL_INC_DIR}/axl_cry/axl_cry_BigNum.h
	${AXL_INC_DIR}/axl_cry/axl_cry_EcKey.h
	${AXL_INC_DIR}/axl_cry/axl_cry_EcPoint.h
	${AXL_INC_DIR}/axl_cry/axl_cry_Rsa.h
	${AXL_INC_DIR}/axl_cry/axl_cry_Dh.h
	${AXL_INC_DIR}/axl_cry/axl_cry_Aes.h
	${AXL_INC_DIR}/axl_cry/axl_cry_X509.h
	${AXL_INC_DIR}/axl_cry/axl_cry_Pkcs7.h
	${AXL_INC_DIR}/axl_cry/axl_cry_Pem.h
)

set(
	LIB_CPP_LIST
	axl_cry_CryptoError.cpp
	axl_cry_AppStoreReceipt.cpp
	axl_cry_Asn1.cpp
	axl_cry_Bio.cpp
	axl_cry_BigNum.cpp
	axl_cry_EcKey.cpp
	axl_cry_EcPoint.cpp
	axl_cry_Rsa.cpp
	axl_cry_Dh.cpp
	axl_cry_X509.cpp
	axl_cry_Pkcs7.cpp
	axl_cry_Pem.cpp
)

source_group(
	lib
	FILES
	${LIB_H_LIST}
	${LIB_CPP_LIST}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set(PCH_H   pch.h)

set(
	PCH_H_LIST
	${PCH_H}
	${AXL_INC_DIR}/axl_cry/axl_cry_Pch.h
)

source_group(
	pch
	FILES
	${PCH_H_LIST}
)

#...............................................................................
#
# axl_cry library
#

include_directories(
	${OPENSSL_INC_DIR}
	${AXL_INC_DIR}
)

add_library(
	axl_cry
	${PCH_H_LIST}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
)

target_precompile_headers(
	axl_cry
	PRIVATE
	${PCH_H}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

axl_install_redirected_includes(
	${AXL_INSTALL_INC_SUBDIR}
	axl_cry
	${LIB_H_LIST}
)

install(
	TARGETS axl_cry
	DESTINATION ${AXL_INSTALL_LIB_SUBDIR}
)

#...............................................................................
