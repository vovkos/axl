#pragma once

#include "axl_rtl_UndoRedo.h"

//.............................................................................

enum HEX_EDIT_NOTIFICATIONS
{
	HEN_BASE = 100,

	HEN_DATA_CHANGE,
	HEN_SELECTION_CHANGE,
	HEN_OVERWRITE_CHANGE,
};

struct HEN_PARAMS: NMHDR
{
};

struct HEN_SELECTION_CHANGE_PARAMS: HEN_PARAMS
{
	SIZE_T m_nOffsetStart;
	SIZE_T m_nOffsetEnd;
};

struct HEN_OVERWRITE_CHANGE_PARAMS: HEN_PARAMS
{
	BOOL m_bOverwrite;
};

//.............................................................................

class CHexEditCtrl: public CWindowImpl<CHexEditCtrl>
{
public:
	enum CURSOR_POS_LOCATION
	{
		CURSOR_HEX_0 = 0,
		CURSOR_HEX_1,
		CURSOR_HEX_2,
		CURSOR_ASCII_0,
		CURSOR_ASCII_1,

		CURSOR_LOCATION_COUNT,
	};

	struct CURSOR_POS
	{
		LONG m_nOffset;
		BYTE m_Location;
		
		CURSOR_POS()
		{
			m_nOffset = 0;
			m_Location = CURSOR_ASCII_0;
		}

		BOOL IsAscii()
			{ return m_Location >= CURSOR_ASCII_0; }

		BOOL IsHex()
			{ return !IsAscii(); }
	};

	enum COLOR_FLAGS
	{
		CF_FORE_COLOR = 1,
		CF_BACK_COLOR = 2,
	};

	struct COLOR_INFO
	{
		int m_Flags;
		COLORREF m_rgbBackColor;
		COLORREF m_rgbForeColor;

		COLOR_INFO()
		{
			m_Flags = 0;
			m_rgbBackColor = 0;
			m_rgbForeColor = 0;
		}

		BOOL operator == (const COLOR_INFO& src)
			{ return m_Flags == src.m_Flags && m_rgbBackColor == src.m_rgbBackColor && m_rgbForeColor == src.m_rgbForeColor; }

		BOOL operator != (const COLOR_INFO& src)
			{ return !operator == (src); }

		COLOR_INFO& operator += (const COLOR_INFO& src)
		{ 
			m_Flags |= src.m_Flags;

			if (src.m_Flags & CF_BACK_COLOR)
				m_rgbBackColor = src.m_rgbBackColor;

			if (src.m_Flags & CF_FORE_COLOR)
				m_rgbForeColor = src.m_rgbForeColor;

			return *this;
		}
	};

	enum MOVE_CURSOR_FLAGS
	{
		MCF_SELECT = 1,
		MCF_KEEP_SELECTION = 2,
		MCF_FORCE = 4
	};

	enum SELECT_RANGE_FLAGS
	{
		SRF_CURSOR_TO_BEGIN = 1,
		SRF_CURSOR_TO_HEX = 2,
	};

protected:
	enum DATA_FLAGS
	{
		DF_READ_ONLY = 8,
		DF_4_BITS    = 16, // data is being edited in HEX pane and is partially deleted
		DF_MODIFIED  = 32,
	};

	enum 
	{
		BUFFER_PAGE_SIZE = 16, // 16 data blocks
	};

	struct DATA
	{
		PVOID m_pvUserContext;
		COLOR_INFO m_Color;
		BYTE m_Data;
		BYTE m_Flags; // data flags and color flags
	};

	struct CURSOR_TRANSITION
	{
		int m_nOffsetDelta;
		int m_NewLocation;
	};

	struct SELECTION_INFO
	{
		SIZE_T m_nStartOffset;
		SIZE_T m_nEndOffset;
		BOOL m_bLeadingGap;
		BOOL m_bTrailingGap;

		SELECTION_INFO()
			{ Clear(); }

		BOOL IsEmpty()
			{ return m_nStartOffset == m_nEndOffset && !m_bLeadingGap && !m_bTrailingGap; }

		void Clear()
			{ m_nStartOffset = m_nEndOffset = 0, m_bLeadingGap = m_bTrailingGap = FALSE; }
	};

protected:
	int m_nLineCount;
	int m_nFirstVisibleLine;
	int m_nFirstVisibleCol;
	int m_nBytesPerLine;
	int m_nVisibleLineCount;
	int m_nVisibleColCount;

	BOOL m_bAutoAdjustBytesPerLine;
	
	int m_nHexGapStep;

	int m_nHexCol;
	int m_nGapCol;
	int m_nAsciiCol;
	int m_nSpaceCol;

	int m_nMouseWheelSpeed;
	
	SIZE m_CharSize;

	HRGN m_hUpdateRgn;

	HFONT m_hDefaultFont;
	HFONT m_hFont;

	DATA* m_pData;
	SIZE_T m_nDataSize;
	SIZE_T m_nAllocSize;
	DATA* m_pDataEnd;

