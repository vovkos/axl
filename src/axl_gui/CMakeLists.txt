# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2014. All rights reserved
# Author: Vladimir Gladkov

#..............................................................................
#
# gui folder
#

set (
	GUI_H_LIST
	${AXL_INC_DIR}/axl_gui_Canvas.h
	${AXL_INC_DIR}/axl_gui_Color.h
	${AXL_INC_DIR}/axl_gui_ColorAttr.h
	${AXL_INC_DIR}/axl_gui_Def.h
	${AXL_INC_DIR}/axl_gui_Engine.h
	${AXL_INC_DIR}/axl_gui_Font.h
	${AXL_INC_DIR}/axl_gui_FontDesc.h
	${AXL_INC_DIR}/axl_gui_HyperlinkAnchorArray.h
	${AXL_INC_DIR}/axl_gui_HyperText.h
	${AXL_INC_DIR}/axl_gui_Image.h
	${AXL_INC_DIR}/axl_gui_ImageDesc.h
	${AXL_INC_DIR}/axl_gui_TextAttr.h
	${AXL_INC_DIR}/axl_gui_TextAttrAnchorArray.h
	${AXL_INC_DIR}/axl_gui_TextPaint.h
	${AXL_INC_DIR}/axl_gui_Widget.h
	)

set (
	GUI_CPP_LIST
	axl_gui_Color.cpp
	axl_gui_ColorAttr.cpp
	axl_gui_Engine.cpp
	axl_gui_FontDesc.cpp
	axl_gui_FontDesc.cpp
	axl_gui_HyperlinkAnchorArray.cpp
	axl_gui_HyperText.cpp
	axl_gui_ImageDesc.cpp
	axl_gui_TextAttr.cpp
	axl_gui_TextAttrAnchorArray.cpp
	axl_gui_TextPaint.cpp
	axl_gui_Widget.cpp
	)

source_group (
	gui
	FILES
	${GUI_H_LIST}
	${GUI_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# gui_gdi folder
#

set (
	GUI_GDI_H_LIST
	${AXL_INC_DIR}/axl_gui_GdiCanvas.h
	${AXL_INC_DIR}/axl_gui_GdiCursor.h
	${AXL_INC_DIR}/axl_gui_GdiEngine.h
	${AXL_INC_DIR}/axl_gui_GdiFont.h
	${AXL_INC_DIR}/axl_gui_GdiImage.h
	${AXL_INC_DIR}/axl_gui_GdiObjectHandle.h
	${AXL_INC_DIR}/axl_gui_GdiWidget.h
	)

set (
	GUI_GDI_CPP_LIST
	axl_gui_GdiCanvas.cpp
	axl_gui_GdiCursor.cpp
	axl_gui_GdiEngine.cpp
	axl_gui_GdiFont.cpp
	axl_gui_GdiImage.cpp
	axl_gui_GdiWidget.cpp
	)

source_group (
	gui_gdi
	FILES
	${GUI_GDI_H_LIST}
	${GUI_GDI_CPP_LIST}
	)

if (NOT BUILD_AXL_GUI_GDI)
	exclude_from_build (${GUI_GDI_CPP_LIST})
endif ()

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# gui_qt folder
#

set (
	GUI_QT_H_LIST
	${AXL_INC_DIR}/axl_gui_QtCaret.h
	${AXL_INC_DIR}/axl_gui_QtEngine.h
	${AXL_INC_DIR}/axl_gui_QtWidget.h
	)

set (
	GUI_QT_CPP_LIST
	axl_gui_QtCaret.cpp
	axl_gui_QtEngine.cpp
	axl_gui_QtWidget.cpp
	)

source_group (
	gui_qt
	FILES
	${GUI_QT_H_LIST}
	${GUI_QT_CPP_LIST}
	)

if (NOT BUILD_AXL_GUI_QT)
	exclude_from_build (${GUI_QT_CPP_LIST})
endif ()

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
# axl_gui library
#

if (BUILD_AXL_GUI_QT)
	add_definitions (-D_AXL_GUI_QT)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
endif ()

add_library (
	axl_gui
	${PCH_H}
	${PCH_CPP}
	${GUI_H_LIST}
	${GUI_CPP_LIST}
	${GUI_GDI_H_LIST}
	${GUI_GDI_CPP_LIST}
	${GUI_QT_H_LIST}
	${GUI_QT_CPP_LIST}
	)

if (BUILD_AXL_GUI_QT)
	qt5_use_modules (axl_gui Widgets)
endif ()

set_pch (
	axl_gui
	${PCH_H}
	${PCH_CPP}
	)

#..............................................................................

