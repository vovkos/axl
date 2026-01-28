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
	${AXL_INC_DIR}/axl_py/axl_py_Error.h
	${AXL_INC_DIR}/axl_py/axl_py_Config.h
	${AXL_INC_DIR}/axl_py/axl_py_Init.h
	${AXL_INC_DIR}/axl_py/axl_py_Type.h
	${AXL_INC_DIR}/axl_py/axl_py_Object.h
	${AXL_INC_DIR}/axl_py/axl_py_Exception.h
	${AXL_INC_DIR}/axl_py/axl_py_Long.h
	${AXL_INC_DIR}/axl_py/axl_py_Tuple.h
	${AXL_INC_DIR}/axl_py/axl_py_List.h
	${AXL_INC_DIR}/axl_py/axl_py_Dict.h
	${AXL_INC_DIR}/axl_py/axl_py_Bytes.h
	${AXL_INC_DIR}/axl_py/axl_py_Unicode.h
	${AXL_INC_DIR}/axl_py/axl_py_Module.h
	${AXL_INC_DIR}/axl_py/axl_py_CFunction.h
)

set(
	LIB_CPP_LIST
	axl_py_Init.cpp
	axl_py_Error.cpp
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
	${AXL_INC_DIR}/axl_py/axl_py_Pch.h
)

source_group(
	pch
	FILES
	${PCH_H_LIST}
	REGULAR_EXPRESSION cmake_pch
)

#...............................................................................
#
# axl_py library
#

include_directories(
	${PYTHON_INC_DIR}
	${AXL_INC_DIR}
)

add_library(
	axl_py
	${PCH_H_LIST}
	${LIB_H_LIST}
	${LIB_CPP_LIST}
	${LIB_RL_LIST}
)

target_precompile_headers(
	axl_py
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
	TARGETS axl_py
	DESTINATION ${AXL_INSTALL_LIB_SUBDIR}
)

#...............................................................................
