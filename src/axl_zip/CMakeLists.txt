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
# zip folder
#

set(
	LIB_H_LIST
	${AXL_INC_DIR}/axl_zip/axl_zip_ZipReader.h
)

set(
	LIB_CPP_LIST
	axl_zip_ZipReader.cpp
)

set(
	LIB_C_LIST
	miniz.c
)

axl_exclude_from_build(${LIB_C_LIST})

source_group(
	lib
	FILES
	${LIB_H_LIST}
	${LIB_CPP_LIST}
	${LIB_C_LIST}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set(PCH_H   pch.h)

set(
	PCH_H_LIST
	${PCH_H}
	${AXL_INC_DIR}/axl_zip/axl_zip_Pch.h
)

source_group(
	pch
	FILES
	${PCH_H_LIST}
	REGULAR_EXPRESSION cmake_pch
)

#...............................................................................
#
# axl_zip library
#

include_directories(
	${AXL_INC_DIR}
)

add_library(
	axl_zip
	${PCH_H_LIST}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
	${LIB_C_LIST}
)

target_precompile_headers(
	axl_zip
	PRIVATE
	${PCH_H}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

axl_install_redirected_includes(
	${AXL_INSTALL_INC_SUBDIR}
	${PCH_H_LIST}
	${LIB_H_LIST}
)

install(
	TARGETS axl_zip
	DESTINATION ${AXL_INSTALL_LIB_SUBDIR}
)

#...............................................................................
