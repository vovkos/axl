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
	${AXL_INC_DIR}/axl_io_mdns/axl_io_MdnsRecord.h
	${AXL_INC_DIR}/axl_io_mdns/axl_io_MdnsSocket.h
)

set(
	LIB_CPP_LIST
	axl_io_MdnsRecord.cpp
	axl_io_MdnsSocket.cpp
)

source_group(
	lib
	FILES
	${LIB_H_LIST}
	${LIB_CPP_LIST}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# mdns folder -- the dependency itself, listed for in-IDE editing only (it is
# neither compiled nor installed from here); drop it once the fork settles
#

set(
	MDNS_H_LIST
	${AXL_INC_DIR}/axl_io_mdns/mdns.h
)

source_group(
	mdns
	FILES
	${MDNS_H_LIST}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set(PCH_H   pch.h)

set(
	PCH_H_LIST
	${PCH_H}
	${AXL_INC_DIR}/axl_io_mdns/axl_io_MdnsPch.h
)

source_group(
	pch
	FILES
	${PCH_H_LIST}
	REGULAR_EXPRESSION cmake_pch
)

#...............................................................................
#
# axl_io_mdns library
#

include_directories(${AXL_INC_DIR})

add_definitions(-D_AXL_IO_MDNS=1)

add_library(
	axl_io_mdns
	${PCH_H_LIST}
	${LIB_H_LIST}
	${MDNS_H_LIST}
	${LIB_CPP_LIST}
)

target_precompile_headers(
	axl_io_mdns
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
	TARGETS axl_io_mdns
	DESTINATION ${AXL_INSTALL_LIB_SUBDIR}
)

#...............................................................................
