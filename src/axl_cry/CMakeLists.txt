# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2014. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................
#
# cry folder
#

set (
	CRY_H_LIST
	${AXL_INC_DIR}/axl_cry_CryptoError.h
	${AXL_INC_DIR}/axl_cry_BigNum.h
	${AXL_INC_DIR}/axl_cry_EcKey.h
	${AXL_INC_DIR}/axl_cry_EcPoint.h
	${AXL_INC_DIR}/axl_cry_EcProductKey.h
	)

set (
	CRY_CPP_LIST
	axl_cry_CryptoError.cpp
	axl_cry_BigNum.cpp
	axl_cry_EcKey.cpp
	axl_cry_EcPoint.cpp
	axl_cry_EcProductKey.cpp
	)

source_group (
	cry
	FILES
	${CRY_H_LIST}
	${CRY_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set (PCH_H   pch.h)
set (PCH_CPP pch.cpp)

source_group (
	pch
	FILES
	${PCH_H}
	${PCH_CPP}
	)

#..............................................................................
#
# axl_cry library
#

include_directories (${OPENSSL_INC_DIR})

add_library (
	axl_cry
	${PCH_H}
	${PCH_CPP}
	${CRY_H_LIST}
	${CRY_CPP_LIST}
	)

set_pch (
	axl_cry
	${PCH_H}
	${PCH_CPP}
	)

#..............................................................................

