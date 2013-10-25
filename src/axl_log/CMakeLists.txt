# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2013. All rights reserved
# Author: Vladimir Gladkov

project_virtual_folder (axl_log)

#..............................................................................
#
# log_File folder
#

set (
	LOG_FILE_H_LIST
	${AXL_INC_DIR}/axl_log_BinDataConfig.h
	${AXL_INC_DIR}/axl_log_IndexFile.h
	${AXL_INC_DIR}/axl_log_MergeCriteria.h
	${AXL_INC_DIR}/axl_log_MergeFile.h
	${AXL_INC_DIR}/axl_log_PacketFile.h
	)

set (
	LOG_FILE_CPP_LIST
	axl_log_BinDataConfig.cpp
	axl_log_IndexFile.cpp
	axl_log_MergeCriteria.cpp
	axl_log_MergeFile.cpp
	axl_log_PacketFile.cpp
	)

source_virtual_folder (
	log_File
	${LOG_FILE_H_LIST}
	${LOG_FILE_CPP_LIST}
	)

#..............................................................................
#
# log_Line folder
#

set (
	LOG_PAGE_H_LIST
	${AXL_INC_DIR}/axl_log_BinLine.h
	${AXL_INC_DIR}/axl_log_BinHexLine.h
	${AXL_INC_DIR}/axl_log_BinTextLine.h
	${AXL_INC_DIR}/axl_log_Line.h
	${AXL_INC_DIR}/axl_log_TextLine.h
	)

set (
	LOG_PAGE_CPP_LIST
	axl_log_BinLine.cpp
	axl_log_BinHexLine.cpp
	axl_log_BinTextLine.cpp
	axl_log_Line.cpp
	axl_log_TextLine.cpp
	)

source_virtual_folder (
	log_Line
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
	${AXL_INC_DIR}/axl_log_RagelColorizer.h
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

source_virtual_folder (
	log_Plugin
	${LOG_PLUGIN_H_LIST}
	${LOG_PLUGIN_CPP_LIST}
	)

#..............................................................................
#
# log_Jancy folder
#

set (
	LOG_JANCY_H_LIST
	${AXL_INC_DIR}/axl_log_JancyColorizer.h
	${AXL_INC_DIR}/axl_log_JancyFilter.h
	${AXL_INC_DIR}/axl_log_JancyRepresenter.h
	)

set (
	LOG_JANCY_CPP_LIST
	axl_log_JancyColorizer.cpp
	axl_log_JancyFilter.cpp
	axl_log_JancyRepresenter.cpp
	)

source_virtual_folder (
	log_Jancy
	${LOG_JANCY_H_LIST}
	${LOG_JANCY_CPP_LIST}
	)

if (NOT AXL_BUILD_LOG_JANCY)
	exclude_from_build (${LOG_JANCY_CPP_LIST})
endif ()

#..............................................................................
#
# log_Server folder
#

set (
	LOG_SERVER_H_LIST
	${AXL_INC_DIR}/axl_log_ColorizeMgr.h
	${AXL_INC_DIR}/axl_log_FilterMgr.h
	${AXL_INC_DIR}/axl_log_IndexMgr.h
	${AXL_INC_DIR}/axl_log_LineRepresentMgr.h
	${AXL_INC_DIR}/axl_log_Protocol.h
	${AXL_INC_DIR}/axl_log_Server.h
	axl_log_ColorizerStateCalcTarget.h
	axl_log_IndexRepresenterTarget.h
	axl_log_LineColorizerTarget.h
	axl_log_LineRepresenterTarget.h
	)


set (
	LOG_SERVER_CPP_LIST
	axl_log_ColorizeMgr.cpp
	axl_log_ColorizerStateCalcTarget.cpp
	axl_log_FilterMgr.cpp
	axl_log_IndexMgr.cpp
	axl_log_IndexRepresenterTarget.cpp
	axl_log_LineColorizerTarget.cpp
	axl_log_LineRepresenterTarget.cpp
	axl_log_LineRepresentMgr.cpp
	axl_log_Protocol.cpp
	axl_log_Server.cpp
	)

source_virtual_folder (
	log_Server
	${LOG_SERVER_H_LIST}
	${LOG_SERVER_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# log_Widget folder
#

set (
	LOG_WIDGET_H_LIST
	${AXL_INC_DIR}/axl_log_CacheMgr.h
	${AXL_INC_DIR}/axl_log_CachePage.h
	${AXL_INC_DIR}/axl_log_Widget.h
	)

set (
	LOG_WIDGET_CPP_LIST
	axl_log_CacheMgr.cpp
	axl_log_CachePage.cpp
	axl_log_Widget.cpp
	axl_log_Widget_Cursor.cpp
	axl_log_Widget_Find.cpp
	axl_log_Widget_Hyperlink.cpp
	axl_log_Widget_Keyboard.cpp
	axl_log_Widget_Mouse.cpp
	axl_log_Widget_Paint.cpp
	axl_log_Widget_ProcessLine.cpp
	axl_log_Widget_Scroll.cpp
	)

source_virtual_folder (
	log_Widget
	${LOG_WIDGET_H_LIST}
	${LOG_WIDGET_CPP_LIST}
	)

exclude_from_build (
	axl_log_Widget_ProcessLine.cpp
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
# axl_log library
#

if (AXL_BUILD_LOG_JANCY)
	include_directories (
		${PATH_LLVM_INC_DIR}
		${PATH_LLVM_INC_DIR_2}
		)

	add_definitions (-D_AXL_LOG_JANCY)
endif ()

add_library (
	axl_log
	${LOG_FILE_H_LIST}
	${LOG_FILE_CPP_LIST}
	${LOG_PAGE_H_LIST}
	${LOG_PAGE_CPP_LIST}
	${LOG_PLUGIN_H_LIST}
	${LOG_PLUGIN_CPP_LIST}
	${LOG_JANCY_H_LIST}
	${LOG_JANCY_CPP_LIST}
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

