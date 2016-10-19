# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2015. All rights reserved
# Author: Vladimir Gladkov

#...............................................................................
#
# lib folder
#

set (
	LIB_H_LIST
	${AXL_INC_DIR}/axl_cf_Type.h
	${AXL_INC_DIR}/axl_cf_Number.h
	${AXL_INC_DIR}/axl_cf_String.h
	${AXL_INC_DIR}/axl_cf_Dictionary.h
	)

set (
	LIB_CPP_LIST
	axl_cf_Type.cpp
	axl_cf_Number.cpp
	axl_cf_String.cpp
	axl_cf_Dictionary.cpp
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
	${AXL_INC_DIR}/axl_cf_Pch.h
	)

source_group (
	pch
	FILES
	${PCH_H_LIST}
	${PCH_CPP}
	)

#...............................................................................
#
# axl_cf library
#

include_directories (${AXL_INC_DIR})

add_library (
	axl_cf
	${PCH_H_LIST}
	${PCH_CPP}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
	)

axl_set_pch (
	axl_cf
	${PCH_H}
	${PCH_CPP}
	)

#...............................................................................