	BOOL m_bSelecting;
	CURSOR_POS m_SelStart;
	CURSOR_POS m_SelEnd;
	SELECTION_INFO m_Selection;

	COLOR_INFO* m_pStdColor;

	COLOR_INFO m_StdColorBg;
	COLOR_INFO m_StdColorAscii;
	COLOR_INFO m_SelectionColor;
	COLOR_INFO m_CursorColor;
	COLOR_INFO m_ModifiedColor;

	CURSOR_POS m_CursorPos;

	BYTE m_OffsetWidth;
	BOOL m_bOnlyPrintableChars;
	BOOL m_bReadOnly;
	BOOL m_bOverwriteMode;
	BOOL m_bModified;
	BOOL m_bDragging;
	BOOL m_bUpperCase;

	BOOL m_bIgnoreTab;
	BOOL m_bIgnoreBackspace;

	struct UNDO_PARAMS
	{
		SIZE_T m_nOffset;
		std::vector<DATA> m_OldData;
		std::vector<DATA> m_NewData;
		CURSOR_POS m_OldCursorPos;
		CURSOR_POS m_NewCursorPos;
	};

#if (_AXL_VER >= 0x0200)
	axl::obj::CDataT<UNDO_PARAMS> m_UndoParams;
	axl::rtl::CUndoRedo m_UndoRedo;
#else
	axl::CDataT<UNDO_PARAMS> m_UndoParams;
	axl::CUndoRedo m_UndoRedo;
#endif

	// Boyer-Moore-Horspool find
	std::vector<BYTE> m_FindPattern;
	size_t m_FindNextBadCharTable[256];
	size_t m_FindPrevBadCharTable[256];
	BOOL m_bFindMatchCase;

public:
	CHexEditCtrl();
	~CHexEditCtrl();

	COLOR_INFO& GetStdColorBg()
		{ return m_StdColorBg; }

	COLOR_INFO& GetStdColorAscii()
		{ return m_StdColorAscii; }

	COLOR_INFO& GetSelectionColor()
		{ return m_SelectionColor; }

	COLOR_INFO& GetCursorColor()
		{ return m_CursorColor; }

	COLOR_INFO& GetModifiedColor()
		{ return m_ModifiedColor; }

	static ATL::CWndClassInfo& GetWndClassInfo();

	BOOL SetUpperCase(BOOL bUpperCase);

	BOOL GetUpperCase()
		{ return m_bUpperCase; }

	void SetOffsetWidth(BYTE Width);

	void ShowOnlyPrintableChars(BOOL bEnable);

	void SetReadOnly(BOOL bReadOnly);

	BOOL IsReadOnly()
		{ return m_bReadOnly; }

	SIZE_T GetDataSize()		
		{ return m_nDataSize; }

	BOOL SetData(PVOID pvData, SIZE_T nDataSize, SIZE_T nOffset = 0);
	SIZE_T GetData(PVOID pvBuffer, SIZE_T nBufferSize, SIZE_T nOffset = 0);

	SIZE_T GetSelectedDataSize()		
		{ return m_Selection.IsEmpty() ? 0 : m_Selection.m_nEndOffset - m_Selection.m_nStartOffset; }

	SIZE_T GetSelectedData(PVOID pvBuffer, SIZE_T nBufferSize);
	
	BOOL SetDataSize(SIZE_T nDataSize);

	BOOL SetFont(HFONT hFont);
	
	HFONT GetFont()
		{ return m_hFont; }

	BOOL SetBytesPerLine(int nBytesPerLine);

	int AutoAdjustBytesPerLine();

	SIZE_T GetBytesPerLine()
		{ return m_nBytesPerLine; }
	
	CURSOR_POS SetCursorPos(CURSOR_POS Pos, int Flags = 0);

	CURSOR_POS GetCursorPos()
		{ return m_CursorPos; }

	void SelectRange(SIZE_T nOffset, SIZE_T nSize, int Flags = 0);

	CURSOR_POS CursorPosFromMousePos(POINT pt);

	void EnsureVisible(CURSOR_POS Pos);

	BOOL SetOverwriteMode(BOOL bOverwriteMode);

	BOOL GetOverwriteMode()
		{ return m_bOverwriteMode; }

	BOOL IsModified()
		{ return m_bModified; }

	void ClearModified();

	BOOL ReplaceSelection(PVOID pvData, SIZE_T nSize, BOOL bCursorToEnd = TRUE);

	BOOL IsEqualCursorPos(CURSOR_POS& Pos1, CURSOR_POS& Pos2);

	void SelectAll();

	BOOL CanCopy()
		{ return m_Selection.m_nStartOffset != m_Selection.m_nEndOffset; }

	BOOL CanCut()
		{ return m_Selection.m_nStartOffset != m_Selection.m_nEndOffset; }

	BOOL CanPaste()
		{ return IsClipboardFormatAvailable(CF_TEXT); }

	void Copy();
	void Cut();
	void Paste();

	BOOL CanUndo()
		{ return m_UndoRedo.CanUndo(); }

	BOOL CanRedo()
		{ return m_UndoRedo.CanRedo(); }

