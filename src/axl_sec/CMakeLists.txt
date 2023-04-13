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
# lib folder
#

set(
	LIB_H_LIST
	${AXL_INC_DIR}/axl_sec/axl_sec_Error.h
	${AXL_INC_DIR}/axl_sec/axl_sec_Code.h
	${AXL_INC_DIR}/axl_sec/axl_sec_StaticCode.h
)

set(
	LIB_CPP_LIST
	axl_sec_Error.cpp
	axl_sec_Code.cpp
	axl_sec_StaticCode.cpp
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
	${AXL_INC_DIR}/axl_sec/axl_sec_Pch.h
)

source_group(
	pch
	FILES
	${PCH_H_LIST}
)

#...............................................................................
#
# axl_sec library
#

include_directories(${AXL_INC_DIR})

add_library(
	axl_sec
	${PCH_H_LIST}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
)

target_precompile_headers(
	axl_sec
	PRIVATE
	${PCH_H}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

axl_install_redirected_includes(
	${INSTALL_INCLUDE_DIR}
	axl_sec
	${LIB_H_LIST}
)

install(
	TARGETS axl_sec
	DESTINATION ${AXL_INSTALL_LIB_SUBDIR}
)

#...............................................................................
