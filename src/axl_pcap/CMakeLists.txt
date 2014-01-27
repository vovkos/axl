# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2013. All rights reserved
# Author: Vladimir Gladkov

project_virtual_folder (axl_pcap)

#..............................................................................
#
# io folder
#

set (
	IO_H_LIST
	${AXL_INC_DIR}/axl_io_PCap.h
	)

set (
	IO_CPP_LIST
	axl_io_PCap.cpp
	)

source_virtual_folder (
	io
	${IO_H_LIST}
	${IO_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set (PCH_H   pch.h)
set (PCH_CPP pch.cpp)

source_virtual_folder (
	pch
	${PCH_H}
	${PCH_CPP}
	)

#..............................................................................
#
# axl_parse library
#

include_directories (${PATH_PCAP_INC_DIR})

add_library (
	axl_pcap
	${PCH_H}
	${PCH_CPP}
	${IO_H_LIST}
	${IO_CPP_LIST}
	)

if (AXL_USE_PCH)
	set_pch (
		axl_pcap
		${PCH_H}
		${PCH_CPP}
		)
endif ()

#..............................................................................

