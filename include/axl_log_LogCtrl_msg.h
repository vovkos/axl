// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "CursorPos.h"
#include "PacketFile.h"
#include "Representor.h"
#include "Filter.h"
#include "Colorizer.h"
#include "HyperlinkHandler.h"

AXL_EXTERN_C_BEGIN

#define WLogCtrl _T("ctrl")

//.............................................................................

typedef struct _TMsgSetPacketFile TMsgSetPacketFile;

struct _TMsgSetPacketFile
{
	TPacketFile* m_pFile;
	TRepresentor* m_pRepresentor;
};

typedef struct _TMsgSetFindPattern TMsgSetFindPattern;

struct _TMsgSetFindPattern
{
	const void* m_p;
	size_t m_Size;
	bool_t m_DoMatchCase;
};

typedef 
void
(AXL_API*
 FProcessLine)(
	void* pContext,
	CLine* pLine,
	CLine* pPrevLine,
	long StartCol, // if StartCol == -1 then it is NOT the first line in range
	long EndCol    // if EndCol == -1 then it is NOT the last line in range
	);


typedef 
void 
(AXL_API*
 FProcessBinBlock)(
	void* pContext,
	const void* p,
	size_t Size
	);

//.............................................................................

// log ctrl packets

enum
{
	EMsg_First = (WM_USER + 1000),

	EMsg_GetPacketFile,
	EMsg_SetPacketFile,
	EMsg_GetRepresentor,
	EMsg_SetRepresentor,
	EMsg_GetFilter,
	EMsg_SetFilter,
	EMsg_GetColorizer,
	EMsg_SetColorizer,
	EMsg_GetHyperlinkHandler,
	EMsg_SetHyperlinkHandler,

	EMsg_GetImageList,
	EMsg_SetImageList,
	EMsg_IsIconVisible,
	EMsg_ShowIcon,
	EMsg_GetOffsetWidth,
	EMsg_SetOffsetWidth,
	EMsg_IsTimestampVisible,
	EMsg_ShowTimestamp,
	EMsg_GetTimestampFormat,
	EMsg_SetTimestampFormat,
	EMsg_GetDefBinDataConfig,
	EMsg_SetDefBinDataConfig,

	EMsg_UpdateLog,
	EMsg_ReRepresentAll,
	EMsg_ReFilterAll,
	EMsg_ReColorizeAll,
	EMsg_ModifyLineVolatileFlags,

	EMsg_GetLineCount,
	EMsg_GetLine,
	EMsg_GetNextLine,
	EMsg_GetPrevLine,
	EMsg_GetLineString,

	EMsg_GetCursorPos,
	EMsg_SetCursorPos,
	EMsg_GetSelection,
	EMsg_SetSelection,
	EMsg_IsSelectionEmpty,
	EMsg_GetHilite,
	EMsg_SetHilite,
	EMsg_SelectBinRange,
	EMsg_HiliteBinRange,
	EMsg_EnsureVisible,
	EMsg_EnsureVisibleRange,

	EMsg_GetRangeBinBlock,
	EMsg_ProcessRange,
	EMsg_ProcessRangeAsBinBlock,

	EMsg_GetSelectionBinBlock,
	EMsg_ProcessSelection,
	EMsg_ProcessSelectionAsBinBlock,

	EMsg_SetFindPattern,
	EMsg_CanFind,
	EMsg_FindNext,
	EMsg_FindPrev,

	EMsg_CanCopy,
	EMsg_Copy,
	EMsg_CopyString,
	EMsg_SaveAsTextFile,

	EMsg_HitTest,
};

// log ctrl notifications

enum
{
	ENm_First = (0U-2000U),
	ENm_CursorPosChange,
};

//.............................................................................

bool_t
RegisterLogCtrlWndClass ();

size_t
FormatTimestamp (
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

