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
		BYTE m_location;
		
		CURSOR_POS()
		{
			m_nOffset = 0;
			m_location = CURSOR_ASCII_0;
		}

		BOOL isAscii()
			{ return m_location >= CURSOR_ASCII_0; }

		BOOL isHex()
			{ return !isAscii(); }
	};

	enum COLOR_FLAGS
	{
		CF_FORE_COLOR = 1,
		CF_BACK_COLOR = 2,
	};

	struct COLOR_INFO
	{
		int m_flags;
		COLORREF m_rgbBackColor;
		COLORREF m_rgbForeColor;

		COLOR_INFO()
		{
			m_flags = 0;
			m_rgbBackColor = 0;
			m_rgbForeColor = 0;
		}

		BOOL operator == (const COLOR_INFO& src)
			{ return m_flags == src.m_flags && m_rgbBackColor == src.m_rgbBackColor && m_rgbForeColor == src.m_rgbForeColor; }

		BOOL operator != (const COLOR_INFO& src)
			{ return !operator == (src); }

		COLOR_INFO& operator += (const COLOR_INFO& src)
		{ 
			m_flags |= src.m_flags;

			if (src.m_flags & CF_BACK_COLOR)
				m_rgbBackColor = src.m_rgbBackColor;

			if (src.m_flags & CF_FORE_COLOR)
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
		COLOR_INFO m_color;
		BYTE m_data;
		BYTE m_flags; // data flags and color flags
	};

	struct CURSOR_TRANSITION
	{
		int m_nOffsetDelta;
		int m_newLocation;
	};

	struct SELECTION_INFO
	{
		SIZE_T m_nStartOffset;
		SIZE_T m_nEndOffset;
		BOOL m_bLeadingGap;
		BOOL m_bTrailingGap;

		SELECTION_INFO()
			{ clear(); }

		BOOL isEmpty()
			{ return m_nStartOffset == m_nEndOffset && !m_bLeadingGap && !m_bTrailingGap; }

		void clear()
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
	
	SIZE m_charSize;

	HRGN m_hUpdateRgn;

	HFONT m_hDefaultFont;
	HFONT m_hFont;

	DATA* m_data;
	SIZE_T m_nDataSize;
	SIZE_T m_nAllocSize;
	DATA* m_dataEnd;

	BOOL m_bSelecting;
	CURSOR_POS m_selStart;
	CURSOR_POS m_selEnd;
	SELECTION_INFO m_selection;

	COLOR_INFO* m_stdColor;

	COLOR_INFO m_stdColorBg;
	COLOR_INFO m_stdColorAscii;
	COLOR_INFO m_selectionColor;
	COLOR_INFO m_cursorColor;
	COLOR_INFO m_modifiedColor;

	CURSOR_POS m_cursorPos;

	BYTE m_offsetWidth;
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
		std::vector<DATA> m_oldData;
		std::vector<DATA> m_newData;
		CURSOR_POS m_oldCursorPos;
		CURSOR_POS m_newCursorPos;
	};

#if (_AXL_VER >= 0x0200)
	axl::obj::CDataT<UNDO_PARAMS> m_undoParams;
	axl::rtl::CUndoRedo m_undoRedo;
#else
	axl::CDataT<UNDO_PARAMS> m_undoParams;
	axl::CUndoRedo m_undoRedo;
#endif

	// Boyer-Moore-Horspool find
	std::vector<BYTE> m_findPattern;
	size_t m_findNextBadCharTable[256];
	size_t m_findPrevBadCharTable[256];
	BOOL m_bFindMatchCase;

public:
	CHexEditCtrl();
	~CHexEditCtrl();

	COLOR_INFO& getStdColorBg()
		{ return m_stdColorBg; }

	COLOR_INFO& getStdColorAscii()
		{ return m_stdColorAscii; }

	COLOR_INFO& getSelectionColor()
		{ return m_selectionColor; }

	COLOR_INFO& getCursorColor()
		{ return m_cursorColor; }

	COLOR_INFO& getModifiedColor()
		{ return m_modifiedColor; }

	static ATL::CWndClassInfo& getWndClassInfo();

	BOOL setUpperCase(BOOL bUpperCase);

	BOOL getUpperCase()
		{ return m_bUpperCase; }

	void setOffsetWidth(BYTE width);

	void showOnlyPrintableChars(BOOL bEnable);

	void setReadOnly(BOOL bReadOnly);

	BOOL isReadOnly()
		{ return m_bReadOnly; }

	SIZE_T getDataSize()		
		{ return m_nDataSize; }

	BOOL setData(PVOID pvData, SIZE_T nDataSize, SIZE_T nOffset = 0);
	SIZE_T getData(PVOID pvBuffer, SIZE_T nBufferSize, SIZE_T nOffset = 0);

	SIZE_T getSelectedDataSize()		
		{ return m_selection.isEmpty() ? 0 : m_selection.m_nEndOffset - m_selection.m_nStartOffset; }

	SIZE_T getSelectedData(PVOID pvBuffer, SIZE_T nBufferSize);
	
	BOOL setDataSize(SIZE_T nDataSize);

	BOOL setFont(HFONT hFont);
	
	HFONT getFont()
		{ return m_hFont; }

	BOOL setBytesPerLine(int nBytesPerLine);

	int autoAdjustBytesPerLine();

	SIZE_T getBytesPerLine()
		{ return m_nBytesPerLine; }
	
	CURSOR_POS setCursorPos(CURSOR_POS pos, int flags = 0);

	CURSOR_POS getCursorPos()
		{ return m_cursorPos; }

	void selectRange(SIZE_T nOffset, SIZE_T nSize, int flags = 0);

	CURSOR_POS cursorPosFromMousePos(POINT pt);

	void ensureVisible(CURSOR_POS pos);

	BOOL setOverwriteMode(BOOL bOverwriteMode);

	BOOL getOverwriteMode()
		{ return m_bOverwriteMode; }

	BOOL isModified()
		{ return m_bModified; }

	void clearModified();

	BOOL replaceSelection(PVOID pvData, SIZE_T nSize, BOOL bCursorToEnd = TRUE);

	BOOL isEqualCursorPos(CURSOR_POS& pos1, CURSOR_POS& pos2);

	void selectAll();

	BOOL canCopy()
		{ return m_selection.m_nStartOffset != m_selection.m_nEndOffset; }

	BOOL canCut()
		{ return m_selection.m_nStartOffset != m_selection.m_nEndOffset; }

	BOOL canPaste()
		{ return isClipboardFormatAvailable(CF_TEXT); }

	void copy();
	void cut();
	void paste();

	BOOL canUndo()
		{ return m_undoRedo.canUndo(); }

	BOOL canRedo()
		{ return m_undoRedo.canRedo(); }

	void undo()
		{ m_undoRedo.undo(); }

	void redo()
		{ m_undoRedo.redo(); }

	void setFindPattern(PVOID p, SIZE_T nSize, BOOL bMatchCase = FALSE);

	BOOL canFindNextPrev()
		{ return !m_findPattern.empty(); }

	BOOL findNext();
	BOOL findPrev();

protected:
	static BOOL isMatch(DATA* data, BYTE* pattern, SIZE_T nSize, BOOL bMatchCase);

	LRESULT notifyParent(UINT nCode, HEN_PARAMS* params = NULL);

	void setModified();

	CHAR getHexChar(BYTE data);

	void setBytesPerLineImpl(int nBytesPerLine);
	void recalcLayout();
	void recalcColumns();
	void recalcScrollBars();
	void fixupFirstVisibleLineCol();

	BOOL insertDeleteData(SIZE_T nOffset, SIZE_T nInsert, SIZE_T nDelete);
	
	BOOL replaceData(SIZE_T nOffset, SIZE_T nDelete, DATA* data, SIZE_T nSize, BOOL bCursorToEnd);

	void paintRect(HDC hdc, RECT& rctUpdate);
	void paintOffs(HDC hdc, RECT rct, int nLineCount, SIZE_T nOffset);
	void paintHex(HDC hdc, RECT rct, int nColCount, int nLineCount, SIZE_T nOffset);
	void paintGap(HDC hdc, RECT rct, int nLineCount);
	void paintAscii(HDC hdc, RECT rct, int nColCount, int nLineCount, SIZE_T nOffset);

	COLOR_INFO getDataColor(DATA* data);
	void applyColor(HDC hdc, COLOR_INFO* color, int prevColorFlags);

	void getCursorLineCol(CURSOR_POS pos, int& nLine, int& nCol, BOOL bAddExtra = TRUE);

	void updateCaret();
	void updateCaretPos();
	
	void transitCursor(CURSOR_TRANSITION* transition, int flags = 0)
		{ transitCursor(transition->m_nOffsetDelta, transition->m_newLocation, flags); }

	void transitCursor(int nOffsetDelta, int newLocation, int flags = 0);

	void invalidateData(DATA* data)
		{ invalidateOffset(data - m_data); }

	void invalidateRange(SIZE_T nOffsetFrom, SIZE_T nOffsetTo);
	void invalidateOffset(SIZE_T nOffset);
	void invalidateSelectionDiff(SELECTION_INFO* sel, SELECTION_INFO* old);
	void invalidateSelection(SELECTION_INFO* sel);
	void invalidateHelper(int nLine, int nCol, int nLineCount, int nColCount);

	void modifyData(DATA* data, BYTE data, BOOL b4Bits = FALSE);

	BOOL isDataSelected(SIZE_T nOffset);
	BOOL isGapSelected(SIZE_T nOffset);

	void setSelEnd(CURSOR_POS pos);

	void killSelection();

	void finalize4Bits();

protected:
	void beginUndoableTransaction(SIZE_T nOffset, SIZE_T nOldDataSize);
	void endUndoableTransaction(SIZE_T nNewDataSize);

#if (_AXL_VER >= 0x0200)
	void undoImpl(axl::obj::TData* params);
	void redoImpl(axl::obj::TData* params);
#else
	void undoImpl(AXL_PTR* params);
	void redoImpl(AXL_PTR* params);
#endif

public:
	BEGIN_MSG_MAP(CHexEditCtrl)
		MESSAGE_HANDLER(WM_CREATE, onCreate)
		MESSAGE_HANDLER(WM_GETDLGCODE, onGetDlgCode)
		MESSAGE_HANDLER(WM_PAINT, onPaint)
		MESSAGE_HANDLER(WM_SETFOCUS, onSetFocus)
		MESSAGE_HANDLER(WM_KILLFOCUS, onKillFocus)
		MESSAGE_HANDLER(WM_SIZE, onSize)
		MESSAGE_HANDLER(WM_HSCROLL, onScroll)
		MESSAGE_HANDLER(WM_VSCROLL, onScroll)
		MESSAGE_HANDLER(WM_MOUSEWHEEL, onMouseWheel)
		MESSAGE_HANDLER(WM_MOUSEMOVE, onMouseMove)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, onLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, onLButtonUp)
		MESSAGE_HANDLER(WM_CAPTURECHANGED, onCaptureChanged)
		MESSAGE_HANDLER(WM_KEYDOWN, onKeyDown)
		MESSAGE_HANDLER(WM_CHAR, onChar)
		MESSAGE_HANDLER(WM_SYSKEYDOWN, onSysKeyDown)
	END_MSG_MAP()

protected:
	LRESULT onCreate(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT onGetDlgCode(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT onPaint(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT onSetFocus(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT onKillFocus(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT onSize(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT onScroll(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT onMouseWheel(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT onMouseMove(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT onCaptureChanged(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT onLButtonDown(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT onLButtonUp(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT onKeyDown(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT onChar(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT onSysKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	void onKeyTab();
	void onKeyLeft(BOOL bShift, BOOL bCtrl);
	void onKeyRight(BOOL bShift, BOOL bCtrl);
	void onKeyUp(BOOL bShift, BOOL bCtrl);
	void onKeyDown(BOOL bShift, BOOL bCtrl);
	void onKeyPageUp(BOOL bShift, BOOL bCtrl);
	void onKeyPageDown(BOOL bShift, BOOL bCtrl);
	void onKeyHome(BOOL bShift, BOOL bCtrl);
	void onKeyEnd(BOOL bShift, BOOL bCtrl);

	void onKeyDelete();
	void onKeyBackspace();

	void onChar(CHAR ch);
};

