# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2013. All rights reserved
# Author: Vladimir Gladkov

project_virtual_folder (axl_jncc)

#..............................................................................
#
# jncc folder
#

set (
	JNCC_H_LIST
	axl_jncc.h
	axl_jncc_CmdLine.h
	axl_jncc_OutStream.h
	)

set (
	JNCC_CPP_LIST
	axl_jncc.cpp
	axl_jncc_CmdLine.cpp
	axl_jncc_Compile.cpp
	axl_jncc_RunFunction.cpp
	axl_jncc_Server.cpp
	axl_jncc_OutStream.cpp
	)

source_virtual_folder (
	jncc
	${JNCC_H_LIST}
	${JNCC_CPP_LIST}
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
# axl_jncc jancy compiler
#

include_directories (
	${PATH_LLVM_INC_DIR}
	${PATH_LLVM_INC_DIR_2}
	)

link_directories (${PATH_LLVM_LIB_DIR})

llvm_map_components_to_libraries (
	LLVM_LIB_LIST
	jit
	native
	MCJIT
	MCDisassembler
	)

add_executable (
	axl_jncc
	${JNCC_H_LIST}
	${JNCC_CPP_LIST}
	${PCH_H}
	${PCH_CPP}
	)

target_link_libraries (
	axl_jncc
	axl_jnc
	axl_io
	axl_core
	axl_parse
	${LLVM_LIB_LIST}
	)

if (WIN32)
	target_link_libraries (
		axl_jncc
		ws2_32
		)
elseif (UNIX)
	target_link_libraries (
		axl_jncc
		rt
		pthread
		)
endif ()

if (AXL_USE_PCH)
	set_pch (
		axl_jncc
		${PCH_H}
		${PCH_CPP}
		)
endif ()

#..............................................................................

# macro for running jancy compiler on specified list of test source files

macro (
add_jncc_tests
	_FLAGS
	_DIR
	# ...
	)

	set (_JNCC ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/axl_jncc)
	set (_FILE_NAME_LIST ${ARGN})

	foreach (_FILE_NAME ${_FILE_NAME_LIST})

		add_test (
			NAME ${_FILE_NAME}
			WORKING_DIRECTORY ${_DIR}
			COMMAND ${_JNCC} ${_FLAGS} ${_FILE_NAME}
			)
	endforeach ()
endmacro ()

#..............................................................................
