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

set (
	LIB_H_LIST
	${AXL_INC_DIR}/axl_cry/axl_cry_CryptoError.h
	${AXL_INC_DIR}/axl_cry/axl_cry_BigNum.h
	${AXL_INC_DIR}/axl_cry/axl_cry_EcKey.h
	${AXL_INC_DIR}/axl_cry/axl_cry_EcPoint.h
	)

set (
	LIB_CPP_LIST
	axl_cry_CryptoError.cpp
	axl_cry_BigNum.cpp
	axl_cry_EcKey.cpp
	axl_cry_EcPoint.cpp
	)

source_group (
	lib
	FILES
	${LIB_H_LIST}
	${LIB_CPP_LIST}
	)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set (PCH_H   pch.h)
set (PCH_CPP pch.cpp)

set (
	PCH_H_LIST
	${PCH_H}
	${AXL_INC_DIR}/axl_cry/axl_cry_Pch.h
	)

source_group (
	pch
	FILES
	${PCH_H_LIST}
	${PCH_CPP}
	)

#...............................................................................
#
# axl_cry library
#

include_directories (
	${OPENSSL_INC_DIR}
	${AXL_INC_DIR}
	)

add_library (
	axl_cry
	${PCH_H_LIST}
	${PCH_CPP}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
	)

axl_set_pch (
	axl_cry
	${PCH_H}
	${PCH_CPP}
	)

#...............................................................................
