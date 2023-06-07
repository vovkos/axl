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
# ini folder
#

set(
	LIB_H_LIST
	${AXL_INC_DIR}/axl_xml/axl_xml_ExpatError.h
	${AXL_INC_DIR}/axl_xml/axl_xml_ExpatParser.h
)

set(
	LIB_CPP_LIST
	axl_xml_ExpatError.cpp
	axl_xml_ExpatParser.cpp
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
	${AXL_INC_DIR}/axl_xml/axl_xml_Pch.h
)

source_group(
	pch
	FILES
	${PCH_H_LIST}
	REGULAR_EXPRESSION cmake_pch
)

#...............................................................................
#
# axl_xml library
#

include_directories(
	${AXL_INC_DIR}
	${EXPAT_INC_DIR}
)

add_library(
	axl_xml
	${PCH_H_LIST}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
)

target_precompile_headers(
	axl_xml
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
	TARGETS axl_xml
	DESTINATION ${AXL_INSTALL_LIB_SUBDIR}
)

#...............................................................................
