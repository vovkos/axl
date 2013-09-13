# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2013. All rights reserved
# Author: Vladimir Gladkov
			   
#..............................................................................
#
# log_File folder
#

set (
	LOG_FILE_H_LIST
	${AXL_INC_DIR}/axl_log_BinDataConfig.h
	${AXL_INC_DIR}/axl_log_IndexFile.h
	${AXL_INC_DIR}/axl_log_MergeCriteria.h
	${AXL_INC_DIR}/axl_log_PacketFile.h
	)

set (
	LOG_FILE_CPP_LIST
	axl_log_BinDataConfig.cpp
	axl_log_IndexFile.cpp
	axl_log_MergeCriteria.cpp
	axl_log_PacketFile.cpp
	)

source_group (
	log_File 
	FILES 
	${LOG_FILE_H_LIST}
	${LOG_FILE_CPP_LIST}
	)

#..............................................................................
#
# log_Page folder
#

set (
	LOG_PAGE_H_LIST
	${AXL_INC_DIR}/axl_log_BinLine.h
	${AXL_INC_DIR}/axl_log_BinHexLine.h
	${AXL_INC_DIR}/axl_log_BinTextLine.h
	${AXL_INC_DIR}/axl_log_Line.h
	${AXL_INC_DIR}/axl_log_Page.h
	${AXL_INC_DIR}/axl_log_TextLine.h
	)

set (
	LOG_PAGE_CPP_LIST
	axl_log_BinLine.cpp
	axl_log_BinHexLine.cpp
	axl_log_BinTextLine.cpp
	axl_log_Line.cpp
	axl_log_Page.cpp
	axl_log_TextLine.cpp
	)

source_group (
	log_Page
	FILES 
	${LOG_PAGE_H_LIST}
	${LOG_PAGE_CPP_LIST}
	)

#..............................................................................
#
# log_Plugin folder
#

set (
	LOG_PLUGIN_H_LIST
	${AXL_INC_DIR}/axl_log_Colorizer.h
	${AXL_INC_DIR}/axl_log_Filter.h
	${AXL_INC_DIR}/axl_log_HyperlinkHandler.h
	${AXL_INC_DIR}/axl_log_LineProcessor.h
	${AXL_INC_DIR}/axl_log_Representer.h
	)

set (
	LOG_PLUGIN_CPP_LIST
	axl_log_Colorizer.cpp
	axl_log_Filter.cpp
	axl_log_HyperlinkHandler.cpp
	axl_log_LineProcessor.cpp
	axl_log_Representer.cpp
	)

source_group (
	log_Plugin
	FILES 
	${LOG_PLUGIN_H_LIST}
	${LOG_PLUGIN_CPP_LIST}
	)

#..............................................................................
#
# log_Server folder
#

set (
	LOG_SERVER_H_LIST
	${AXL_INC_DIR}/axl_log_Protocol.h
	${AXL_INC_DIR}/axl_log_Server.h
	axl_log_IndexRepresenterTarget.h
	axl_log_PageRepresenterTarget.h
	axl_log_Server_Copy.h
	)

set (
	LOG_SERVER_CPP_LIST
	axl_log_IndexRepresenterTarget.cpp
	axl_log_PageRepresenterTarget.cpp
	axl_log_Protocol.cpp
	axl_log_Server.cpp
	axl_log_Server_IoThread.cpp
	axl_log_Server_ColorizeThread.cpp
	axl_log_Server_Copy.cpp
	)

source_group (
	log_Server
	FILES 
	${LOG_SERVER_H_LIST}
	${LOG_SERVER_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# log_Widget folder
#

set (
	LOG_WIDGET_H_LIST
	${AXL_INC_DIR}/axl_log_Widget.h
	)

set (
	LOG_WIDGET_CPP_LIST
	axl_log_Widget.cpp
	axl_log_Widget_Cache.cpp
	axl_log_Widget_Cursor.cpp
	axl_log_Widget_Find.cpp
	axl_log_Widget_Hyperlink.cpp
	axl_log_Widget_Keyboard.cpp
	axl_log_Widget_Mouse.cpp
	axl_log_Widget_Paint.cpp
	axl_log_Widget_ProcessLine.cpp
	axl_log_Widget_Protocol.cpp
	axl_log_Widget_Scroll.cpp
	)

source_group (
	log_Widget
	FILES
	${LOG_WIDGET_H_LIST}
	${LOG_WIDGET_CPP_LIST}
	)

exclude_from_build (
	axl_log_Widget_Hyperlink.cpp
	axl_log_Widget_ProcessLine.cpp
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
# axl_log library
#

add_library (
	axl_log
	${LOG_FILE_H_LIST}
	${LOG_FILE_CPP_LIST}
	${LOG_PAGE_H_LIST}
	${LOG_PAGE_CPP_LIST}
	${LOG_PLUGIN_H_LIST}
	${LOG_PLUGIN_CPP_LIST}
	${LOG_SERVER_H_LIST}
	${LOG_SERVER_CPP_LIST}
	${LOG_WIDGET_H_LIST}
	${LOG_WIDGET_CPP_LIST}
	${PCH_H}
	${PCH_CPP}
	)

if (AXL_USE_PCH)
	set_pch (
		axl_log
		${PCH_H} 
		${PCH_CPP}
		)
endif ()

#..............................................................................
 
