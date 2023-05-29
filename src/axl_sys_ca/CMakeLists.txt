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

if (LINUX)
	list(APPEND LIB_H_LIST axl_sys_lnx_ElfParser.h)
	list(APPEND LIB_CPP_LIST axl_sys_lnx_ElfParser.cpp)
endif()

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set(PCH_H   pch.h)

set(
	PCH_H_LIST
	${PCH_H}
	${AXL_INC_DIR}/axl_sys_ca/axl_sys_CaPch.h
)

source_group(
	pch
	FILES
	${PCH_H_LIST}
)

#...............................................................................
#
# axl_sys_ca library
#

include_directories(${AXL_INC_DIR})

if(LINUX)
	include_directories(${OPENSSL_INC_DIR})
endif()

add_definitions(-D_AXL_SYS_CA=1)

add_library(
	axl_sys_ca
	${PCH_H_LIST}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
)

target_precompile_headers(
	axl_sys_ca
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
	TARGETS axl_sys_ca
	DESTINATION ${AXL_INSTALL_LIB_SUBDIR}
)

#...............................................................................
