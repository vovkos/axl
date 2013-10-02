# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2013. All rights reserved
# Author: Vladimir Gladkov
  
#..............................................................................
#
# gui folder
#

set (
	GUI_H_LIST
	${AXL_INC_DIR}/axl_gui_Canvas.h
	${AXL_INC_DIR}/axl_gui_Color.h
	${AXL_INC_DIR}/axl_gui_Cursor.h
	${AXL_INC_DIR}/axl_gui_Def.h
	${AXL_INC_DIR}/axl_gui_Engine.h
	${AXL_INC_DIR}/axl_gui_Font.h
	${AXL_INC_DIR}/axl_gui_HyperlinkAnchorArray.h
	${AXL_INC_DIR}/axl_gui_HyperText.h
	${AXL_INC_DIR}/axl_gui_Image.h
	${AXL_INC_DIR}/axl_gui_ImageList.h
	${AXL_INC_DIR}/axl_gui_TextAttrAnchorArray.h
	${AXL_INC_DIR}/axl_gui_TextPaint.h
	${AXL_INC_DIR}/axl_gui_Widget.h
	)

set (
	GUI_CPP_LIST
	axl_gui_Canvas.cpp
	axl_gui_Color.cpp
	axl_gui_Cursor.cpp
	axl_gui_Engine.cpp
	axl_gui_Font.cpp
	axl_gui_HyperlinkAnchorArray.cpp
	axl_gui_HyperText.cpp
	axl_gui_Image.cpp
	axl_gui_ImageList.cpp
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
	${AXL_INC_DIR}/axl_gui_gdi_Bitmap.h
	${AXL_INC_DIR}/axl_gui_gdi_Cursor.h
	${AXL_INC_DIR}/axl_gui_gdi_Dc.h
	${AXL_INC_DIR}/axl_gui_gdi_Engine.h
	${AXL_INC_DIR}/axl_gui_gdi_Font.h
	${AXL_INC_DIR}/axl_gui_gdi_ImageList.h
	${AXL_INC_DIR}/axl_gui_gdi_Widget.h
	)

set (
	GUI_GDI_CPP_LIST
	axl_gui_gdi_Bitmap.cpp
	axl_gui_gdi_Cursor.cpp
	axl_gui_gdi_Dc.cpp
	axl_gui_gdi_Engine.cpp
	axl_gui_gdi_Font.cpp
	axl_gui_gdi_ImageList.cpp
	axl_gui_gdi_Widget.cpp
	)

source_group (
	gui_gdi
	FILES 
	${GUI_GDI_H_LIST}
	${GUI_GDI_CPP_LIST}
	)

if (NOT AXL_BUILD_GUI_GDI)
	exclude_from_build (${GUI_GDI_CPP_LIST})
endif ()

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# gui_qt folder
#

set (
	GUI_QT_H_LIST
	${AXL_INC_DIR}/axl_gui_qt_Cursor.h
	${AXL_INC_DIR}/axl_gui_qt_Engine.h
	${AXL_INC_DIR}/axl_gui_qt_Font.h
	${AXL_INC_DIR}/axl_gui_qt_Image.h
	${AXL_INC_DIR}/axl_gui_qt_ImageList.h
	${AXL_INC_DIR}/axl_gui_qt_Painter.h
	${AXL_INC_DIR}/axl_gui_qt_Pch.h
	${AXL_INC_DIR}/axl_gui_qt_Pixmap.h
	${AXL_INC_DIR}/axl_gui_qt_Widget.h
	)

set (
	GUI_QT_CPP_LIST
	axl_gui_qt_Cursor.cpp
	axl_gui_qt_Engine.cpp
	axl_gui_qt_Font.cpp
	axl_gui_qt_Image.cpp
	axl_gui_qt_ImageList.cpp
	axl_gui_qt_Painter.cpp
	axl_gui_qt_Pixmap.cpp
	axl_gui_qt_Widget.cpp
	)

source_group (
	gui_qt
	FILES 
	${GUI_QT_H_LIST}
	${GUI_QT_CPP_LIST}
	)

exclude_from_build (
	axl_gui_qt_Image.cpp
	axl_gui_qt_ImageList.cpp
	)

if (NOT AXL_BUILD_GUI_QT)
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

if (AXL_BUILD_GUI_QT)
	add_definitions (-D_AXL_GUI_QT)
endif ()

add_library (
	axl_gui
	${GUI_H_LIST}
	${GUI_CPP_LIST}
	${GUI_GDI_H_LIST}
	${GUI_GDI_CPP_LIST}
	${GUI_QT_H_LIST}
	${GUI_QT_CPP_LIST}
	${PCH_H}
	${PCH_CPP}
	)

if (AXL_BUILD_GUI_QT)
	qt5_use_modules (axl_gui Widgets)	
endif ()

if (AXL_USE_PCH)
	set_pch (
		axl_gui
		${PCH_H} 
		${PCH_CPP}
		)
endif ()

#..............................................................................
 
