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
	${AXL_INC_DIR}/axl_re2/axl_re2_Regex.h
	${AXL_INC_DIR}/axl_re2/axl_re2_State.h
	axl_re2_Storage.h
)

set(
	LIB_CPP_LIST
	axl_re2_Regex.cpp
	axl_re2_State.cpp
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
	${AXL_INC_DIR}/axl_re2/axl_re2_Pch.h
)

source_group(
	pch
	FILES
	${PCH_H_LIST}
	REGULAR_EXPRESSION cmake_pch
)

#...............................................................................
#
# axl_re2s library (wrappers for re2)
#

include_directories(
	${AXL_INC_DIR}
	${RE2S_INC_DIR}
)

add_library(
	axl_re2
	${PCH_H_LIST}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
)

target_precompile_headers(
	axl_re2
	PRIVATE
	${PCH_H}
)

if(TARGET re2::re2)
	target_link_libraries(axl_re2 re2::re2)
endif()

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

axl_install_redirected_includes(
	${AXL_INSTALL_INC_SUBDIR}
	${PCH_H_LIST}
	${LIB_H_LIST}
)

install(
	TARGETS axl_re2
	DESTINATION ${AXL_INSTALL_LIB_SUBDIR}
)

#...............................................................................
