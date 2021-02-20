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
	${AXL_INC_DIR}/axl_sys_ca/axl_sys_CodeAuthenticator.h
	)

set(
	LIB_CPP_LIST
	axl_sys_CodeAuthenticator.cpp
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
set(PCH_CPP pch.cpp)

set(
	PCH_H_LIST
	${PCH_H}
	${AXL_INC_DIR}/axl_sys_ca/axl_sys_CaPch.h
	)

source_group(
	pch
	FILES
	${PCH_H_LIST}
	${PCH_CPP}
	)

#...............................................................................
#
# axl_sys_ca library
#

include_directories(${AXL_INC_DIR})

add_definitions(-D_AXL_SYS_CA=1)

add_library(
	axl_sys_ca
	${PCH_H_LIST}
	${PCH_CPP}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
	)

axl_set_pch(
	axl_sys_ca
	${PCH_H}
	${PCH_CPP}
	)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

axl_install_redirected_includes(
	${INSTALL_INCLUDE_DIR}
	axl_sys_ca
	${LIB_H_LIST}
	)

install(
	TARGETS axl_sys_ca
	DESTINATION ${AXL_INSTALL_LIB_SUBDIR}
	)

#...............................................................................
