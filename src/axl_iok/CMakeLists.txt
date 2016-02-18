# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2015. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................
#
# lib folder
#

set (
	LIB_H_LIST
	${AXL_INC_DIR}/axl_iok_Object.h
	${AXL_INC_DIR}/axl_iok_Iterator.h
	${AXL_INC_DIR}/axl_iok_RegistryEntry.h
	)

set (
	LIB_CPP_LIST
	axl_iok_Iterator.cpp
	axl_iok_RegistryEntry.cpp
	)

source_group (
	lib
	FILES
	${LIB_H_LIST}
	${LIB_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set (PCH_H   pch.h)
set (PCH_CPP pch.cpp)

set (
	PCH_H_LIST
	${PCH_H}
	${AXL_INC_DIR}/axl_iok_Pch.h
	)

source_group (
	pch
	FILES
	${PCH_H_LIST}
	${PCH_CPP}
	)

#..............................................................................
#
# axl_iok library
#

include_directories (${AXL_INC_DIR})

add_library (
	axl_iok
	${PCH_H_LIST}
	${PCH_CPP}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
	)

axl_set_pch (
	axl_iok
	${PCH_H}
	${PCH_CPP}
	)

if (GCC)
	axl_add_target_string_property (
		axl_iok
		COMPILE_FLAGS
		"-fPIC"
		)
endif ()

#..............................................................................
