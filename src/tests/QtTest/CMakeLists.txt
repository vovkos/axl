# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2013. All rights reserved
# Author: Vladimir Gladkov
			   
#..............................................................................
#
# src folder
#

set (
	SRC_H_LIST 
	mylogrepresentor.h
	mylogwidget.h
	mainwindow.h
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

source_group (
	moc
	FILES 
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

source_group (
	res
	FILES 
	${RES_UI_LIST}
	${UI_H_LIST}
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
