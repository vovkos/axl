# This file is part of AXL (R) Library
# Tibbo Technology Inc (C) 2004-2013. All rights reserved
# Author: Vladimir Gladkov
			   
#..............................................................................
#
# log folder
#

set (
	LOG_H_LIST
	${AXL_INC_DIR}/axl_log_BinDataConfig.h
	${AXL_INC_DIR}/axl_log_IndexFile.h
	${AXL_INC_DIR}/axl_log_IndexMgr.h
	${AXL_INC_DIR}/axl_log_IndexRepresentorTarget.h
	${AXL_INC_DIR}/axl_log_MergeCriteria.h
	${AXL_INC_DIR}/axl_log_PacketFile.h
	${AXL_INC_DIR}/axl_log_Represent.h
	${AXL_INC_DIR}/axl_log_Transform.h
	${AXL_INC_DIR}/axl_log_VolatilePacketFilter.h
	)

set (
	LOG_CPP_LIST
	axl_log_IndexFile.cpp
	axl_log_IndexMgr.cpp
	axl_log_IndexRepresentorTarget.cpp
	axl_log_MergeCriteria.cpp
	axl_log_PacketFile.cpp
	axl_log_Represent.cpp
	axl_log_Transform.cpp
	axl_log_VolatilePacketFilter.cpp
	)

source_group (
	log 
	FILES 
	${LOG_H_LIST}
	${LOG_CPP_LIST}
	)

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# log_Widget folder
#

set (
	LOG_WIDGET_H_LIST
	${AXL_INC_DIR}/axl_log_BinLine.h
	${AXL_INC_DIR}/axl_log_CacheMgr.h
	${AXL_INC_DIR}/axl_log_CachePage.h
	${AXL_INC_DIR}/axl_log_CacheRepresentorTarget.h
	${AXL_INC_DIR}/axl_log_Colorize.h
	${AXL_INC_DIR}/axl_log_ColorizeMgr.h
	${AXL_INC_DIR}/axl_log_Line.h
	${AXL_INC_DIR}/axl_log_Widget.h
	axl_log_Widget_Copy.h
	)

set (
	LOG_WIDGET_CPP_LIST
	axl_log_BinLine.cpp
	axl_log_CacheMgr.cpp
	axl_log_CachePage.cpp
	axl_log_CacheRepresentorTarget.cpp
	axl_log_Colorize.cpp
	axl_log_ColorizeMgr.cpp
	axl_log_Line.cpp
	axl_log_Widget.cpp
	axl_log_Widget_Copy.cpp
	axl_log_Widget_Cursor.cpp
	axl_log_Widget_Find.cpp
	axl_log_Widget_Hyperlink.cpp
	axl_log_Widget_Keyboard.cpp
	axl_log_Widget_Mouse.cpp
	axl_log_Widget_Paint.cpp
	axl_log_Widget_ProcessLine.cpp
	axl_log_Widget_Scroll.cpp
	)

source_group (
	log_Widget
	FILES
	${LOG_WIDGET_H_LIST}
	${LOG_WIDGET_CPP_LIST}
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
	${LOG_H_LIST}
	${LOG_CPP_LIST}
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
 
