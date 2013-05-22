// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "LogCtrl_msg.h"

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
	static LPCTSTR GetWndClassName ()
		{ return WLogCtrl; }

	static BOOL RegisterWndClass ()
		{ return RegisterLogCtrlWndClass (); }

	HWND Create (
		HWND hWndParent, 
		ATL::_U_RECT Rect = NULL, 
		DWORD Style = 0, 
		DWORD StyleEx = 0,
		ATL::_U_MENUorID MenuOrID = 0U, 
		LPVOID pCreateParam = NULL
		)
	{ 
		ATLASSERT (!m_hWnd);
		return TBase::Create (GetWndClassName (), hWndParent, Rect.m_lpRect, NULL, Style, StyleEx, MenuOrID.m_hMenu, pCreateParam); 
	}

	TPacketFile* GetPacketFile ()
	{ 
		ATLASSERT (::IsWindow (m_hWnd));
		return (TPacketFile*) ::SendPacket (m_hWnd, EMsg_GetPacketFile, 0, 0);
	}

	BOOL SetPacketFile (TPacketFile* pFile, TRepresentor* pRepresentor)
	{ 
		ATLASSERT (::IsWindow (m_hWnd));

		TMsgSetPacketFile Msg;
		Msg.m_pFile = pFile;
		Msg.m_pRepresentor = pRepresentor;

		return (BOOL) ::SendPacket (m_hWnd, EMsg_SetPacketFile, 0, (LPARAM) &Msg);
	}

	TRepresentor* GetRepresentor ()
	{ 
		ATLASSERT (::IsWindow (m_hWnd));
		return (TRepresentor*) ::SendPacket (m_hWnd, EMsg_GetRepresentor, 0, 0);
	}

	BOOL SetRepresentor (TRepresentor* pRepresentor)
	{ 
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_SetRepresentor, 0, (LPARAM) pRepresentor);
	}

	TFilter* GetFilter ()
	{ 
		ATLASSERT (::IsWindow (m_hWnd));
		return (TFilter*) ::SendPacket (m_hWnd, EMsg_GetFilter, 0, 0);
	}

	BOOL SetFilter (TFilter* pFilter)
	{ 
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_SetFilter, 0, (LPARAM) pFilter);
	}

	TColorizer* GetColorizer ()
	{ 
		ATLASSERT (::IsWindow (m_hWnd));
		return (TColorizer*) ::SendPacket (m_hWnd, EMsg_GetColorizer, 0, 0);
	}

	BOOL SetColorizer (TColorizer* pColorizer)
	{ 
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_SetColorizer, 0, (LPARAM) pColorizer);
	}

	THyperlinkHandler* GetHyperlinkHandler ()
	{ 
		ATLASSERT (::IsWindow (m_hWnd));
		return (THyperlinkHandler*) ::SendPacket (m_hWnd, EMsg_GetHyperlinkHandler, 0, 0);
	}

	BOOL SetHyperlinkHandler (THyperlinkHandler* pHyperlinkHandler)
	{ 
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_SetHyperlinkHandler, 0, (LPARAM) pHyperlinkHandler);
	}

	HIMAGELIST GetImageList ()
	{ 
		ATLASSERT (::IsWindow (m_hWnd));
		return (HIMAGELIST) ::SendPacket (m_hWnd, EMsg_GetImageList, 0, 0);
	}

	BOOL SetImageList (HIMAGELIST hImageList)
	{ 
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_SetImageList, 0, (LPARAM) hImageList);
	}

	BOOL IsIconVisible ()
	{ 
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_IsIconVisible, 0, 0);
	}

	BOOL ShowIcon (BOOL IsVisible)
	{ 
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_ShowIcon, (WPARAM) IsVisible, 0);
	}

	int GetOffsetWidth ()
	{ 
		ATLASSERT (::IsWindow (m_hWnd));
		return (int) ::SendPacket (m_hWnd, EMsg_GetOffsetWidth, 0, 0);
	}

	BOOL SetOffsetWidth (int Width)
	{ 
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_SetOffsetWidth, (WPARAM) Width, 0);
	}

	BOOL IsTimestampVisible ()
	{ 
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_IsTimestampVisible, 0, 0);
	}

	BOOL ShowTimestamp (BOOL IsVisible)
	{ 
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_ShowTimestamp, (WPARAM) IsVisible, 0);
	}

	const TCHAR* GetTimestampFormat ()
	{ 
		ATLASSERT (::IsWindow (m_hWnd));
		return (const TCHAR*) ::SendPacket (m_hWnd, EMsg_GetTimestampFormat, 0, 0);
	}

	BOOL SetTimestampFormat (const TCHAR* pFormat)
	{ 
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_SetTimestampFormat, 0, (LPARAM) pFormat);
	}

	const TBinDataConfig* GetDefBinDataConfig ()
	{ 
		ATLASSERT (::IsWindow (m_hWnd));
		return (const TBinDataConfig*) ::SendPacket (m_hWnd, EMsg_GetDefBinDataConfig, 0, 0);
	}

	BOOL SetDefBinDataConfig (const TBinDataConfig* pDataConfig)
	{ 
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_SetDefBinDataConfig, 0, (LPARAM) pDataConfig);
	}

	BOOL UpdateLog ()
	{ 
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_UpdateLog, 0, 0); 
	}

	BOOL ReRepresentAll ()
	{ 
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_ReRepresentAll, 0, 0); 
	}

	BOOL ReFilterAll ()
	{ 
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_ReFilterAll, 0, 0); 
	}

	BOOL ReColorizeAll ()
	{ 
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_ReColorizeAll, 0, 0); 
	}

	BOOL ModifyLineVolatileFlags (CLine* pLine, ulong_t Flags)
	{ 
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_ModifyLineVolatileFlags, (WPARAM) Flags, (LPARAM) pLine); 
	}

	ULONG GetLineCount ()
	{
		ATLASSERT (::IsWindow (m_hWnd));
		return (ULONG) ::SendPacket (m_hWnd, EMsg_GetLineCount, 0, 0); 
	}

	CLine* GetLine (ULONG i)
	{
		ATLASSERT (::IsWindow (m_hWnd));
		return (CLine*) ::SendPacket (m_hWnd, EMsg_GetLine, (WPARAM) i, 0); 
	}

	CLine* GetNextLine (CLine* pLine)
	{
		ATLASSERT (::IsWindow (m_hWnd));
		return (CLine*) ::SendPacket (m_hWnd, EMsg_GetNextLine, 0, (LPARAM) pLine); 
	}

	CLine* GetPrevLine (CLine* pLine)
	{
		ATLASSERT (::IsWindow (m_hWnd));
		return (CLine*) ::SendPacket (m_hWnd, EMsg_GetPrevLine, 0, (LPARAM) pLine); 
	}

	// { return ::SendPacket (m_hWnd, EMsg_GetLineString, 0, 0); }

	BOOL GetCursorPos (TCursorPos* pCursorPos)
	{
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_GetCursorPos, 0, (LPARAM) pCursorPos); 
	}

	BOOL SetCursorPos (const TCursorPos* pCursorPos)
	{
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_SetCursorPos, 0, (LPARAM) pCursorPos); 
	}

	BOOL GetSelection (TCursorPos* pSelStart, TCursorPos* pSelEnd)
	{
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_GetSelection, (WPARAM) pSelStart, (LPARAM) pSelEnd); 
	}

	BOOL IsSelectionEmpty ()
	{
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_IsSelectionEmpty, 0, 0); 
	}

	BOOL SetSelection (const TCursorPos* pSelStart, const TCursorPos* pSelEnd)
	{
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_SetSelection, (WPARAM) pSelStart, (LPARAM) pSelEnd); 
	}

	// { return ::SendPacket (m_hWnd, EMsg_GetHilite, 0, 0); }
	// { return ::SendPacket (m_hWnd, EMsg_SetHilite, 0, 0); }
	// { return ::SendPacket (m_hWnd, EMsg_SelectBinRange, 0, 0); }
	// { return ::SendPacket (m_hWnd, EMsg_HiliteBinRange, 0, 0); }
	// { return ::SendPacket (m_hWnd, EMsg_EnsureVisible, 0, 0); }
	// { return ::SendPacket (m_hWnd, EMsg_EnsureVisibleRange, 0, 0); }

	// { return ::SendPacket (m_hWnd, EMsg_GetRangeBinBlock, 0, 0); }
	// { return ::SendPacket (m_hWnd, EMsg_ProcessRange, 0, 0); }
	// { return ::SendPacket (m_hWnd, EMsg_ProcessRangeAsBinBlock, 0, 0); }

	BOOL GetSelectionBinBlock (size_t* pOffset, size_t* pSize)
	{
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_GetSelectionBinBlock, (WPARAM) pOffset, (LPARAM) pSize); 
	}

	ULONG ProcessSelection (FProcessLine pfProcess, void* pContext)
	{
		ATLASSERT (::IsWindow (m_hWnd));
		return (ULONG) ::SendPacket (m_hWnd, EMsg_ProcessSelection, (WPARAM) pfProcess, (LPARAM) pContext); 
	}

	ULONG ProcessSelectionAsBinBlock (FProcessBinBlock pfProcess, void* pContext)
	{
		ATLASSERT (::IsWindow (m_hWnd));
		return (ULONG) ::SendPacket (m_hWnd, EMsg_ProcessSelectionAsBinBlock, (WPARAM) pfProcess, (LPARAM) pContext); 
	}

	BOOL SetFindPattern (const void* p, size_t Size, BOOL DoMatchCase)
	{
		ATLASSERT (::IsWindow (m_hWnd));

		TMsgSetFindPattern SetPattern;
		SetPattern.m_p= p;
		SetPattern.m_Size = Size;
		SetPattern.m_DoMatchCase = DoMatchCase;

		return (BOOL) ::SendPacket (m_hWnd, EMsg_SetFindPattern, 0, (LPARAM) &SetPattern); 
	}

	BOOL CanFind ()
	{
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_CanFind, 0, 0); 
	}

	BOOL FindNext ()
	{
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_FindNext, 0, 0); 
	}

	BOOL FindPrev ()
	{
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_FindPrev, 0, 0); 
	}

	BOOL CanCopy ()
	{
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_CanCopy, 0, 0); 
	}

	BOOL Copy ()
	{
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_Copy, 0, 0); 
	}

	SIZE_T CopyString (axl_rtl_TString* pString)
	{
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_CopyString, 0, (LPARAM) pString); 
	}

	axl::rtl::CString CopyString ()
	{
		ATLASSERT (::IsWindow (m_hWnd));
		axl::rtl::CString String;
		::SendPacket (m_hWnd, EMsg_CopyString, 0, (LPARAM) &String); 
		return String;
	}

	BOOL SaveAsTextFile (const TCHAR* pFileName, axl_rtl_TBinTree* pIconMap)
	{
		ATLASSERT (::IsWindow (m_hWnd));
		return (BOOL) ::SendPacket (m_hWnd, EMsg_SaveAsTextFile, (WPARAM) pIconMap, (LPARAM) pFileName); 
	}

	// { return ::SendPacket (m_hWnd, EMsg_HitTest, 0, 0); }

};

typedef CLogCtrlT<ATL::CWindow> CLogCtrl;

} // namespace log {
} // namespace axl {

//.............................................................................
