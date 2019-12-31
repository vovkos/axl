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
	${AXL_INC_DIR}/axl_io_pcap/axl_io_Pcap.h
	${AXL_INC_DIR}/axl_io_pcap/axl_io_PcapFilter.h
	)

set(
	LIB_CPP_LIST
	axl_io_Pcap.cpp
	axl_io_PcapFilter.cpp
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
	${AXL_INC_DIR}/axl_io_pcap/axl_io_PcapPch.h
	)

source_group(
	pch
	FILES
	${PCH_H_LIST}
	${PCH_CPP}
	)

#...............................................................................
#
# axl_io_pcap library
#

include_directories(
	${PCAP_INC_DIR}
	${AXL_INC_DIR}
	)

add_definitions(-D_AXL_IO_PCAP=1)

add_library(
	axl_io_pcap
	${PCH_H_LIST}
	${PCH_CPP}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
	)

axl_set_pch(
	axl_io_pcap
	${PCH_H}
	${PCH_CPP}
	)

axl_install_redirected_includes(
	${AXL_INSTALL_INC_SUBDIR}
	axl_io_pcap
	${LIB_H_LIST}
	)

install(
	TARGETS axl_io_pcap
	DESTINATION ${AXL_INSTALL_LIB_SUBDIR}
	)

#...............................................................................
