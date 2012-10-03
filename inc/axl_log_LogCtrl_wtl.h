// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_log_LogCtrl_msg.h"

#ifndef __cplusplus
#error C++ compiler required
#endif

//.............................................................................

namespace axl {
namespace log {

template <class TBase>
class CLogCtrlT: public TBase
{
public:
	static LPCTSTR GetWndClassName()
		{ return axl_log_WLogCtrl; }

	static BOOL RegisterWndClass()
		{ return axl_log_RegisterLogCtrlWndClass(); }

	HWND Create(
		HWND hWndParent, 
		ATL::_U_RECT Rect = NULL, 
		DWORD Style = 0, 
		DWORD StyleEx = 0,
		ATL::_U_MENUorID MenuOrID = 0U, 
		LPVOID pCreateParam = NULL
		)
	{ 
		ATLASSERT(!m_hWnd);
		return TBase::Create(GetWndClassName(), hWndParent, Rect.m_lpRect, NULL, Style, StyleEx, MenuOrID.m_hMenu, pCreateParam); 
	}

	axl_log_TMessageFile* GetMessageFile()
	{ 
		ATLASSERT(::IsWindow(m_hWnd));
		return (axl_log_TMessageFile*) ::SendMessage(m_hWnd, axl_log_EMsg_GetMessageFile, 0, 0);
	}

	BOOL SetMessageFile(axl_log_TMessageFile* pFile, axl_log_TRepresentor* pRepresentor)
	{ 
		ATLASSERT(::IsWindow(m_hWnd));

		axl_log_TMsgSetMessageFile Msg;
		Msg.m_pFile = pFile;
		Msg.m_pRepresentor = pRepresentor;

		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_SetMessageFile, 0, (LPARAM) &Msg);
	}

	axl_log_TRepresentor* GetRepresentor()
	{ 
		ATLASSERT(::IsWindow(m_hWnd));
		return (axl_log_TRepresentor*) ::SendMessage(m_hWnd, axl_log_EMsg_GetRepresentor, 0, 0);
	}

	BOOL SetRepresentor(axl_log_TRepresentor* pRepresentor)
	{ 
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_SetRepresentor, 0, (LPARAM) pRepresentor);
	}

	axl_log_TFilter* GetFilter()
	{ 
		ATLASSERT(::IsWindow(m_hWnd));
		return (axl_log_TFilter*) ::SendMessage(m_hWnd, axl_log_EMsg_GetFilter, 0, 0);
	}

