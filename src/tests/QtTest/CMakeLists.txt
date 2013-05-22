# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2013. All rights reserved
# Author: Vladimir Gladkov
			   
#..............................................................................
#
# src folder
#

set (
	SRC_MOC_H_LIST
	mainwindow.h
	)

set (
	SRC_H_LIST 
	${SRC_MOC_H_LIST}
	mylogrepresentor.h
	mylogwidget.h
	)

set (
	SRC_CPP_LIST
	mylogrepresentor.cpp
	mylogwidget.cpp
	main.cpp
	mainwindow.cpp
	)

source_group (
	src 
	FILES 
	${SRC_H_LIST}
	${SRC_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# res folder
#

set (
	RES_UI_LIST
	mainwindow.ui
	)

source_group (
	res
	FILES 
	${RES_UI_LIST}
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
# QtTest compiler test
#

set (PREV_CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})
set (CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR})

qt5_wrap_cpp (
	MOC_CPP_LIST
	${SRC_MOC_H_LIST}
	)

exclude_from_build (${MOC_CPP_LIST})

qt5_wrap_ui (
	UI_H_LIST
	${RES_UI_LIST}
	)

set (CMAKE_CURRENT_BINARY_DIR ${PREV_CMAKE_CURRENT_BINARY_DIR})

add_executable (
	QtTest 
	${SRC_H_LIST}
	${SRC_CPP_LIST}
	${RES_UI_LIST}
	${MOC_CPP_LIST}
	${UI_H_LIST}
	${PCH_H}
	${PCH_CPP}
	)

target_link_libraries (
	QtTest
	axl_log
	axl_gui
	axl_io
	axl_core
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
