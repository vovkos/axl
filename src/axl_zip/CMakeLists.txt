# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2015. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................
#
# zip folder
#

set (
	ZIP_H_LIST
	${AXL_INC_DIR}/axl_zip_ZipReader.h
	)

set (
	ZIP_CPP_LIST
	axl_zip_ZipReader.cpp
	)

set (
	ZIP_C_LIST
	miniz.c
	)

exclude_from_build (miniz.c)

source_group (
	zip
	FILES
	${ZIP_H_LIST}
	${ZIP_CPP_LIST}
	${ZIP_C_LIST}
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
# axl_zip library
#

include_directories (
	${AXL_INC_DIR}
	)

add_library (
	axl_zip
	${PCH_H}
	${PCH_CPP}
	${ZIP_H_LIST}
	${ZIP_CPP_LIST}
	${ZIP_C_LIST}
	)

set_pch (
	axl_zip
	${PCH_H}
	${PCH_CPP}
	)

if (CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	add_target_string_property (
		axl_zip
		COMPILE_FLAGS
		"-fPIC"
		)
endif ()

#..............................................................................