	BOOL SetFilter(axl_log_TFilter* pFilter)
	{ 
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_SetFilter, 0, (LPARAM) pFilter);
	}

	axl_log_TColorizer* GetColorizer()
	{ 
		ATLASSERT(::IsWindow(m_hWnd));
		return (axl_log_TColorizer*) ::SendMessage(m_hWnd, axl_log_EMsg_GetColorizer, 0, 0);
	}

	BOOL SetColorizer(axl_log_TColorizer* pColorizer)
	{ 
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_SetColorizer, 0, (LPARAM) pColorizer);
	}

	axl_log_THyperlinkHandler* GetHyperlinkHandler()
	{ 
		ATLASSERT(::IsWindow(m_hWnd));
		return (axl_log_THyperlinkHandler*) ::SendMessage(m_hWnd, axl_log_EMsg_GetHyperlinkHandler, 0, 0);
	}

	BOOL SetHyperlinkHandler(axl_log_THyperlinkHandler* pHyperlinkHandler)
	{ 
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_SetHyperlinkHandler, 0, (LPARAM) pHyperlinkHandler);
	}

	HIMAGELIST GetImageList()
	{ 
		ATLASSERT(::IsWindow(m_hWnd));
		return (HIMAGELIST) ::SendMessage(m_hWnd, axl_log_EMsg_GetImageList, 0, 0);
	}

	BOOL SetImageList(HIMAGELIST hImageList)
	{ 
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_SetImageList, 0, (LPARAM) hImageList);
	}

	BOOL IsIconVisible()
	{ 
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_IsIconVisible, 0, 0);
	}

	BOOL ShowIcon(BOOL IsVisible)
	{ 
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_ShowIcon, (WPARAM) IsVisible, 0);
	}

	int GetOffsetWidth()
	{ 
		ATLASSERT(::IsWindow(m_hWnd));
		return (int) ::SendMessage(m_hWnd, axl_log_EMsg_GetOffsetWidth, 0, 0);
	}

	BOOL SetOffsetWidth(int Width)
	{ 
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_SetOffsetWidth, (WPARAM) Width, 0);
	}

	BOOL IsTimestampVisible()
	{ 
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_IsTimestampVisible, 0, 0);
	}

	BOOL ShowTimestamp(BOOL IsVisible)
	{ 
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_ShowTimestamp, (WPARAM) IsVisible, 0);
	}

	const TCHAR* GetTimestampFormat()
	{ 
		ATLASSERT(::IsWindow(m_hWnd));
		return (const TCHAR*) ::SendMessage(m_hWnd, axl_log_EMsg_GetTimestampFormat, 0, 0);
	}

	BOOL SetTimestampFormat(const TCHAR* pFormat)
	{ 
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_SetTimestampFormat, 0, (LPARAM) pFormat);
	}

	const axl_log_TDataConfig* GetPreferredDataConfig()
	{ 
		ATLASSERT(::IsWindow(m_hWnd));
		return (const axl_log_TDataConfig*) ::SendMessage(m_hWnd, axl_log_EMsg_GetPreferredDataConfig, 0, 0);
	}

	BOOL SetPreferredDataConfig(const axl_log_TDataConfig* pDataConfig)
	{ 
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_SetPreferredDataConfig, 0, (LPARAM) pDataConfig);
	}

	BOOL UpdateLog()
	{ 
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_UpdateLog, 0, 0); 
	}

	BOOL ReRepresentAll()
	{ 
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_ReRepresentAll, 0, 0); 
	}

	BOOL ReFilterAll()
	{ 
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_ReFilterAll, 0, 0); 
	}

	BOOL ReColorizeAll()
	{ 
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_ReColorizeAll, 0, 0); 
	}

	BOOL ModifyLineVolatileFlags(axl_log_TLine* pLine, ulong_t Flags)
	{ 
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_ModifyLineVolatileFlags, (WPARAM) Flags, (LPARAM) pLine); 
	}

	ULONG GetLineCount()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (ULONG) ::SendMessage(m_hWnd, axl_log_EMsg_GetLineCount, 0, 0); 
	}

	axl_log_TLine* GetLine(ULONG i)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (axl_log_TLine*) ::SendMessage(m_hWnd, axl_log_EMsg_GetLine, (WPARAM) i, 0); 
	}

	axl_log_TLine* GetNextLine(axl_log_TLine* pLine)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (axl_log_TLine*) ::SendMessage(m_hWnd, axl_log_EMsg_GetNextLine, 0, (LPARAM) pLine); 
	}

	axl_log_TLine* GetPrevLine(axl_log_TLine* pLine)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (axl_log_TLine*) ::SendMessage(m_hWnd, axl_log_EMsg_GetPrevLine, 0, (LPARAM) pLine); 
	}

	// { return ::SendMessage(m_hWnd, axl_log_EMsg_GetLineString, 0, 0); }

	BOOL GetCursorPos(axl_log_TCursorPos* pCursorPos)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_GetCursorPos, 0, (LPARAM) pCursorPos); 
	}

	BOOL SetCursorPos(const axl_log_TCursorPos* pCursorPos)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_SetCursorPos, 0, (LPARAM) pCursorPos); 
	}

	BOOL GetSelection(axl_log_TCursorPos* pSelStart, axl_log_TCursorPos* pSelEnd)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_GetSelection, (WPARAM) pSelStart, (LPARAM) pSelEnd); 
	}

	BOOL IsSelectionEmpty()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_IsSelectionEmpty, 0, 0); 
	}

	BOOL SetSelection(const axl_log_TCursorPos* pSelStart, const axl_log_TCursorPos* pSelEnd)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_SetSelection, (WPARAM) pSelStart, (LPARAM) pSelEnd); 
	}

	// { return ::SendMessage(m_hWnd, axl_log_EMsg_GetHilite, 0, 0); }
	// { return ::SendMessage(m_hWnd, axl_log_EMsg_SetHilite, 0, 0); }
	// { return ::SendMessage(m_hWnd, axl_log_EMsg_SelectBinRange, 0, 0); }
	// { return ::SendMessage(m_hWnd, axl_log_EMsg_HiliteBinRange, 0, 0); }
	// { return ::SendMessage(m_hWnd, axl_log_EMsg_EnsureVisible, 0, 0); }
	// { return ::SendMessage(m_hWnd, axl_log_EMsg_EnsureVisibleRange, 0, 0); }

	// { return ::SendMessage(m_hWnd, axl_log_EMsg_GetRangeBinBlock, 0, 0); }
	// { return ::SendMessage(m_hWnd, axl_log_EMsg_ProcessRange, 0, 0); }
	// { return ::SendMessage(m_hWnd, axl_log_EMsg_ProcessRangeAsBinBlock, 0, 0); }

	BOOL GetSelectionBinBlock(size_t* pOffset, size_t* pSize)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_GetSelectionBinBlock, (WPARAM) pOffset, (LPARAM) pSize); 
	}

	ULONG ProcessSelection(axl_log_FProcessLine pfnProcess, void* pContext)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (ULONG) ::SendMessage(m_hWnd, axl_log_EMsg_ProcessSelection, (WPARAM) pfnProcess, (LPARAM) pContext); 
	}

	ULONG ProcessSelectionAsBinBlock(axl_log_FProcessBinBlock pfnProcess, void* pContext)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (ULONG) ::SendMessage(m_hWnd, axl_log_EMsg_ProcessSelectionAsBinBlock, (WPARAM) pfnProcess, (LPARAM) pContext); 
	}

	BOOL SetFindPattern(const void* p, size_t Size, BOOL DoMatchCase)
	{
		ATLASSERT(::IsWindow(m_hWnd));

		axl_log_TMsgSetFindPattern SetPattern;
		SetPattern.m_p= p;
		SetPattern.m_Size = Size;
		SetPattern.m_DoMatchCase = DoMatchCase;

		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_SetFindPattern, 0, (LPARAM) &SetPattern); 
	}

	BOOL CanFind()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_CanFind, 0, 0); 
	}

	BOOL FindNext()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_FindNext, 0, 0); 
	}

	BOOL FindPrev()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_FindPrev, 0, 0); 
	}

	BOOL CanCopy()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_CanCopy, 0, 0); 
	}

	BOOL Copy()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_Copy, 0, 0); 
	}

	SIZE_T CopyString(axl_rtl_TString* pString)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_CopyString, 0, (LPARAM) pString); 
	}

	axl::rtl::CString CopyString()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		axl::rtl::CString String;
		::SendMessage(m_hWnd, axl_log_EMsg_CopyString, 0, (LPARAM) &String); 
		return String;
	}

	BOOL SaveAsTextFile(const TCHAR* pFileName, axl_rtl_TBinTree* pIconMap)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL) ::SendMessage(m_hWnd, axl_log_EMsg_SaveAsTextFile, (WPARAM) pIconMap, (LPARAM) pFileName); 
	}

	// { return ::SendMessage(m_hWnd, axl_log_EMsg_HitTest, 0, 0); }

};

typedef CLogCtrlT<ATL::CWindow> CLogCtrl;

} // namespace log {
} // namespace axl {

//.............................................................................