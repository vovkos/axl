// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_log_CursorPos.h"
#include "axl_log_MessageFile.h"
#include "axl_log_Representor.h"
#include "axl_log_Filter.h"
#include "axl_log_Colorizer.h"
#include "axl_log_HyperlinkHandler.h"

AXL_EXTERN_C_BEGIN

#define axl_log_WLogCtrl _T("axl_log_ctrl")

//.............................................................................

typedef struct _axl_log_TMsgSetMessageFile axl_log_TMsgSetMessageFile;

struct _axl_log_TMsgSetMessageFile
{
	axl_log_TMessageFile* m_pFile;
	axl_log_TRepresentor* m_pRepresentor;
};

typedef struct _axl_log_TMsgSetFindPattern axl_log_TMsgSetFindPattern;

struct _axl_log_TMsgSetFindPattern
{
	const void* m_p;
	size_t m_Size;
	bool_t m_DoMatchCase;
};

typedef 
void
(AXL_API*
 axl_log_FProcessLine)(
	void* pContext,
	axl_log_TLogCtrl* pLogCtrl,
	axl_log_TLine* pLine,
	axl_log_TLine* pPrevLine,
	long StartCol, // if StartCol == -1 then it is NOT the first line in range
	long EndCol    // if EndCol == -1 then it is NOT the last line in range
	);


typedef 
void 
(AXL_API*
 axl_log_FProcessBinBlock)(
	void* pContext,
	const void* p,
	size_t Size
	);

//.............................................................................

// log ctrl messages

enum
{
	axl_log_EMsg_First = (WM_USER + 1000),

	axl_log_EMsg_GetMessageFile,
	axl_log_EMsg_SetMessageFile,
	axl_log_EMsg_GetRepresentor,
	axl_log_EMsg_SetRepresentor,
	axl_log_EMsg_GetFilter,
	axl_log_EMsg_SetFilter,
	axl_log_EMsg_GetColorizer,
	axl_log_EMsg_SetColorizer,
	axl_log_EMsg_GetHyperlinkHandler,
	axl_log_EMsg_SetHyperlinkHandler,

	axl_log_EMsg_GetImageList,
	axl_log_EMsg_SetImageList,
	axl_log_EMsg_IsIconVisible,
	axl_log_EMsg_ShowIcon,
	axl_log_EMsg_GetOffsetWidth,
	axl_log_EMsg_SetOffsetWidth,
	axl_log_EMsg_IsTimestampVisible,
	axl_log_EMsg_ShowTimestamp,
	axl_log_EMsg_GetTimestampFormat,
	axl_log_EMsg_SetTimestampFormat,
	axl_log_EMsg_GetPreferredDataConfig,
	axl_log_EMsg_SetPreferredDataConfig,

	axl_log_EMsg_UpdateLog,
	axl_log_EMsg_ReRepresentAll,
	axl_log_EMsg_ReFilterAll,
	axl_log_EMsg_ReColorizeAll,
	axl_log_EMsg_ModifyLineVolatileFlags,

	axl_log_EMsg_GetLineCount,
	axl_log_EMsg_GetLine,
	axl_log_EMsg_GetNextLine,
	axl_log_EMsg_GetPrevLine,
	axl_log_EMsg_GetLineString,

	axl_log_EMsg_GetCursorPos,
	axl_log_EMsg_SetCursorPos,
	axl_log_EMsg_GetSelection,
	axl_log_EMsg_SetSelection,
	axl_log_EMsg_IsSelectionEmpty,
	axl_log_EMsg_GetHilite,
	axl_log_EMsg_SetHilite,
	axl_log_EMsg_SelectBinRange,
	axl_log_EMsg_HiliteBinRange,
	axl_log_EMsg_EnsureVisible,
	axl_log_EMsg_EnsureVisibleRange,

	axl_log_EMsg_GetRangeBinBlock,
	axl_log_EMsg_ProcessRange,
	axl_log_EMsg_ProcessRangeAsBinBlock,

	axl_log_EMsg_GetSelectionBinBlock,
	axl_log_EMsg_ProcessSelection,
	axl_log_EMsg_ProcessSelectionAsBinBlock,

	axl_log_EMsg_SetFindPattern,
	axl_log_EMsg_CanFind,
	axl_log_EMsg_FindNext,
	axl_log_EMsg_FindPrev,

	axl_log_EMsg_CanCopy,
	axl_log_EMsg_Copy,
	axl_log_EMsg_CopyString,
	axl_log_EMsg_SaveAsTextFile,

	axl_log_EMsg_HitTest,
};

// log ctrl notifications

enum
{
	axl_log_ENm_First = (0U-2000U),
	axl_log_ENm_CursorPosChange,
};

//.............................................................................

bool_t
AXL_API
axl_log_RegisterLogCtrlWndClass();

size_t
AXL_API
axl_log_FormatTimestamp(
	axl_rtl_TString* pString,
	const tchar_t* pFormat,
	SYSTEMTIME* pTime
	);

//.............................................................................

AXL_EXTERN_C_END

#ifdef __cplusplus

namespace axl {
namespace log {

} // namespace log {
} // namespace axl {

#endif

//.............................................................................

