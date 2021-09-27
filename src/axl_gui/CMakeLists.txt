#...............................................................................
#
#  This file is part of the AXL library.
#
#  AXL is distributed under the MIT license.
#  For details see accompanying license.txt file,
#  the public copy of which is also available at:
#  http://tibbo.com/downloads/archive/axl/license.txt
#
#...............................................................................

#
# gui folder
#

set(
	GUI_H_LIST
	${AXL_INC_DIR}/axl_gui/axl_gui_AnsiAttrParser.h
	${AXL_INC_DIR}/axl_gui/axl_gui_Canvas.h
	${AXL_INC_DIR}/axl_gui/axl_gui_Color.h
	${AXL_INC_DIR}/axl_gui/axl_gui_ColorAttr.h
	${AXL_INC_DIR}/axl_gui/axl_gui_Def.h
	${AXL_INC_DIR}/axl_gui/axl_gui_Engine.h
	${AXL_INC_DIR}/axl_gui/axl_gui_Font.h
	${AXL_INC_DIR}/axl_gui/axl_gui_FontDesc.h
	${AXL_INC_DIR}/axl_gui/axl_gui_HyperlinkAnchorArray.h
	${AXL_INC_DIR}/axl_gui/axl_gui_HyperText.h
	${AXL_INC_DIR}/axl_gui/axl_gui_Image.h
	${AXL_INC_DIR}/axl_gui/axl_gui_ImageDesc.h
	${AXL_INC_DIR}/axl_gui/axl_gui_TextAttr.h
	${AXL_INC_DIR}/axl_gui/axl_gui_TextAttrAnchorArray.h
	${AXL_INC_DIR}/axl_gui/axl_gui_TextPainter.h
	${AXL_INC_DIR}/axl_gui/axl_gui_Widget.h
)

set(
	GUI_CPP_LIST
	axl_gui_AnsiAttrParser.cpp
	axl_gui_Color.cpp
	axl_gui_Engine.cpp
	axl_gui_FontDesc.cpp
	axl_gui_FontDesc.cpp
	axl_gui_HyperlinkAnchorArray.cpp
	axl_gui_HyperText.cpp
	axl_gui_ImageDesc.cpp
	axl_gui_TextAttrAnchorArray.cpp
	axl_gui_TextPainter.cpp
	axl_gui_Widget.cpp
)

source_group(
	gui
	FILES
	${GUI_H_LIST}
	${GUI_CPP_LIST}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# gui_gdi folder
#

set(
	GUI_GDI_H_LIST
	${AXL_INC_DIR}/axl_gui/axl_gui_GdiCanvas.h
	${AXL_INC_DIR}/axl_gui/axl_gui_GdiCursor.h
	${AXL_INC_DIR}/axl_gui/axl_gui_GdiEngine.h
	${AXL_INC_DIR}/axl_gui/axl_gui_GdiFont.h
	${AXL_INC_DIR}/axl_gui/axl_gui_GdiImage.h
	${AXL_INC_DIR}/axl_gui/axl_gui_GdiObjectHandle.h
	${AXL_INC_DIR}/axl_gui/axl_gui_GdiWidget.h
)

set(
	GUI_GDI_CPP_LIST
	axl_gui_GdiCanvas.cpp
	axl_gui_GdiCursor.cpp
	axl_gui_GdiEngine.cpp
	axl_gui_GdiFont.cpp
	axl_gui_GdiImage.cpp
	axl_gui_GdiWidget.cpp
)

source_group(
	gui_gdi
	FILES
	${GUI_GDI_H_LIST}
	${GUI_GDI_CPP_LIST}
)

if(NOT BUILD_AXL_GUI_GDI)
	axl_exclude_from_build(${GUI_GDI_CPP_LIST})
endif()

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# gui_qt folder
#

set(
	GUI_QT_H_LIST
	${AXL_INC_DIR}/axl_gui/axl_gui_QtCaret.h
	${AXL_INC_DIR}/axl_gui/axl_gui_QtEngine.h
	${AXL_INC_DIR}/axl_gui/axl_gui_QtWidget.h
	${AXL_INC_DIR}/axl_gui/axl_gui_QtRagelSyntaxHighlighter.h
)

set(
	GUI_QT_CPP_LIST
	axl_gui_QtCaret.cpp
	axl_gui_QtEngine.cpp
	axl_gui_QtWidget.cpp
)

source_group(
	gui_qt
	FILES
	${GUI_QT_H_LIST}
	${GUI_QT_CPP_LIST}
)

if(NOT BUILD_AXL_GUI_QT)
	axl_exclude_from_build(${GUI_QT_CPP_LIST})
endif()

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# pch folder
#

set(PCH_H   pch.h)
set(PCH_CPP pch.cpp)

set(
	PCH_H_LIST
	${PCH_H}
	${AXL_INC_DIR}/axl_gui/axl_gui_Pch.h
	${AXL_INC_DIR}/axl_gui/axl_gui_GdiPch.h
	${AXL_INC_DIR}/axl_gui/axl_gui_QtPch.h
)

source_group(
	pch
	FILES
	${PCH_H_LIST}
	${PCH_CPP}
)

#...............................................................................
#
# axl_gui library
#

include_directories(${AXL_INC_DIR})

if(BUILD_AXL_GUI_QT)
	add_definitions(
		-D_AXL_GUI_QT=1
		-DQT_COMPILING_QSTRING_COMPAT_CPP
		-DQT_COMPILING_QIMAGE_COMPAT_CPP
	)
endif()

add_library(
	axl_gui
	${PCH_H_LIST}
	${PCH_CPP}
	${GUI_H_LIST}
	${GUI_CPP_LIST}
	${GUI_GDI_H_LIST}
	${GUI_GDI_CPP_LIST}
	${GUI_QT_H_LIST}
	${GUI_QT_CPP_LIST}
)

if(BUILD_AXL_GUI_QT)
	qt5_use_modules_alt(axl_gui Widgets)
endif()

axl_set_pch(
	axl_gui
	${PCH_H}
	${PCH_CPP}
)

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

axl_install_redirected_includes(
	${AXL_INSTALL_INC_SUBDIR}
	axl_gui
	${LIB_H_LIST}
)

install(
	TARGETS axl_gui
	DESTINATION ${AXL_INSTALL_LIB_SUBDIR}
)

#...............................................................................