	void Undo()
		{ m_UndoRedo.Undo(); }

	void Redo()
		{ m_UndoRedo.Redo(); }

	void SetFindPattern(PVOID p, SIZE_T nSize, BOOL bMatchCase = FALSE);

	BOOL CanFindNextPrev()
		{ return !m_FindPattern.empty(); }

	BOOL FindNext();
	BOOL FindPrev();

protected:
	static BOOL IsMatch(DATA* pData, BYTE* pPattern, SIZE_T nSize, BOOL bMatchCase);

	LRESULT NotifyParent(UINT nCode, HEN_PARAMS* pParams = NULL);

	void SetModified();

	CHAR GetHexChar(BYTE Data);

	void SetBytesPerLineImpl(int nBytesPerLine);
	void RecalcLayout();
	void RecalcColumns();
	void RecalcScrollBars();
	void FixupFirstVisibleLineCol();

	BOOL InsertDeleteData(SIZE_T nOffset, SIZE_T nInsert, SIZE_T nDelete);
	
	BOOL ReplaceData(SIZE_T nOffset, SIZE_T nDelete, DATA* pData, SIZE_T nSize, BOOL bCursorToEnd);

	void PaintRect(HDC hdc, RECT& rctUpdate);
	void PaintOffs(HDC hdc, RECT rct, int nLineCount, SIZE_T nOffset);
	void PaintHex(HDC hdc, RECT rct, int nColCount, int nLineCount, SIZE_T nOffset);
	void PaintGap(HDC hdc, RECT rct, int nLineCount);
	void PaintAscii(HDC hdc, RECT rct, int nColCount, int nLineCount, SIZE_T nOffset);

	COLOR_INFO GetDataColor(DATA* pData);
	void ApplyColor(HDC hdc, COLOR_INFO* pColor, int PrevColorFlags);

	void GetCursorLineCol(CURSOR_POS Pos, int& nLine, int& nCol, BOOL bAddExtra = TRUE);

	void UpdateCaret();
	void UpdateCaretPos();
	
	void TransitCursor(CURSOR_TRANSITION* pTransition, int Flags = 0)
		{ TransitCursor(pTransition->m_nOffsetDelta, pTransition->m_NewLocation, Flags); }

	void TransitCursor(int nOffsetDelta, int NewLocation, int Flags = 0);

	void InvalidateData(DATA* pData)
		{ InvalidateOffset(pData - m_pData); }

	void InvalidateRange(SIZE_T nOffsetFrom, SIZE_T nOffsetTo);
	void InvalidateOffset(SIZE_T nOffset);
	void InvalidateSelectionDiff(SELECTION_INFO* pSel, SELECTION_INFO* pOld);
	void InvalidateSelection(SELECTION_INFO* pSel);
	void InvalidateHelper(int nLine, int nCol, int nLineCount, int nColCount);

	void ModifyData(DATA* pData, BYTE Data, BOOL b4Bits = FALSE);

	BOOL IsDataSelected(SIZE_T nOffset);
	BOOL IsGapSelected(SIZE_T nOffset);

	void SetSelEnd(CURSOR_POS Pos);

	void KillSelection();

	void Finalize4Bits();

protected:
	void BeginUndoableTransaction(SIZE_T nOffset, SIZE_T nOldDataSize);
	void EndUndoableTransaction(SIZE_T nNewDataSize);

#if (_AXL_VER >= 0x0200)
	void UndoImpl(axl::obj::TData* pParams);
	void RedoImpl(axl::obj::TData* pParams);
#else
	void UndoImpl(AXL_PTR* pParams);
	void RedoImpl(AXL_PTR* pParams);
#endif

public:
	BEGIN_MSG_MAP(CHexEditCtrl)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
		MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_HSCROLL, OnScroll)
		MESSAGE_HANDLER(WM_VSCROLL, OnScroll)
		MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_CAPTURECHANGED, OnCaptureChanged)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_CHAR, OnChar)
		MESSAGE_HANDLER(WM_SYSKEYDOWN, OnSysKeyDown)
	END_MSG_MAP()

protected:
	LRESULT OnCreate(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetDlgCode(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnPaint(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSetFocus(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKillFocus(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnScroll(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseWheel(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCaptureChanged(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonDown(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUp(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDown(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnChar(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSysKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	void OnKeyTab();
	void OnKeyLeft(BOOL bShift, BOOL bCtrl);
	void OnKeyRight(BOOL bShift, BOOL bCtrl);
	void OnKeyUp(BOOL bShift, BOOL bCtrl);
	void OnKeyDown(BOOL bShift, BOOL bCtrl);
	void OnKeyPageUp(BOOL bShift, BOOL bCtrl);
	void OnKeyPageDown(BOOL bShift, BOOL bCtrl);
	void OnKeyHome(BOOL bShift, BOOL bCtrl);
	void OnKeyEnd(BOOL bShift, BOOL bCtrl);

	void OnKeyDelete();
	void OnKeyBackspace();

	void OnChar(CHAR ch);
};

