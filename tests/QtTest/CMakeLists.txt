# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2013. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................

project_virtual_folder (QtTest)

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
	jancycolorizer.cpp
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

source_virtual_folder (
	src
	${SRC_H_LIST}
	${SRC_CPP_LIST}
	${SRC_RL_LIST}
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

pop (CMAKE_CURRENT_BINARY_DIR)

source_virtual_folder (
	res
	${RES_UI_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# gen folder
#

set (GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen)

push_and_set (CMAKE_CURRENT_BINARY_DIR ${GEN_DIR})

set (
	MOC_H_LIST
	mainwindow.h
	${AXL_INC_DIR}/axl_gui_QtWidget.h
	)

qt5_wrap_cpp_to (
	GEN_MOC_CPP_LIST
	${GEN_DIR}
	${MOC_H_LIST}
	)

qt5_wrap_ui (
	GEN_UI_H_LIST
	${RES_UI_LIST}
	)

add_ragel_step (
	jancycolorizer.rl.cpp
	jancycolorizer.rl
	)

set (
	GEN_RL_CPP
	${GEN_DIR}/jancycolorizer.rl.cpp
	)

pop (CMAKE_CURRENT_BINARY_DIR)

exclude_from_build (${GEN_RL_CPP_LIST})  # include "*.rl.cpp" manually
exclude_from_build (${GEN_MOC_CPP_LIST}) # include "moc_*.cpp" manually

source_virtual_folder (
	gen
	${GEN_MOC_CPP_LIST}
	${GEN_UI_H_LIST}
	${GEN_RL_CPP_LIST}
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

source_virtual_folder (
	script
	${SCRIPT_JNC_LIST}
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
# QtTest compiler test
#

if (MSVC)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zm200")
endif ()

include_directories (${GEN_DIR})

add_executable (
	QtTest
	${PCH_H}
	${PCH_CPP}
	${SRC_H_LIST}
	${SRC_CPP_LIST}
	${RES_UI_LIST}
	${GEN_MOC_CPP_LIST}
	${GEN_UI_H_LIST}
	${GEN_RL_CPP_LIST}
	${SCRIPT_JNC_LIST}
	)

target_link_libraries (
	QtTest
	axl_log
	axl_gui
	axl_io
	axl_core
	ws2_32
	)

qt5_use_modules (QtTest Widgets)

if (AXL_USE_PCH)
	set_pch (
		QtTest
		${PCH_H}
		${PCH_CPP}
		)
endif ()

set_target_properties (
	QtTest
	PROPERTIES
	FOLDER tests
	LINKER_LANGUAGE C
	)

#..............................................................................
