# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2013. All rights reserved
# Author: Vladimir Gladkov

set (VIRTUAL_FOLDER_SUB_DIR QtTest)

#..............................................................................
#
# src folder
#

set (
	SRC_H_LIST
	jancycolorizer.h
	mylogproxy.h
	mylogrepresenter.h
	mylogserver.h
	mylogwidget.h
	mainwindow.h
	)

set (
	SRC_CPP_LIST
	jancycolorizer.rl.cpp
	mylogproxy.cpp
	mylogrepresenter.cpp
	mylogserver.cpp
	mylogwidget.cpp
	main.cpp
	mainwindow.cpp
	)

set (
	SRC_RL_LIST
	jancycolorizer.rl
	)

push_and_set (CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_ragel_step (
	jancycolorizer.rl.cpp
	jancycolorizer.rl
	)

pop (CMAKE_CURRENT_BINARY_DIR)

virtual_folder (
	src
	${SRC_H_LIST}
	${SRC_CPP_LIST}
	${SRC_RL_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# moc folder
#

set (
	MOC_SRC_H_LIST
	mainwindow.h
	${AXL_INC_DIR}/axl_gui_qt_Widget.h
	)

qt5_wrap_cpp_to (
	MOC_CPP_LIST
	${CMAKE_CURRENT_SOURCE_DIR}
	${MOC_SRC_H_LIST}
	)

exclude_from_build (${MOC_CPP_LIST}) # include "moc_*.cpp" manually

virtual_folder (
	moc
	${MOC_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# res folder
#

set (
	RES_UI_LIST
	mainwindow.ui
	)

push_and_set (CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR})

qt5_wrap_ui (
	UI_H_LIST
	${RES_UI_LIST}
	)

pop (CMAKE_CURRENT_BINARY_DIR)

virtual_folder (
	res
	${RES_UI_LIST}
	${UI_H_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# script folder
#

set (
	SCRIPT_JNC_LIST
	script/gui.jnc
	script/log.jnc
	script/mylogrepresenter.jnc
	script/std.jnc
	)

virtual_folder (
	script
	${SCRIPT_JNC_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set (PCH_H   pch.h)
set (PCH_CPP pch.cpp)

virtual_folder (
	pch
	${PCH_H}
	${PCH_CPP}
	)

#..............................................................................
#
# QtTest compiler test
#

if (MSVC)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zm200")
endif ()

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
	QtTest
	${PCH_H}
	${PCH_CPP}
	${SRC_H_LIST}
	${SRC_CPP_LIST}
	${RES_UI_LIST}
	${MOC_CPP_LIST}
	${UI_H_LIST}
	${SCRIPT_JNC_LIST}
	)

target_link_libraries (
	QtTest
#	axl_parse
#	axl_jnc
	axl_log
	axl_gui
	axl_io
	axl_core
	ws2_32
	${LLVM_LIB_LIST}
	)

qt5_use_modules (QtTest Widgets)

if (AXL_USE_PCH)
	set_pch (
		QtTest
		${PCH_H}
		${PCH_CPP}
		)
endif ()

#..............................................................................
