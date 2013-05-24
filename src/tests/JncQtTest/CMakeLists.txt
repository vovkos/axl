# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2013. All rights reserved
# Author: Vladimir Gladkov
			   
#..............................................................................
#
# src folder
#

set (
	SRC_MOC_H_LIST
	disassembly.h
	editor.h
	highlighter.h
	jancyhighlighter.h
	llvmir.h
	llvmirhighlighter.h
	mainwindow.h
	mdichild.h
	modulepane.h
	output.h
	)

set (
	SRC_H_LIST 
	${SRC_MOC_H_LIST}
	)

set (
	SRC_CPP_LIST
	disassembly.cpp
	editor.cpp
	highlighter.cpp
	jancyhighlighter.rl.cpp
	llvmir.cpp
	llvmirhighlighter.rl.cpp
	main.cpp
	mainwindow.cpp
	mdichild.cpp
	modulepane.cpp 
	output.cpp
	)

set (
	SRC_RL_LIST
	jancyhighlighter.rl
	llvmirhighlighter.rl
	)

push_and_set (CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_ragel_step (
	jancyhighlighter.rl.cpp
	jancyhighlighter.rl
	) 

add_ragel_step (
	llvmirhighlighter.rl.cpp
	llvmirhighlighter.rl
	) 

pop (CMAKE_CURRENT_BINARY_DIR)

source_group (
	src 
	FILES 
	${SRC_H_LIST}
	${SRC_CPP_LIST}
	${SRC_RL_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# res folder
#

set (
	RES_PNG_LIST
	res/compile.png
	res/new.png
	res/open.png
	res/run.png
	res/save.png
	)

set (
	RES_QRC_LIST
	jancyedit.qrc
	)

push_and_set (CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR})

qt5_add_resources (
	QRC_CPP_LIST
	${RES_QRC_LIST}
	)

pop (CMAKE_CURRENT_BINARY_DIR)

exclude_from_build (${QRC_CPP_LIST}) # include "qrc_*.cpp" manually

source_group (
	res
	FILES 
	${RES_PNG_LIST}
	${RES_QRC_LIST}
	${QRC_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# moc folder
#

push_and_set (CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR})

qt5_wrap_cpp (
	MOC_CPP_LIST
	${SRC_MOC_H_LIST}
	)

pop (CMAKE_CURRENT_BINARY_DIR)

exclude_from_build (${MOC_CPP_LIST}) # include "moc_*.cpp" manually

source_group (
	moc
	FILES 
	${MOC_CPP_LIST}
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
# JncQtTest compiler test
#

include_directories (
	${PATH_LLVM_INC_DIR}
	${PATH_LLVM_INC_DIR_2}
	)

link_directories (${PATH_LLVM_LIB_DIR})

add_executable (
	JncQtTest
	${SRC_H_LIST}
	${SRC_CPP_LIST}
	${SRC_RL_LIST}
	${RES_PNG_LIST}
	${RES_QRC_LIST}
	${QRC_CPP_LIST}
	${MOC_CPP_LIST}
	${UI_H_LIST}
	${PCH_H}
	${PCH_CPP}
	)

llvm_map_components_to_libraries (
	LLVM_LIB_LIST 
	jit 
	native
	MCDisassembler
	)

target_link_libraries (
	JncQtTest
	axl_jnc
	axl_io
	axl_core
	axl_parse
	${LLVM_LIB_LIST}
	)

qt5_use_modules (JncQtTest Widgets)

if (AXL_USE_PCH)
	set_pch (
		JncQtTest
		${PCH_H} 
		${PCH_CPP}
		${Qt5Core_EXECUTABLE_COMPILE_FLAGS}
		)
endif ()

#..............................................................................
