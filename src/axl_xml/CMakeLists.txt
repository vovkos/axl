# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2015. All rights reserved
# Author: Vladimir Gladkov

#...............................................................................
#
# ini folder
#

set (
	LIB_H_LIST
	${AXL_INC_DIR}/axl_xml_ExpatError.h
	${AXL_INC_DIR}/axl_xml_ExpatParser.h
	)

set (
	LIB_CPP_LIST
	axl_xml_ExpatError.cpp
	axl_xml_ExpatParser.cpp
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
	${AXL_INC_DIR}/axl_xml_Pch.h
	)

source_group (
	pch
	FILES
	${PCH_H_LIST}
	${PCH_CPP}
	)

#...............................................................................
#
# axl_xml library
#

include_directories (
	${AXL_INC_DIR}
	${EXPAT_INC_DIR}
	)

add_library (
	axl_xml
	${PCH_H_LIST}
	${PCH_CPP}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
	)

axl_set_pch (
	axl_xml
	${PCH_H}
	${PCH_CPP}
	)

#...............................................................................

