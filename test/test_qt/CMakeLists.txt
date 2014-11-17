# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2013. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................
#
# src folder
#

set (
	SRC_H_LIST
	mainwindow.h
	)

set (
	SRC_CPP_LIST
	main.cpp
	mainwindow.cpp
	)

source_group (
	src
	FILES
	${SRC_H_LIST}
	${SRC_CPP_LIST}
	${SRC_RL_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# gen folder
#

set (GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen)
file (MAKE_DIRECTORY ${GEN_DIR})

push_and_set (CMAKE_CURRENT_BINARY_DIR ${GEN_DIR})

set (
	MOC_H_LIST
	mainwindow.h
	${AXL_INC_DIR}/axl_gui_QtWidget.h
	)

qt5_wrap_cpp_alt (
	GEN_MOC_CPP_LIST
	${MOC_H_LIST}
	)

pop (CMAKE_CURRENT_BINARY_DIR)

exclude_from_build (${GEN_MOC_CPP_LIST}) # include "moc_*.cpp" manually

source_group (
	gen
	FILES
	${GEN_MOC_CPP_LIST}
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
# test_qt compiler test
#

include_directories (
	${AXL_INC_DIR}
	${GEN_DIR}
	)

link_directories (${AXL_LIB_DIR})

add_executable (
	test_qt
	${PCH_H}
	${PCH_CPP}
	${SRC_H_LIST}
	${SRC_CPP_LIST}
	${GEN_MOC_CPP_LIST}
	)

target_link_libraries (
	test_qt
	axl_gui
	axl_io
	axl_core
	)

if (UNIX)
	target_link_libraries (
		test_qt
		rt
		pthread
		)
endif ()

qt5_use_modules (test_qt Widgets)

if (WIN32)
	copy_qt_dll_files (test_qt Core Gui Widgets)
endif ()

set_pch (
	test_qt
	${PCH_H}
	${PCH_CPP}
	${Qt5Core_EXECUTABLE_COMPILE_FLAGS}
	)

set_target_properties (
	test_qt
	PROPERTIES
	FOLDER test
	)

#..............................................................................
