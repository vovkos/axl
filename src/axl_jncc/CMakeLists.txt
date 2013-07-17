# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2013. All rights reserved
# Author: Vladimir Gladkov
	
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

source_group (
	jncc
	FILES 
	${JNCC_H_LIST}
	${JNCC_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set (PCH_H   pch.h)
set (PCH_CPP pch.cpp)

source_group (
	pch
	FILES 	
	${PCH_H}
	${PCH_CPP}
	)

#..............................................................................
#
# axl_jnc library
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
	LLVMJIT
	LLVMMCJIT
	)

if (WIN32)
	target_link_libraries (
		axl_jncc
		ws2_32
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
 
