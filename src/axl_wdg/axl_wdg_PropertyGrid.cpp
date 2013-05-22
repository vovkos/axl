#include "axl_Precomp.h"
#include "axl_win_PropertyCtrl.h"

enum
{
	axl_win_EPropertyTimer_MenuDelay = 1,
};

//.............................................................................

void
AXL_API
axl_win_TPropertyInPlace_Construct(
	axl_win_TPropertyInPlace* pInPlace,
	axl_obj_TAgent* pAgent
	)
{
	axl_obj_TObject_Construct(pInPlace, pAgent);
	pInPlace->m_pfLoad = NULL;
	pInPlace->m_pfSave = NULL;
	pInPlace->m_pfLayout = NULL;
	pInPlace->m_pfSetFocus = NULL;
}

//.............................................................................

// forwards

LRESULT 
CALLBACK 
axl_win_TPropertyCtrl_WndProc(
	HWND hWnd,
	UINT Msg,
	WPARAM wParam,
	LPARAM lParam
	);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
AXL_API
axl_win_TPropertyCtrl_CalcCenteredRect(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	SIZE* pSize,
	long Width,
	RECT* pRect
	)
{
	pRect->left = (Width - pSize->cx) / 2;
	pRect->top = (pPropertyCtrl->m_LineHeight - pSize->cy) / 2;
	pRect->right = pRect->left + pSize->cx;
	pRect->bottom = pRect->top + pSize->cy;
}

bool_t
AXL_API
axl_win_TPropertyCtrl_CalcLineHeight(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	ulong_t Height = max(pPropertyCtrl->m_CharSize.cy, pPropertyCtrl->m_IconSize.cy);
	ulong_t Mod;

	Height += pPropertyCtrl->m_Margins.top + pPropertyCtrl->m_Margins.bottom;	
	Mod = Height % 4;

	Height = Mod ? Height + 4 - Mod : Height;

	if (pPropertyCtrl->m_LineHeight == Height)
		return false;

	pPropertyCtrl->m_LineHeight = Height;
	pPropertyCtrl->m_TextOrigin.x = pPropertyCtrl->m_Margins.left;
	pPropertyCtrl->m_TextOrigin.y = (pPropertyCtrl->m_LineHeight - pPropertyCtrl->m_CharSize.cy) / 2;

	axl_win_TPropertyCtrl_CalcCenteredRect(pPropertyCtrl, &pPropertyCtrl->m_PlusMinusSize, pPropertyCtrl->m_IndentWidth, &pPropertyCtrl->m_PlusMinusRect);
	axl_win_TPropertyCtrl_CalcCenteredRect(pPropertyCtrl, &pPropertyCtrl->m_OptionBoxSize, pPropertyCtrl->m_OptionBoxWidth, &pPropertyCtrl->m_OptionBoxRect);
	axl_win_TPropertyCtrl_CalcCenteredRect(pPropertyCtrl, &pPropertyCtrl->m_IconSize, pPropertyCtrl->m_IconWidth, &pPropertyCtrl->m_IconRect);

	// v-gridline is at odd x, but indent is even, therefore we must shift plusminus rect if we want it to match v-gridline

	pPropertyCtrl->m_PlusMinusRect.left++;
	pPropertyCtrl->m_PlusMinusRect.right++;

	return true;
}

void
AXL_API
axl_win_TPropertyCtrl_Construct(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	pPropertyCtrl->m_hWnd = NULL;
	pPropertyCtrl->m_hWndToolTip = NULL;
//	pPropertyCtrl->m_hUpdateRgn = CreateRectRgn(0, 0, 0, 0);
	pPropertyCtrl->m_hWndNotify = NULL;
	pPropertyCtrl->m_IdNotify = 0;

	pPropertyCtrl->m_ToolTipShowDelay = 1000;  // 1 sec initial
	pPropertyCtrl->m_ToolTipReShowDelay = 200; // 200 msec reshow

	pPropertyCtrl->m_pMenuPropertyCtrl = NULL;
	pPropertyCtrl->m_pMenuParentPropertyCtrl = NULL;
	pPropertyCtrl->m_pMenuRootPropertyCtrl = NULL;
	pPropertyCtrl->m_pMenuAnchorLine = NULL;
	pPropertyCtrl->m_MenuDelay = 500;

	pPropertyCtrl->m_hImageList = NULL;
	pPropertyCtrl->m_hArrowCursor = LoadCursor(NULL, IDC_ARROW);
	pPropertyCtrl->m_hSplitterCursor = LoadCursor(NULL, IDC_SIZEWE);
	pPropertyCtrl->m_hReArrangeCursor = LoadCursor(NULL, IDC_SIZENS);
	pPropertyCtrl->m_hHyperlinkCursor = LoadCursor(NULL, IDC_HAND);	

	pPropertyCtrl->m_SelectedLine = -1;
	pPropertyCtrl->m_FirstVisibleLine = 0;
	pPropertyCtrl->m_FirstVisibleX = 0;
	pPropertyCtrl->m_VisibleWidth = 0;
	pPropertyCtrl->m_FullyVisibleLineCount = 0;
	pPropertyCtrl->m_VisibleLineCount = 0;
	pPropertyCtrl->m_SplitterPos = 200;
	pPropertyCtrl->m_MinNamePaneWidth = 50;

	pPropertyCtrl->m_Drag = axl_win_EPropertyDrag_None;
	pPropertyCtrl->m_DragAnchorHitTest = axl_win_EPropertyHitTest_None;
	pPropertyCtrl->m_pDragAnchorLine = NULL;
	pPropertyCtrl->m_DragDelta.x = 0;
	pPropertyCtrl->m_DragDelta.y = 0;

	pPropertyCtrl->m_Hover = axl_win_EPropertyHover_None;
	pPropertyCtrl->m_pHoverAnchorLine = NULL;
	pPropertyCtrl->m_pLastTooltipLine = NULL;

	pPropertyCtrl->m_Margins.left = 5;
	pPropertyCtrl->m_Margins.top = 5;
	pPropertyCtrl->m_Margins.right = 5;
	pPropertyCtrl->m_Margins.bottom = 5;

	pPropertyCtrl->m_PlusMinusSize.cx = 9;
	pPropertyCtrl->m_PlusMinusSize.cy = 9;
	pPropertyCtrl->m_OptionBoxSize.cx = 13;
	pPropertyCtrl->m_OptionBoxSize.cy = 13;
	pPropertyCtrl->m_OptionBoxWidth = pPropertyCtrl->m_OptionBoxSize.cx + pPropertyCtrl->m_Margins.left + pPropertyCtrl->m_Margins.right;

	pPropertyCtrl->m_IconSize.cx = 0;
	pPropertyCtrl->m_IconSize.cy = 0;
	pPropertyCtrl->m_IconWidth = 0;
	memset(&pPropertyCtrl->m_IconRect, 0, sizeof(RECT));

	pPropertyCtrl->m_SplitterProximityRect.left = -2;
	pPropertyCtrl->m_SplitterProximityRect.top = -2;
	pPropertyCtrl->m_SplitterProximityRect.right = 2;
	pPropertyCtrl->m_SplitterProximityRect.bottom = 2;

	pPropertyCtrl->m_IndentWidth = pPropertyCtrl->m_OptionBoxWidth & ~1;  // indent width must be even
	pPropertyCtrl->m_HasGridLinesAtRoot = true;
	pPropertyCtrl->m_DoAlwaysDrawButtons = false;

	axl_gr_TFont_Construct(&pPropertyCtrl->m_DefaultFont);
	axl_gr_TFont_Attach(&pPropertyCtrl->m_DefaultFont, GetStockObject(DEFAULT_GUI_FONT));
	
	pPropertyCtrl->m_CharSize = axl_gr_TFont_CalcCharSize(&pPropertyCtrl->m_DefaultFont, '|');

	axl_win_TPropertyCtrl_CalcLineHeight(pPropertyCtrl);

	pPropertyCtrl->m_SinglePaneWidth = 0;
	pPropertyCtrl->m_ValueWidth = 0;

	pPropertyCtrl->m_MouseWheelSpeed = 3;

	axl_rtl_TList_Construct(&pPropertyCtrl->m_LineList);
	axl_rtl_TPtrArray_Construct(&pPropertyCtrl->m_VisibleLineArray);

	pPropertyCtrl->m_pLineList = &pPropertyCtrl->m_LineList;
	pPropertyCtrl->m_pActiveInPlace = NULL;
	pPropertyCtrl->m_ActiveInPlaceMinWidth = 0;

	memcpy(pPropertyCtrl->m_Palette, g_axl_gr_StdPalette, sizeof(g_axl_gr_StdPalette));
	pPropertyCtrl->m_Palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_Name)]            = GetSysColor(COLOR_WINDOWTEXT);
	pPropertyCtrl->m_Palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_NameBack)]        = GetSysColor(COLOR_BTNFACE);
	pPropertyCtrl->m_Palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_DisabledText)]    = GetSysColor(COLOR_GRAYTEXT);	
	pPropertyCtrl->m_Palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_Value)]           = RGB(0,0,255);
	pPropertyCtrl->m_Palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_ValueBack)]       = GetSysColor(COLOR_BTNFACE);
	pPropertyCtrl->m_Palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_SelectedName)]    = GetSysColor(COLOR_HIGHLIGHTTEXT);
	pPropertyCtrl->m_Palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_SelectedValue)]   = RGB(255, 255, 0);
	pPropertyCtrl->m_Palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_SelectedBack)]    = GetSysColor(COLOR_HIGHLIGHT);
	pPropertyCtrl->m_Palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_ButtonText)]      = GetSysColor(COLOR_WINDOWTEXT);
	pPropertyCtrl->m_Palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_HoverButtonText)] = GetSysColor(COLOR_WINDOWTEXT);
	pPropertyCtrl->m_Palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_Empty)]           = GetSysColor(COLOR_WINDOW);
	pPropertyCtrl->m_Palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_NameDelimiter)]   = GetSysColor(COLOR_BTNSHADOW);
	pPropertyCtrl->m_Palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_ValueDelimiter)]  = GetSysColor(COLOR_BTNSHADOW);
	pPropertyCtrl->m_Palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_Splitter)]        = GetSysColor(COLOR_BTNSHADOW);
	pPropertyCtrl->m_Palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_Grid)]            = GetSysColor(COLOR_BTNSHADOW);
	pPropertyCtrl->m_Palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_PlusMinus)]       = RGB(0, 0, 0);
	pPropertyCtrl->m_Palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_PlusMinusRect)]   = RGB(128, 128, 128);
	pPropertyCtrl->m_Palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_PlusMinusBack)]   = RGB(255, 255, 255);
	pPropertyCtrl->m_Palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_Unused)]          = RGB(255, 0, 255);

#ifdef _AXL_USE_OFFSCREEN_BUFFER
	axl_gr_TOffscreenBuffer_Construct(&pPropertyCtrl->m_OffscreenBuffer);
#endif

	axl_gr_TStockCtrlPaint_Construct(&pPropertyCtrl->m_StockCtrlPaint);
	axl_gr_TTextPaint_Construct(&pPropertyCtrl->m_TextPaint);
	axl_gr_TFontMod_SetBaseFont(&pPropertyCtrl->m_TextPaint.m_Font, pPropertyCtrl->m_DefaultFont.m_hFont);

	pPropertyCtrl->m_TextPaint.m_pPalette = pPropertyCtrl->m_Palette;
	pPropertyCtrl->m_TextPaint.m_PaletteSize = axl_win_EPropertyColor_Count;

	pPropertyCtrl->m_hPlusMinusImageList = axl_win_TPropertyCtrl_CreatePlusMinusImageList(pPropertyCtrl->m_PlusMinusSize.cx, pPropertyCtrl->m_PlusMinusSize.cy, pPropertyCtrl->m_Palette);
	pPropertyCtrl->m_hGridImageList = axl_win_TPropertyCtrl_CreateGridImageList(pPropertyCtrl->m_IndentWidth, pPropertyCtrl->m_LineHeight, pPropertyCtrl->m_Palette);
	pPropertyCtrl->m_hMenuArrowImageList = axl_win_TPropertyCtrl_CreateMenuArrowImageList();
}

void
AXL_API
axl_win_TPropertyCtrl_Destruct(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	axl_win_TPropertyLine_ClearLineList(&pPropertyCtrl->m_LineList);
	axl_rtl_TPtrArray_Destruct(&pPropertyCtrl->m_VisibleLineArray);
	axl_gr_TFont_Destruct(&pPropertyCtrl->m_DefaultFont);

//	DeleteObject(pPropertyCtrl->m_hUpdateRgn);

	if (pPropertyCtrl->m_pMenuPropertyCtrl)
	{
		axl_win_TPropertyCtrl_Destruct(pPropertyCtrl->m_pMenuPropertyCtrl);
		axl_mem_Free(pPropertyCtrl->m_pMenuPropertyCtrl);
	}

#ifdef _AXL_USE_OFFSCREEN_BUFFER
	axl_gr_TOffscreenBuffer_Destruct(&pPropertyCtrl->m_OffscreenBuffer);
#endif

	axl_gr_TStockCtrlPaint_Destruct(&pPropertyCtrl->m_StockCtrlPaint);
	axl_gr_TTextPaint_Destruct(&pPropertyCtrl->m_TextPaint);

	ImageList_Destroy(pPropertyCtrl->m_hPlusMinusImageList);
	ImageList_Destroy(pPropertyCtrl->m_hGridImageList);
	ImageList_Destroy(pPropertyCtrl->m_hMenuArrowImageList);
}

LRESULT
AXL_API
axl_win_TPropertyCtrl_Notify(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	int Code,
	axl_win_TPropertyHitTest* pHitTest
	)
{
	axl_win_TPropertyNotify Notify;
	
	pPropertyCtrl = axl_win_TPropertyCtrl_GetMenuRootPropertyCtrl(pPropertyCtrl);

	Notify.m_Hdr.code = Code;
	Notify.m_Hdr.hwndFrom = pPropertyCtrl->m_hWndNotify;
	Notify.m_Hdr.idFrom = pPropertyCtrl->m_hWndNotify ? pPropertyCtrl->m_IdNotify : GetDlgCtrlID(pPropertyCtrl->m_hWnd);
	Notify.m_HitTest = *pHitTest;	

	return pPropertyCtrl->m_hWndNotify ?
		SendMessage(pPropertyCtrl->m_hWndNotify, WM_NOTIFY, Notify.m_Hdr.idFrom, (LPARAM) &Notify):
		SendMessage(GetParent(pPropertyCtrl->m_hWnd), WM_NOTIFY, Notify.m_Hdr.idFrom, (LPARAM) &Notify);
}


void 
AXL_API
axl_win_TPropertyCtrl_SetImageList(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	HIMAGELIST hImageList
	)
{
	bool_t HasChanged;

	if (!hImageList)
	{
		pPropertyCtrl->m_IconSize.cx = 0;
		pPropertyCtrl->m_IconSize.cy = 0;
		pPropertyCtrl->m_IconWidth = 0;
		memset(&pPropertyCtrl->m_IconRect, 0, sizeof(RECT));
	}
	else
	{
		int cx, cy;
		ImageList_GetIconSize(hImageList, &cx, &cy);

		pPropertyCtrl->m_IconSize.cx = cx;
		pPropertyCtrl->m_IconSize.cy = cy;
		pPropertyCtrl->m_IconWidth = cx + pPropertyCtrl->m_Margins.left + pPropertyCtrl->m_Margins.right;
		axl_win_TPropertyCtrl_CalcCenteredRect(pPropertyCtrl, &pPropertyCtrl->m_IconSize, pPropertyCtrl->m_IconWidth, &pPropertyCtrl->m_IconRect);
	}

	pPropertyCtrl->m_hImageList = hImageList;

	HasChanged = axl_win_TPropertyCtrl_CalcLineHeight(pPropertyCtrl);
	
	if (HasChanged)
	{
		ImageList_Destroy(pPropertyCtrl->m_hGridImageList);
		pPropertyCtrl->m_hGridImageList = axl_win_TPropertyCtrl_CreateGridImageList(pPropertyCtrl->m_IndentWidth, pPropertyCtrl->m_LineHeight, pPropertyCtrl->m_Palette);
	}

	axl_win_TPropertyCtrl_Invalidate(pPropertyCtrl);
}

bool_t
AXL_API
axl_win_TPropertyCtrl_RegisterWndClass(HINSTANCE hInstance)
{
	BOOL Result;
	WNDCLASS WndClass;

	static bool_t _IsRegistered = false;
	if (_IsRegistered)
		return true;

	memset(&WndClass, 0, sizeof(WndClass));
	WndClass.style = CS_DBLCLKS;
    WndClass.lpfWndProc = axl_win_TPropertyCtrl_WndProc;
    WndClass.cbWndExtra = sizeof(axl_win_TPropertyCtrl*);
    WndClass.hInstance = hInstance;
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.lpszClassName = axl_win_TPropertyCtrl_WndClassName;

	Result = RegisterClass(&WndClass);
	if (!Result)
		return false;

	_IsRegistered = true;
	return true;
}

HWND
AXL_API
axl_win_TPropertyCtrl_CreateEx(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	DWORD ExStyle,
	DWORD Style,
	const RECT* pRect,
	HWND hWndParent,
	UINT ID,
	HINSTANCE hInstance
	)
{
	bool_t Result;
	
	static RECT _NullRect = {0};

	if (!pRect)
		pRect = &_NullRect;

	if (pPropertyCtrl->m_hWnd)
		return NULL;

	Result = axl_win_TPropertyCtrl_RegisterWndClass(hInstance);
	if (!Result)
		return false;

	Style |= WS_CLIPCHILDREN;

	CreateWindowEx(
		ExStyle, 
		axl_win_TPropertyCtrl_WndClassName, NULL, 
		Style, 
		pRect->left, pRect->top, 
		pRect->right - pRect->left, 
		pRect->bottom - pRect->top,
		hWndParent, (HMENU)(uintptr_t) ID,
		hInstance,
		pPropertyCtrl
		);

	return pPropertyCtrl->m_hWnd;
}

void
AXL_API
axl_win_TPropertyCtrl_Destroy(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	if (!pPropertyCtrl->m_hWnd)
		return;

	DestroyWindow(pPropertyCtrl->m_hWnd);
	pPropertyCtrl->m_hWnd = NULL;
}

axl_win_TPropertyLine*
AXL_API
axl_win_TPropertyCtrl_GetSelectedLine(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	if (pPropertyCtrl->m_SelectedLine == -1)
		return NULL;

	ASSERT(pPropertyCtrl->m_SelectedLine < axl_rtl_TPtrArray_GetCount(&pPropertyCtrl->m_VisibleLineArray));

	return ((axl_win_TPropertyLine**) pPropertyCtrl->m_VisibleLineArray.m_p)[pPropertyCtrl->m_SelectedLine]; 
}

axl_win_TPropertyLine* 
AXL_API
axl_win_TPropertyCtrl_HitTest(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	long x,
	long y,	
	axl_win_TPropertyHitTest* pHitTest
	)
{
	long _x;

	axl_win_TPropertyLine* pLine;

	size_t LineCount = axl_rtl_TPtrArray_GetCount(&pPropertyCtrl->m_VisibleLineArray);
	size_t Line = y / pPropertyCtrl->m_LineHeight + pPropertyCtrl->m_FirstVisibleLine;

	long IndentWidth;

	pHitTest->m_HitTest = axl_win_EPropertyHitTest_None;
	pHitTest->m_pLine = NULL; 
	pHitTest->m_pHyperlink = NULL; 

	if (Line >= LineCount)
		return NULL;

	x += pPropertyCtrl->m_FirstVisibleX;
	y %= pPropertyCtrl->m_LineHeight;

	pLine = ((axl_win_TPropertyLine**) pPropertyCtrl->m_VisibleLineArray.m_p)[Line];
	pHitTest->m_pLine = pLine;

	if (!axl_win_TPropertyLine_IsSinglePane(pLine))
	{
		if (Line == pPropertyCtrl->m_SelectedLine && pPropertyCtrl->m_pActiveInPlace)
		{
			// in this case, no proximity from the right

			if (x >= pPropertyCtrl->m_SplitterPos + pPropertyCtrl->m_SplitterProximityRect.left && x <= pPropertyCtrl->m_SplitterPos)
			{
				pHitTest->m_HitTest = axl_win_EPropertyHitTest_Splitter;
				return pLine;
			}
		}
		else
		{
			if (x >= pPropertyCtrl->m_SplitterPos + pPropertyCtrl->m_SplitterProximityRect.left && 
				x <= pPropertyCtrl->m_SplitterPos + pPropertyCtrl->m_SplitterProximityRect.right)
			{
				pHitTest->m_HitTest = axl_win_EPropertyHitTest_Splitter;
				return pLine;
			}
		}

		if (x > pPropertyCtrl->m_SplitterPos)
		{
			_x = x - pPropertyCtrl->m_SplitterPos - pPropertyCtrl->m_Margins.left;

			if (pLine->m_Flags & axl_win_EPropertyLine_ValueHyperText && 
				y >= pPropertyCtrl->m_TextOrigin.y && 
				y < pPropertyCtrl->m_TextOrigin.y + pPropertyCtrl->m_CharSize.cy)
			{
				axl_gr_THyperlinkAnchor* pAnchor = axl_gr_THyperText_FindHyperlinkByX(&pLine->m_ValueHyperText, _x);
				if (pAnchor && !axl_rtl_TString_IsEmpty(&pAnchor->m_Hyperlink))
				{
					pHitTest->m_pHyperlink = pAnchor;
					pHitTest->m_HitTest = axl_win_EPropertyHitTest_Hyperlink;
					return pLine;
				}
			}

			pHitTest->m_HitTest = axl_win_EPropertyHitTest_Value;
			return pLine;
		}
	}

	_x = x;

	if (!axl_win_TPropertyCtrl_IsMenu(pPropertyCtrl))
		_x -= pLine->m_Level * pPropertyCtrl->m_IndentWidth;

	if (axl_win_TPropertyLine_HasPlusMinus(pLine) &&
		_x >= pPropertyCtrl->m_PlusMinusRect.left && _x < pPropertyCtrl->m_PlusMinusRect.right && 
		y >= pPropertyCtrl->m_PlusMinusRect.top && y < pPropertyCtrl->m_PlusMinusRect.bottom)
	{
		pHitTest->m_HitTest = axl_win_EPropertyHitTest_PlusMinus;
		return pLine;
	}

	IndentWidth = (pLine->m_Indent + 1) * pPropertyCtrl->m_IndentWidth;

	if (pLine->m_pParent && (pLine->m_pParent->m_Flags & axl_win_EPropertyLine_ReArrangeable) && 
		_x >= 0 && _x <= IndentWidth &&
		y >= pPropertyCtrl->m_PlusMinusRect.top && y < pPropertyCtrl->m_PlusMinusRect.bottom)
	{
		pHitTest->m_HitTest = axl_win_EPropertyHitTest_ReArrange;
		return pLine;
	}

	_x -= IndentWidth;

	if (pLine->m_OptionBoxType)
	{
		if (_x >= pPropertyCtrl->m_OptionBoxRect.left && _x < (long) pPropertyCtrl->m_OptionBoxWidth &&
			y >= pPropertyCtrl->m_OptionBoxRect.top && y < pPropertyCtrl->m_OptionBoxRect.bottom)
		{
			pHitTest->m_HitTest = axl_win_EPropertyHitTest_OptionBox;
			return pLine;
		}

		_x -= pPropertyCtrl->m_OptionBoxWidth;
	}

	if (_x < 0)
	{
		pHitTest->m_HitTest = axl_win_EPropertyHitTest_Indent;
		return pLine;
	}

	if (pLine->m_Flags & axl_win_EPropertyLine_Menu)
	{
		if (x < (long) pLine->m_MenuWidth + pPropertyCtrl->m_Margins.left + pPropertyCtrl->m_Margins.right + GetSystemMetrics(SM_CXVSCROLL))
		{
			pHitTest->m_HitTest = axl_win_EPropertyHitTest_Menu;
			return pLine;
		}
	}

	if (pLine->m_Flags & axl_win_EPropertyLine_Button)
	{
		if (x < (long) pLine->m_SinglePaneWidth + pPropertyCtrl->m_Margins.left + 2 * pPropertyCtrl->m_Margins.right)
		{
			pHitTest->m_HitTest = axl_win_EPropertyHitTest_Button;
			return pLine;
		}
	}

	if (pLine->m_Icon != -1)
	{
		_x -= (long) pPropertyCtrl->m_IconWidth;
		if (_x < 0)
		{
			pHitTest->m_HitTest = pLine->m_OptionBoxType ? axl_win_EPropertyHitTest_OptionBox : axl_win_EPropertyHitTest_Icon;
			return pLine;
		}
	}

	_x -= pPropertyCtrl->m_Margins.left;

	if (pLine->m_Flags & axl_win_EPropertyLine_NameHyperText && 
		y >= pPropertyCtrl->m_TextOrigin.y && 
		y < pPropertyCtrl->m_TextOrigin.y + pPropertyCtrl->m_CharSize.cy)
	{
		axl_gr_THyperlinkAnchor* pAnchor = axl_gr_THyperText_FindHyperlinkByX(&pLine->m_NameHyperText, _x);
		if (pAnchor && !axl_rtl_TString_IsEmpty(&pAnchor->m_Hyperlink))
		{
			pHitTest->m_pHyperlink = pAnchor;
			pHitTest->m_HitTest = axl_win_EPropertyHitTest_Hyperlink;
			return pLine;
		}
	}

	if (_x < (long) pLine->m_NameWidth)
		pHitTest->m_HitTest = pLine->m_OptionBoxType ? axl_win_EPropertyHitTest_OptionBox : axl_win_EPropertyHitTest_Name;
	else
		pHitTest->m_HitTest = axl_win_EPropertyHitTest_NameTail;

	return pLine;
}

long
AXL_API
axl_win_TPropertyCtrl_GetFullWidth(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	ulong_t ValueWidth = pPropertyCtrl->m_ValueWidth + pPropertyCtrl->m_Margins.left + pPropertyCtrl->m_SplitterPos + 1;
	ulong_t SinglePaneWidth = pPropertyCtrl->m_SinglePaneWidth + pPropertyCtrl->m_Margins.left;
	ulong_t InPlaceMinWidth = pPropertyCtrl->m_pActiveInPlace ? pPropertyCtrl->m_ActiveInPlaceMinWidth + pPropertyCtrl->m_SplitterPos + 1: 0;
	ulong_t Width = max(ValueWidth, max(SinglePaneWidth, InPlaceMinWidth));
	Width += pPropertyCtrl->m_Margins.right;
	return Width;
}

long
AXL_API
axl_win_TPropertyCtrl_GetFullHeight(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	size_t LineCount = axl_rtl_TPtrArray_GetCount(&pPropertyCtrl->m_VisibleLineArray);
	return (long) LineCount * pPropertyCtrl->m_LineHeight;
}

void
AXL_API
axl_win_TPropertyCtrl_GetSize(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	SIZE* pSize
	)
{
	pSize->cx = axl_win_TPropertyCtrl_GetFullWidth(pPropertyCtrl);
	pSize->cy = axl_win_TPropertyCtrl_GetFullHeight(pPropertyCtrl);
}

void
AXL_API
axl_win_TPropertyCtrl_GetInPlaceRect(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	RECT* pRect
	)
{
	ASSERT(pPropertyCtrl->m_SelectedLine != -1);

	GetClientRect(pPropertyCtrl->m_hWnd, pRect);

	pRect->left = pPropertyCtrl->m_SplitterPos - pPropertyCtrl->m_FirstVisibleX + 1;
	pRect->top = (long) (pPropertyCtrl->m_SelectedLine - pPropertyCtrl->m_FirstVisibleLine) * pPropertyCtrl->m_LineHeight;
	pRect->bottom = pRect->top + pPropertyCtrl->m_LineHeight;
	
	pRect->top--;
}

void
AXL_API
axl_win_TPropertyCtrl_GetMenuPos(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	const SIZE* pSize,
	POINT* pPos
	)
{
	POINT ReferencePoint = { 0, 0 };
	SIZE ScreenSize = { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };

	ASSERT(pPropertyCtrl->m_pMenuAnchorLine && pPropertyCtrl->m_pMenuAnchorLine->m_VisibleIndex != -1);

	ClientToScreen(pPropertyCtrl->m_hWnd, &ReferencePoint);

	pPos->x = pPropertyCtrl->m_pMenuAnchorLine->m_MenuWidth + pPropertyCtrl->m_Margins.left + pPropertyCtrl->m_Margins.right;
	pPos->x += GetSystemMetrics(SM_CXVSCROLL);

	if (ReferencePoint.x + pPos->x + pSize->cx > ScreenSize.cx)
		pPos->x = -pSize->cx;

	pPos->y = (long) (pPropertyCtrl->m_pMenuAnchorLine->m_VisibleIndex - pPropertyCtrl->m_FirstVisibleLine) * pPropertyCtrl->m_LineHeight;
	pPos->y -= GetSystemMetrics(SM_CYBORDER);

	if (ReferencePoint.y + pPos->y + pSize->cy > ScreenSize.cy)
	{
		long Delta = ReferencePoint.y + pPos->y + pSize->cy - ScreenSize.cy;
		pPos->y -= Delta;
	}

	ClientToScreen(pPropertyCtrl->m_hWnd, pPos);
}

bool_t
AXL_API
axl_win_TPropertyCtrl_FixupFirstVisibleLine(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	size_t LineCount = axl_rtl_TPtrArray_GetCount(&pPropertyCtrl->m_VisibleLineArray);
	size_t NewFirstVisibleLine = pPropertyCtrl->m_FirstVisibleLine;

	if (pPropertyCtrl->m_FirstVisibleLine + pPropertyCtrl->m_FullyVisibleLineCount > LineCount)
	{
		if (LineCount > pPropertyCtrl->m_FullyVisibleLineCount)
			NewFirstVisibleLine = LineCount - pPropertyCtrl->m_FullyVisibleLineCount;
		else
			NewFirstVisibleLine = 0;
	}

	if (NewFirstVisibleLine == pPropertyCtrl->m_FirstVisibleLine)
		return false;

	pPropertyCtrl->m_FirstVisibleLine = NewFirstVisibleLine;
	return true;
}

bool_t
AXL_API
axl_win_TPropertyCtrl_FixupFirstVisibleX(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	ulong_t NewFirstVisibleX = pPropertyCtrl->m_FirstVisibleX;
	ulong_t Width = axl_win_TPropertyCtrl_GetFullWidth(pPropertyCtrl);

	if (pPropertyCtrl->m_FirstVisibleX + pPropertyCtrl->m_VisibleWidth > Width)
	{
		if (Width > pPropertyCtrl->m_VisibleWidth)
			NewFirstVisibleX = Width - pPropertyCtrl->m_VisibleWidth;
		else
			NewFirstVisibleX = 0;
	}

	if (NewFirstVisibleX == pPropertyCtrl->m_FirstVisibleX)
		return false;

	pPropertyCtrl->m_FirstVisibleX = NewFirstVisibleX;
	return true;	
}

void 
AXL_API
axl_win_TPropertyCtrl_RecalcHScroll(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	SCROLLINFO ScrollInfo = { sizeof(ScrollInfo) };
	ScrollInfo.fMask = SIF_ALL;
	ScrollInfo.nMin  = 0;
	ScrollInfo.nMax  = axl_win_TPropertyCtrl_GetFullWidth(pPropertyCtrl) - 1;
    ScrollInfo.nPage = pPropertyCtrl->m_VisibleWidth;
    ScrollInfo.nPos  = pPropertyCtrl->m_FirstVisibleX;
	SetScrollInfo(pPropertyCtrl->m_hWnd, SB_HORZ, &ScrollInfo, TRUE);
}

void 
AXL_API
axl_win_TPropertyCtrl_RecalcVScroll(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	size_t LineCount = axl_rtl_TPtrArray_GetCount(&pPropertyCtrl->m_VisibleLineArray);

	SCROLLINFO ScrollInfo = { sizeof(ScrollInfo) };
	ScrollInfo.fMask = SIF_ALL;
	ScrollInfo.nMin  = 0;
    ScrollInfo.nMax  = (long) LineCount - 1;
    ScrollInfo.nPage = (long) pPropertyCtrl->m_FullyVisibleLineCount;
	ScrollInfo.nPos  = (long) pPropertyCtrl->m_FirstVisibleLine;
	SetScrollInfo(pPropertyCtrl->m_hWnd, SB_VERT, &ScrollInfo, TRUE);
}

bool_t
AXL_API
axl_win_TPropertyCtrl_CalcWidth(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	int Flags
	)
{
	bool_t HasChanged = false;

	axl_win_TPropertyLine* pLine;

	ulong_t MaxSinglePaneWidth = 0;
	ulong_t MaxValueWidth = 0;

	pLine = (axl_win_TPropertyLine*) axl_rtl_TList_GetHead(pPropertyCtrl->m_pLineList);
	while (pLine)
	{
		ulong_t SinglePaneWidth = 0;
		ulong_t ValueWidth = 0;

		if (Flags & axl_win_EPropertyCalcWidth_Recursive)
			axl_win_TPropertyLine_CalcWidth(pLine, Flags);

		if (pLine->m_Flags & axl_win_EPropertyLine_IsExpanded)
		{
			SinglePaneWidth = max(pLine->m_ChildrenSinglePaneWidth, pLine->m_SinglePaneWidth);
			ValueWidth = max(pLine->m_ChildrenValueWidth, pLine->m_ValueWidth);
		}
		else
		{
			SinglePaneWidth = pLine->m_SinglePaneWidth;
			ValueWidth = pLine->m_ValueWidth;
		}

		if (SinglePaneWidth > MaxSinglePaneWidth)
			MaxSinglePaneWidth = SinglePaneWidth;

		if (ValueWidth > MaxValueWidth)
			MaxValueWidth = ValueWidth;

		pLine = (axl_win_TPropertyLine*) axl_rtl_TList_GetNext(pPropertyCtrl->m_pLineList, (axl_rtl_TListEntry*) pLine);
	}

	if (MaxValueWidth != pPropertyCtrl->m_ValueWidth || MaxSinglePaneWidth != pPropertyCtrl->m_SinglePaneWidth)
		HasChanged = true;

	pPropertyCtrl->m_ValueWidth = MaxValueWidth;
	pPropertyCtrl->m_SinglePaneWidth = MaxSinglePaneWidth;

	return HasChanged;
}

void 
AXL_API
axl_win_TPropertyCtrl_UpdateLineWidth(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	axl_win_TPropertyLine* pLine
	)
{
	bool_t HasChanged = axl_win_TPropertyLine_CalcWidth(pLine, axl_win_EPropertyCalcWidth_Line | axl_win_EPropertyCalcWidth_Parent);
	if (!HasChanged)
		return;

	HasChanged = axl_win_TPropertyCtrl_CalcWidth(pPropertyCtrl, 0);
	if (!HasChanged)
		return;

	axl_win_TPropertyCtrl_RecalcHScroll(pPropertyCtrl);
}

void 
AXL_API
axl_win_TPropertyCtrl_UpdateAllLineWidths(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	axl_win_TPropertyCtrl_CalcWidth(pPropertyCtrl, axl_win_EPropertyCalcWidth_Line | axl_win_EPropertyCalcWidth_Children | axl_win_EPropertyCalcWidth_Recursive);
	axl_win_TPropertyCtrl_CalcWidth(pPropertyCtrl, axl_win_EPropertyCalcWidth_Menu | axl_win_EPropertyCalcWidth_Children | axl_win_EPropertyCalcWidth_Recursive);
	axl_win_TPropertyCtrl_RecalcHScroll(pPropertyCtrl);
}

void
AXL_API
axl_win_TPropertyCtrl_ReArrangeVisibleLines(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	size_t From,
	size_t To
	)
{
	size_t TotalCount = axl_rtl_TPtrArray_GetCount(&pPropertyCtrl->m_VisibleLineArray);

	axl_win_TPropertyLine** ppBase = (axl_win_TPropertyLine**) pPropertyCtrl->m_VisibleLineArray.m_p;
	axl_win_TPropertyLine* pFrom = ppBase[From];
	
	size_t i;
	size_t End;
	axl_win_TPropertyLine** pp;
	
	ASSERT(From < TotalCount);
	ASSERT(To < TotalCount);

	if (From == To)
		return;

	if (From < To)
	{
		memmove(ppBase + From, ppBase + From + 1, (To - From) * sizeof(void*));

		i = From;
		End = To + 1;
	}
	else
	{
		memmove(ppBase + To + 1, ppBase + To, (From - To) * sizeof(void*));

		i = To;
		End = From + 1;
	}

	ppBase[To] = pFrom;

	pp = ppBase + i;
	for (; i < End; i++, pp++)
		(*pp)->m_VisibleIndex = i;
}


axl_win_TPropertyLine**
AXL_API
axl_win_TPropertyCtrl_InsertVisibleLines(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	size_t i,
	size_t Count
	)
{
	axl_win_TPropertyLine** ppAnchor;
	axl_win_TPropertyLine** pp;
	axl_win_TPropertyLine** ppEnd;

	axl_rtl_TPtrArray_Insert(&pPropertyCtrl->m_VisibleLineArray, i, NULL, Count);

	ppAnchor = (axl_win_TPropertyLine**) pPropertyCtrl->m_VisibleLineArray.m_p + i;
	ppEnd = (axl_win_TPropertyLine**) pPropertyCtrl->m_VisibleLineArray.m_p + axl_rtl_TPtrArray_GetCount(&pPropertyCtrl->m_VisibleLineArray);

	pp = ppAnchor + Count;

	for (; pp < ppEnd; pp++)
	{
		axl_win_TPropertyLine* pLine = *pp;
		ASSERT(pLine);
		pLine->m_VisibleIndex += Count;
	}

	return ppAnchor;
}

void
AXL_API
axl_win_TPropertyCtrl_DeleteVisibleLines(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	size_t i,
	size_t Count
	)
{
	axl_win_TPropertyLine** pp;
	axl_win_TPropertyLine** ppEnd;

	// mark deleted lines as invisible

	pp = (axl_win_TPropertyLine**) pPropertyCtrl->m_VisibleLineArray.m_p + i;
    ppEnd = pp + Count;

	for (; pp < ppEnd; pp++)
	{
		axl_win_TPropertyLine* pLine = *pp;
		ASSERT(pLine);
		pLine->m_VisibleIndex = -1;
	}

	// shift remaining visible lines

	ppEnd = (axl_win_TPropertyLine**) pPropertyCtrl->m_VisibleLineArray.m_p + axl_rtl_TPtrArray_GetCount(&pPropertyCtrl->m_VisibleLineArray);

	for (; pp < ppEnd; pp++)
	{
		axl_win_TPropertyLine* pLine = *pp;
		ASSERT(pLine);
		pLine->m_VisibleIndex -= Count;
	}

	axl_rtl_TPtrArray_Remove(&pPropertyCtrl->m_VisibleLineArray, i, Count);
}

axl_win_TPropertyLine*
AXL_API
axl_win_TPropertyCtrl_InsertLineEx(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	axl_win_TPropertyLine* pParent,
	axl_win_TPropertyLine* pBefore,
	axl_win_TPropertyLine* pSource
	)
{
	size_t VisibleIndex = -1;

	axl_win_TPropertyLine* pLine = axl_mem_Allocate(sizeof(axl_win_TPropertyLine));
	axl_win_TPropertyLine_Construct(pLine);

	pLine->m_pPropertyCtrl = pPropertyCtrl;

	if (!pParent)
	{
		axl_rtl_TList_InsertBefore(&pPropertyCtrl->m_LineList, (axl_rtl_TListEntry*) pLine, (axl_rtl_TListEntry*) pBefore);

		VisibleIndex = pBefore ? 
			pBefore->m_VisibleIndex != -1 ? pBefore->m_VisibleIndex : -1 :
			axl_rtl_TPtrArray_GetCount(&pPropertyCtrl->m_VisibleLineArray);
	}
	else
	{
		pLine->m_pParent = pParent;	
		pLine->m_Level = pParent->m_Level + pParent->m_Indent + 1;
		pLine->m_Flags = pParent->m_Flags &	axl_win_EPropertyLine_NoChildGridLines;
		pLine->m_Delimiters = pParent->m_Delimiters;

		axl_win_TPropertyLine_ModifyChildrenHeight(pParent, 1);

		axl_rtl_TList_InsertBefore(&pParent->m_ChildrenList, (axl_rtl_TListEntry*) pLine, (axl_rtl_TListEntry*) pBefore);

		VisibleIndex = 
			pParent->m_VisibleIndex == -1 || !(pParent->m_Flags & axl_win_EPropertyLine_IsExpanded) ? -1 : pBefore ? 
			pBefore->m_VisibleIndex != -1 ? pBefore->m_VisibleIndex : -1 :
			pParent->m_VisibleIndex + pParent->m_ChildrenHeight;
	}

	if (VisibleIndex != -1)
	{
		*axl_win_TPropertyCtrl_InsertVisibleLines(pPropertyCtrl, VisibleIndex, 1) = pLine;
		pLine->m_VisibleIndex = VisibleIndex;

		axl_win_TPropertyCtrl_InvalidateLineRange(pPropertyCtrl, VisibleIndex, -1);		
		axl_win_TPropertyCtrl_RecalcVScroll(pPropertyCtrl);
	}

	if (pSource)
	{
		axl_win_TPropertyLine* pChild;

		pLine->m_Flags = pSource->m_Flags;
		pLine->m_OptionBoxType = pSource->m_OptionBoxType;
		pLine->m_OptionBoxCheckState = pSource->m_OptionBoxCheckState;
		pLine->m_Icon = pSource->m_Icon;
		pLine->m_Indent = pSource->m_Indent;
		axl_obj_TObject_CopyPtr(&pLine->m_pInPlace, pSource->m_pInPlace);
		axl_obj_TData_Copy(&pLine->m_UserData, &pSource->m_UserData);
		pLine->m_NameAttr = pSource->m_NameAttr;
		pLine->m_ValueAttr = pSource->m_ValueAttr;
		axl_win_TPropertyLine_SetName(pLine, pSource->m_Name.m_p);
		axl_win_TPropertyLine_SetValue(pLine, pSource->m_Name.m_p);

		pChild = (axl_win_TPropertyLine*) axl_rtl_TList_GetHead(&pSource->m_ChildrenList);
		while (pChild)
		{
			axl_win_TPropertyCtrl_InsertLineEx(pPropertyCtrl, pLine, NULL, pChild);
			pChild = (axl_win_TPropertyLine*) axl_rtl_TList_GetNext(&pSource->m_ChildrenList, (axl_rtl_TListEntry*) pChild);
		}
	}

	return pLine;	
}

static
bool_t
AXL_API
_IsChildWindow(HWND hWnd, HWND hWndParent)
{
	while (hWnd)
	{
		hWnd = GetParent(hWnd);
		if (hWnd == hWndParent)
			return true;
	}

	return false;
}

void
AXL_API
axl_win_TPropertyCtrl_AdjustInPlacePosition(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	RECT Rect;

	if (!pPropertyCtrl->m_pActiveInPlace)
		return;

	axl_win_TPropertyCtrl_GetInPlaceRect(pPropertyCtrl, &Rect);
	axl_win_TPropertyInPlace_Layout(pPropertyCtrl->m_pActiveInPlace, &Rect);
}

void
AXL_API
axl_win_TPropertyCtrl_Clear(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	axl_win_TPropertyCtrl_KillSelection(pPropertyCtrl);
	axl_win_TPropertyLine_ClearLineList(&pPropertyCtrl->m_LineList);
	axl_rtl_TPtrArray_Clear(&pPropertyCtrl->m_VisibleLineArray);
	axl_win_TPropertyCtrl_CalcWidth(pPropertyCtrl, 0);
	axl_win_TPropertyCtrl_RecalcHScroll(pPropertyCtrl);
	axl_win_TPropertyCtrl_RecalcVScroll(pPropertyCtrl);
	axl_win_TPropertyCtrl_Invalidate(pPropertyCtrl);
}

void
AXL_API
axl_win_TPropertyCtrl_DelayMenu(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	axl_win_TPropertyCtrl* pRootPropertyCtrl = axl_win_TPropertyCtrl_GetMenuRootPropertyCtrl(pPropertyCtrl);
	SetTimer(pRootPropertyCtrl->m_hWnd, axl_win_EPropertyTimer_MenuDelay, pRootPropertyCtrl->m_MenuDelay, NULL);
}

void
AXL_API
axl_win_TPropertyCtrl_SetSelection(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	size_t Line,
	bool_t SetInPlaceFocus
	)
{
	axl_win_TPropertyLine* pLine;

	size_t LineCount = axl_rtl_TPtrArray_GetCount(&pPropertyCtrl->m_VisibleLineArray);
	if (Line >= LineCount)
		Line = -1;

	pLine = Line != -1 ? ((axl_win_TPropertyLine**) pPropertyCtrl->m_VisibleLineArray.m_p)[Line] : NULL; 

	if (Line == pPropertyCtrl->m_SelectedLine)
	{
		// make sure parent is selected -- even if selection didnt change!
		if (pLine && (pLine->m_Flags & axl_win_EPropertyLine_Menu) && pPropertyCtrl->m_pMenuParentPropertyCtrl != pPropertyCtrl->m_pMenuRootPropertyCtrl) 
			axl_win_TPropertyCtrl_SetSelection(pPropertyCtrl->m_pMenuParentPropertyCtrl, pPropertyCtrl->m_pMenuParentPropertyCtrl->m_pMenuAnchorLine->m_VisibleIndex, false);

		return;
	}

	axl_win_TPropertyCtrl_KillSelection(pPropertyCtrl);

	if (!pLine || pLine->m_Flags & (axl_win_EPropertyLine_NotSelectable | axl_win_EPropertyLine_IsDisabled))
	{
		axl_win_TPropertyCtrl_RecalcHScroll(pPropertyCtrl);
		return;
	}

	pPropertyCtrl->m_SelectedLine = Line;
	axl_win_TPropertyCtrl_InvalidateLineRange(pPropertyCtrl, Line, Line);

	if (pLine->m_Flags & axl_win_EPropertyLine_Menu)
	{
		// schedule menu update

		if (!axl_rtl_TList_IsEmpty(&pLine->m_ChildrenList))
			axl_win_TPropertyCtrl_DelayMenu(pPropertyCtrl);

		// make sure parent is selected

		if (pPropertyCtrl->m_pMenuParentPropertyCtrl != pPropertyCtrl->m_pMenuRootPropertyCtrl)
			axl_win_TPropertyCtrl_SetSelection(pPropertyCtrl->m_pMenuParentPropertyCtrl, pPropertyCtrl->m_pMenuParentPropertyCtrl->m_pMenuAnchorLine->m_VisibleIndex, false);

		return;
	}

	if (pLine->m_pInPlace)
	{
		RECT Rect;
		pPropertyCtrl->m_pActiveInPlace = pLine->m_pInPlace;
		pPropertyCtrl->m_ActiveInPlaceMinWidth = 0;

		axl_win_TPropertyCtrl_GetInPlaceRect(pPropertyCtrl, &Rect);
		axl_win_TPropertyInPlace_Load(pLine->m_pInPlace, pLine, &Rect, &pPropertyCtrl->m_ActiveInPlaceMinWidth);

		if (SetInPlaceFocus)
			axl_win_TPropertyInPlace_SetFocus(pLine->m_pInPlace);
	}

	axl_win_TPropertyCtrl_EnsureVisible(pPropertyCtrl, Line);
	axl_win_TPropertyCtrl_RecalcHScroll(pPropertyCtrl);
}

void 
AXL_API
axl_win_TPropertyCtrl_AdvanceSelection(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	int Delta,
	bool_t SetInPlaceFocus
	)
{
	axl_win_TPropertyLine* pLine;
	size_t Line;
	size_t LineCount = axl_rtl_TPtrArray_GetCount(&pPropertyCtrl->m_VisibleLineArray);
	
	if (!LineCount || !Delta) 
		return;

	if (pPropertyCtrl->m_SelectedLine == -1)
	{
		Line = 0;
		Delta = 1;
	}
	else 
	{
		Line = pPropertyCtrl->m_SelectedLine + Delta;
		
		if (Delta < 0)
		{
			if (Line < LineCount)			
				Delta = -1;
			else
				Line = 0, Delta = 1;
		}
		else
		{
			if (Line < LineCount)			
				Delta = 1;
			else
				Line = LineCount - 1, Delta = -1;
		}
	}

	while (Line < LineCount)
	{
		pLine = ((axl_win_TPropertyLine**) pPropertyCtrl->m_VisibleLineArray.m_p)[Line]; 
	
		if (!(pLine->m_Flags & (axl_win_EPropertyLine_NotSelectable | axl_win_EPropertyLine_IsDisabled)))
		{
			axl_win_TPropertyCtrl_SetSelection(pPropertyCtrl, Line, SetInPlaceFocus);
			return;
		}

		Line += Delta;
	}
}

void 
AXL_API
axl_win_TPropertyCtrl_EnsureVisible(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	size_t Line
	)
{
	bool_t IsUpdateNeeded = false;

	if (Line >= pPropertyCtrl->m_FirstVisibleLine + pPropertyCtrl->m_FullyVisibleLineCount)
	{
		pPropertyCtrl->m_FirstVisibleLine = Line - pPropertyCtrl->m_FullyVisibleLineCount + 1;
		IsUpdateNeeded = true;
	}

	if (Line < pPropertyCtrl->m_FirstVisibleLine)
	{
		pPropertyCtrl->m_FirstVisibleLine = Line;
		IsUpdateNeeded = true;
	}

	if (!IsUpdateNeeded)
		return;

	axl_win_TPropertyCtrl_FixupFirstVisibleLine(pPropertyCtrl);
	axl_win_TPropertyCtrl_RecalcVScroll(pPropertyCtrl);
	axl_win_TPropertyCtrl_AdjustInPlacePosition(pPropertyCtrl);
	axl_win_TPropertyCtrl_Invalidate(pPropertyCtrl);
}

bool_t
AXL_API
axl_win_TPropertyCtrl_SaveActiveInPlace(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	axl_win_TPropertyLine* pLine = axl_win_TPropertyCtrl_GetSelectedLine(pPropertyCtrl);
	return pLine && pPropertyCtrl->m_pActiveInPlace ? axl_win_TPropertyInPlace_Save(pPropertyCtrl->m_pActiveInPlace, pLine) : true;
}

void
AXL_API
axl_win_TPropertyCtrl_KillSelection(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	axl_win_TPropertyInPlace* pActiveInPlace;

	axl_win_TPropertyLine* pLine = axl_win_TPropertyCtrl_GetSelectedLine(pPropertyCtrl);

	if (axl_win_TPropertyCtrl_IsMenuActive(pPropertyCtrl))
		axl_win_TPropertyCtrl_DelayMenu(pPropertyCtrl);

	if (!pLine)
		return;

	pLine->m_OptionBoxVolatileState = 0;
	pLine->m_ButtonVolatileState = 0;

	pActiveInPlace = pPropertyCtrl->m_pActiveInPlace;

	axl_win_TPropertyCtrl_InvalidateLineRange(pPropertyCtrl, pPropertyCtrl->m_SelectedLine, pPropertyCtrl->m_SelectedLine);

	pPropertyCtrl->m_SelectedLine = -1;
	pPropertyCtrl->m_pActiveInPlace = NULL;

	if (pActiveInPlace)
	{		
		if (_IsChildWindow(GetFocus(), pPropertyCtrl->m_hWnd))
			SetFocus(pPropertyCtrl->m_hWnd);

		axl_win_TPropertyInPlace_Save(pActiveInPlace, pLine);
		axl_win_TPropertyInPlace_Layout(pActiveInPlace, NULL); // hide active inplace
	}
}

void
AXL_API
axl_win_TPropertyCtrl_InvalidateSplitterChange(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	long x
	)
{
	size_t i;
	size_t iEnd;
	size_t LastVisibleLine = pPropertyCtrl->m_FirstVisibleLine + pPropertyCtrl->m_VisibleLineCount;

	RECT Rect;
	GetClientRect(pPropertyCtrl->m_hWnd, &Rect);
	Rect.left = x - pPropertyCtrl->m_FirstVisibleX;
	Rect.top = Rect.bottom = 0;

    iEnd = axl_rtl_TPtrArray_GetCount(&pPropertyCtrl->m_VisibleLineArray);
	if (iEnd > LastVisibleLine)
		iEnd = LastVisibleLine;

	for (i = pPropertyCtrl->m_FirstVisibleLine; i < iEnd; i++)
	{
		axl_win_TPropertyLine* pLine = ((axl_win_TPropertyLine**) pPropertyCtrl->m_VisibleLineArray.m_p)[i];

		if (!axl_win_TPropertyLine_IsSinglePane(pLine))
		{
			Rect.bottom += pPropertyCtrl->m_LineHeight;
			continue;
		}

		if (Rect.bottom > Rect.top)
			InvalidateRect(pPropertyCtrl->m_hWnd, &Rect, false);
	
		Rect.bottom += pPropertyCtrl->m_LineHeight;
		Rect.top = Rect.bottom;
	}

	if (Rect.bottom > Rect.top)
		InvalidateRect(pPropertyCtrl->m_hWnd, &Rect, false);
}

long
AXL_API
axl_win_TPropertyCtrl_SetSplitterPos(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	long SplitterPos
	)
{
	long x;
	bool_t DidFirstVisibleXChange;

	if (SplitterPos < (long) pPropertyCtrl->m_MinNamePaneWidth)
		SplitterPos = pPropertyCtrl->m_MinNamePaneWidth;

	if (SplitterPos == pPropertyCtrl->m_SplitterPos)
		return SplitterPos;

	x = min(pPropertyCtrl->m_SplitterPos, SplitterPos);

	pPropertyCtrl->m_SplitterPos = SplitterPos;

	DidFirstVisibleXChange = axl_win_TPropertyCtrl_FixupFirstVisibleX(pPropertyCtrl);

	if (DidFirstVisibleXChange)
		axl_win_TPropertyCtrl_Invalidate(pPropertyCtrl);
	else
		axl_win_TPropertyCtrl_InvalidateSplitterChange(pPropertyCtrl, x);
	
	axl_win_TPropertyCtrl_AdjustInPlacePosition(pPropertyCtrl);	
	axl_win_TPropertyCtrl_RecalcHScroll(pPropertyCtrl);
	return SplitterPos;
}

void
AXL_API
axl_win_TPropertyCtrl_ExpandHelper(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	axl_win_TPropertyLine* pLine
	)
{
	size_t i;
	axl_win_TPropertyLine* pChild;
	
	ASSERT(pLine->m_VisibleIndex != -1);
	ASSERT(pLine->m_Flags & axl_win_EPropertyLine_IsExpanded);

	pChild = (axl_win_TPropertyLine*) axl_rtl_TList_GetHead(&pLine->m_ChildrenList);
	i = pLine->m_VisibleIndex + 1;

	while (pChild)
	{
		pChild->m_VisibleIndex = i;
		((axl_win_TPropertyLine**) pPropertyCtrl->m_VisibleLineArray.m_p)[i] =pChild;

		if (pChild->m_Flags & axl_win_EPropertyLine_IsExpanded)
		{
			axl_win_TPropertyCtrl_ExpandHelper(pPropertyCtrl, pChild);
			i += pChild->m_ChildrenHeight;
		}

		pChild = (axl_win_TPropertyLine*) axl_rtl_TList_GetNext(&pLine->m_ChildrenList, (axl_rtl_TListEntry*) pChild);
		i++;
	}
}

bool_t
AXL_API
axl_win_TPropertyCtrl_Expand(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	axl_win_TPropertyLine* pLine
	)
{
	axl_win_TPropertyHitTest HitTest = {0};

	if (pLine->m_Flags & axl_win_EPropertyLine_IsExpanded)
		return true;

	pLine->m_Flags |= axl_win_EPropertyLine_IsExpanded;

	if (axl_rtl_TList_IsEmpty(&pLine->m_ChildrenList))
		return false;

	if (pLine->m_pParent)
	{
		axl_win_TPropertyLine_ModifyChildrenHeight(pLine->m_pParent, (long) pLine->m_ChildrenHeight);
		axl_win_TPropertyLine_CalcWidth(pLine->m_pParent, axl_win_EPropertyCalcWidth_Children | axl_win_EPropertyCalcWidth_Parent);
	}

	if (pLine->m_VisibleIndex == -1)
		return true;

	if (pPropertyCtrl->m_SelectedLine != -1 && pPropertyCtrl->m_SelectedLine > pLine->m_VisibleIndex)
		pPropertyCtrl->m_SelectedLine += pLine->m_ChildrenHeight;

	axl_win_TPropertyCtrl_InsertVisibleLines(pPropertyCtrl, pLine->m_VisibleIndex + 1, pLine->m_ChildrenHeight);
	axl_win_TPropertyCtrl_ExpandHelper(pPropertyCtrl, pLine);
	axl_win_TPropertyCtrl_InvalidateLineRange(pPropertyCtrl, pLine->m_VisibleIndex, -1);
	axl_win_TPropertyCtrl_AdjustInPlacePosition(pPropertyCtrl);
	axl_win_TPropertyCtrl_CalcWidth(pPropertyCtrl, 0);
	axl_win_TPropertyCtrl_RecalcVScroll(pPropertyCtrl);
	axl_win_TPropertyCtrl_RecalcHScroll(pPropertyCtrl);

	HitTest.m_pLine = pLine;
	HitTest.m_HitTest = axl_win_EPropertyHitTest_PlusMinus;
	axl_win_TPropertyCtrl_Notify(pPropertyCtrl, axl_win_EPropertyNotify_ExpandCollapse, &HitTest);
	return true;
}

bool_t
AXL_API
axl_win_TPropertyCtrl_Collapse(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	axl_win_TPropertyLine* pLine
	)
{
	axl_win_TPropertyHitTest HitTest = {0};

	if (!(pLine->m_Flags & axl_win_EPropertyLine_IsExpanded))
		return true;

	pLine->m_Flags &= ~axl_win_EPropertyLine_IsExpanded;

	if (axl_rtl_TList_IsEmpty(&pLine->m_ChildrenList))
		return false;

	if (pLine->m_pParent)
	{
		axl_win_TPropertyLine_ModifyChildrenHeight(pLine->m_pParent, -(long) pLine->m_ChildrenHeight);
		axl_win_TPropertyLine_CalcWidth(pLine->m_pParent, axl_win_EPropertyCalcWidth_Children | axl_win_EPropertyCalcWidth_Parent);
	}

	if (pLine->m_VisibleIndex == -1)
		return true;

	if (pPropertyCtrl->m_SelectedLine != -1 && pPropertyCtrl->m_SelectedLine > pLine->m_VisibleIndex)
	{
		if (pPropertyCtrl->m_SelectedLine <= pLine->m_VisibleIndex + pLine->m_ChildrenHeight)
			axl_win_TPropertyCtrl_KillSelection(pPropertyCtrl);
		else
			pPropertyCtrl->m_SelectedLine -= pLine->m_ChildrenHeight;
	}

	axl_win_TPropertyCtrl_DeleteVisibleLines(pPropertyCtrl, pLine->m_VisibleIndex + 1, pLine->m_ChildrenHeight);
	axl_win_TPropertyCtrl_InvalidateLineRange(pPropertyCtrl, pLine->m_VisibleIndex, -1);
	axl_win_TPropertyCtrl_AdjustInPlacePosition(pPropertyCtrl);
	axl_win_TPropertyCtrl_CalcWidth(pPropertyCtrl, 0);
	axl_win_TPropertyCtrl_RecalcVScroll(pPropertyCtrl);
	axl_win_TPropertyCtrl_RecalcHScroll(pPropertyCtrl);

	HitTest.m_pLine = pLine;
	HitTest.m_HitTest = axl_win_EPropertyHitTest_PlusMinus;
	axl_win_TPropertyCtrl_Notify(pPropertyCtrl, axl_win_EPropertyNotify_ExpandCollapse, &HitTest);
	return true;
}

void
AXL_API
axl_win_TPropertyCtrl_Hover(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	int Hover,
	axl_win_TPropertyLine* pLine
	)
{
	TRACKMOUSEEVENT Track = { sizeof(Track), 0, pPropertyCtrl->m_hWnd };

	bool_t IsMouseLeave = Hover & axl_win_EPropertyHover_Leave;
	Hover &= ~axl_win_EPropertyHover_Leave;

	if (pPropertyCtrl->m_Hover == Hover && pPropertyCtrl->m_pHoverAnchorLine == pLine)
		return;

	switch (pPropertyCtrl->m_Hover)
	{
	case axl_win_EPropertyHover_None:
		Track.dwFlags = TME_LEAVE;
		TrackMouseEvent(&Track);
		break;

	case axl_win_EPropertyHover_OptionBox:
		axl_win_TPropertyLine_SetOptionBoxVolatileState(pPropertyCtrl->m_pHoverAnchorLine, axl_win_EPropertyButtonVolatileState_Normal);
		break;

	case axl_win_EPropertyHover_Button:
		axl_win_TPropertyLine_SetButtonVolatileState(pPropertyCtrl->m_pHoverAnchorLine, axl_win_EPropertyButtonVolatileState_Normal);
		break;

	case axl_win_EPropertyHover_Menu:
		if (IsMouseLeave && !axl_win_TPropertyCtrl_IsMenuActive(pPropertyCtrl))
			axl_win_TPropertyCtrl_KillSelection(pPropertyCtrl);
		break;
	}

	switch (Hover)
	{
	case axl_win_EPropertyHover_None:
		Track.dwFlags = TME_CANCEL;
		TrackMouseEvent(&Track);
		break;

	case axl_win_EPropertyHover_OptionBox:
		axl_win_TPropertyLine_SetOptionBoxVolatileState(pLine, axl_win_EPropertyButtonVolatileState_Hover);
		break;

	case axl_win_EPropertyHover_Button:
		axl_win_TPropertyLine_SetButtonVolatileState(pLine, axl_win_EPropertyButtonVolatileState_Hover);
		break;

	case axl_win_EPropertyHover_Menu:
		axl_win_TPropertyCtrl_SetSelection(pPropertyCtrl, pLine->m_VisibleIndex, false);
		break;
	}

	pPropertyCtrl->m_Hover = Hover;
	pPropertyCtrl->m_pHoverAnchorLine = pLine;
}

void
AXL_API
axl_win_TPropertyCtrl_StartDrag(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	int Drag,
	int HitTest,
	axl_win_TPropertyLine* pLine
	)
{
	axl_win_TPropertyCtrl_Hover(pPropertyCtrl, axl_win_EPropertyHover_None, NULL); // stop hover

	pPropertyCtrl->m_Drag = Drag;
	pPropertyCtrl->m_DragAnchorHitTest = HitTest;
	pPropertyCtrl->m_pDragAnchorLine = pLine;

	switch (Drag)
	{
	case axl_win_EPropertyDrag_ReArrange:
		axl_win_TPropertyCtrl_SetSelection(pPropertyCtrl, pLine->m_VisibleIndex, false);
		break;

	case axl_win_EPropertyDrag_Selection:
		axl_win_TPropertyCtrl_SetSelection(pPropertyCtrl, pLine ? pLine->m_VisibleIndex : -1, false);
		break;

	case axl_win_EPropertyDrag_OptionBox:
		axl_win_TPropertyLine_SetOptionBoxVolatileState(pPropertyCtrl->m_pDragAnchorLine, axl_win_EPropertyButtonVolatileState_Pushed);
		break;

	case axl_win_EPropertyDrag_Button:
		axl_win_TPropertyLine_SetButtonVolatileState(pPropertyCtrl->m_pDragAnchorLine, axl_win_EPropertyButtonVolatileState_Pushed);
		break;
	}

	if (GetCapture() != pPropertyCtrl->m_hWnd)
		SetCapture(pPropertyCtrl->m_hWnd);
}

void
AXL_API
axl_win_TPropertyCtrl_StopDrag(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	switch (pPropertyCtrl->m_Drag)
	{
	case axl_win_EPropertyDrag_None:
		return;

	case axl_win_EPropertyDrag_OptionBox:
		axl_win_TPropertyLine_SetOptionBoxVolatileState(pPropertyCtrl->m_pDragAnchorLine, axl_win_EPropertyButtonVolatileState_Normal);
		break;

	case axl_win_EPropertyDrag_Button:
		axl_win_TPropertyLine_SetButtonVolatileState(pPropertyCtrl->m_pDragAnchorLine, axl_win_EPropertyButtonVolatileState_Normal);
		break;
	}

	pPropertyCtrl->m_Drag = axl_win_EPropertyDrag_None;
	pPropertyCtrl->m_DragAnchorHitTest = axl_win_EPropertyHitTest_None;
	pPropertyCtrl->m_pDragAnchorLine = NULL;
	
	if (GetCapture() == pPropertyCtrl->m_hWnd)
		ReleaseCapture();
}

HWND
AXL_API
axl_win_TPropertyCtrl_CreateMenuPropertyCtrl(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	HINSTANCE hInstance = (HINSTANCE) (LONG_PTR) GetWindowLongPtr(pPropertyCtrl->m_hWnd, GWLP_HINSTANCE);

	ASSERT(!pPropertyCtrl->m_pMenuPropertyCtrl);

	pPropertyCtrl->m_pMenuPropertyCtrl = axl_mem_Allocate(sizeof(axl_win_TPropertyCtrl));	
	axl_win_TPropertyCtrl_Construct(pPropertyCtrl->m_pMenuPropertyCtrl);
	axl_win_TPropertyCtrl_SetImageList(pPropertyCtrl->m_pMenuPropertyCtrl, pPropertyCtrl->m_hImageList);

	pPropertyCtrl->m_pMenuPropertyCtrl->m_pMenuRootPropertyCtrl = axl_win_TPropertyCtrl_GetMenuRootPropertyCtrl(pPropertyCtrl);
	pPropertyCtrl->m_pMenuPropertyCtrl->m_pMenuParentPropertyCtrl = pPropertyCtrl;
	pPropertyCtrl->m_pMenuPropertyCtrl->m_HasGridLinesAtRoot = false;
	pPropertyCtrl->m_pMenuPropertyCtrl->m_SplitterPos = 0;

	return axl_win_TPropertyCtrl_Create(pPropertyCtrl->m_pMenuPropertyCtrl, WS_POPUP | WS_CHILD | WS_BORDER, NULL, pPropertyCtrl->m_hWnd, 0, hInstance);
}

bool_t 
AXL_API
axl_win_TPropertyCtrl_IsMenuWindow(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	HWND hWnd
	)
{
	while (axl_win_TPropertyCtrl_IsMenuActive(pPropertyCtrl))
	{
		ASSERT(pPropertyCtrl->m_pMenuPropertyCtrl);
		if (pPropertyCtrl->m_pMenuPropertyCtrl->m_hWnd == hWnd)
			return true;

		pPropertyCtrl = pPropertyCtrl->m_pMenuPropertyCtrl;
	}

	return false;
}

void
AXL_API
axl_win_TPropertyCtrl_HideMenu(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	if (!axl_win_TPropertyCtrl_IsMenuActive(pPropertyCtrl))
		return;

	if (!pPropertyCtrl->m_pMenuParentPropertyCtrl)
		axl_win_TPropertyLine_SetButtonVolatileState(pPropertyCtrl->m_pMenuAnchorLine, axl_win_EPropertyButtonVolatileState_Normal);

	while (pPropertyCtrl)
	{
		if (!axl_win_TPropertyCtrl_IsMenuActive(pPropertyCtrl))
			break;
		
		ShowWindow(pPropertyCtrl->m_pMenuPropertyCtrl->m_hWnd, SW_HIDE);
		pPropertyCtrl->m_pMenuAnchorLine = NULL;
		pPropertyCtrl = pPropertyCtrl->m_pMenuPropertyCtrl;
	}
}

void
AXL_API
axl_win_TPropertyCtrl_CancelMenu(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	axl_win_TPropertyCtrl* pRootPropertyCtrl = axl_win_TPropertyCtrl_GetMenuRootPropertyCtrl(pPropertyCtrl);
	KillTimer(pRootPropertyCtrl->m_hWnd, axl_win_EPropertyTimer_MenuDelay);
	axl_win_TPropertyCtrl_HideMenu(pRootPropertyCtrl);
}

void
AXL_API
axl_win_TPropertyCtrl_TrackMenu(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	axl_win_TPropertyLine* pLine
	)
{
	MSG Msg;
	POINT Pos;
	SIZE Size;

	axl_win_TPropertyCtrl* pMenuPropertyCtrl;
	axl_win_TPropertyLine* pChild;
	size_t VisibleIndex;

	axl_win_TPropertyCtrl* pRootPropertyCtrl = axl_win_TPropertyCtrl_GetMenuRootPropertyCtrl(pPropertyCtrl);
	KillTimer(pRootPropertyCtrl->m_hWnd, axl_win_EPropertyTimer_MenuDelay);

	axl_win_TPropertyCtrl_Hover(pPropertyCtrl, axl_win_EPropertyHover_None, NULL); // stop hover

	if (pPropertyCtrl->m_pMenuAnchorLine == pLine)
		return;

	axl_win_TPropertyCtrl_HideMenu(pPropertyCtrl);

	if (!pPropertyCtrl->m_pMenuParentPropertyCtrl)
		axl_win_TPropertyLine_SetButtonVolatileState(pLine, axl_win_EPropertyButtonVolatileState_Pushed);

	if (!pLine || axl_rtl_TList_IsEmpty(&pLine->m_ChildrenList))
		return;

	ASSERT(pLine->m_VisibleIndex != -1);
	ASSERT(pLine->m_Flags & axl_win_EPropertyLine_Menu);

	if (!pPropertyCtrl->m_pMenuPropertyCtrl)
		axl_win_TPropertyCtrl_CreateMenuPropertyCtrl(pPropertyCtrl);

	pMenuPropertyCtrl = pPropertyCtrl->m_pMenuPropertyCtrl;
	ASSERT(pMenuPropertyCtrl);

	axl_win_TPropertyCtrl_Clear(pMenuPropertyCtrl);
	pMenuPropertyCtrl->m_pLineList = &pLine->m_ChildrenList;

	axl_rtl_TPtrArray_Reserve(&pMenuPropertyCtrl->m_VisibleLineArray, pLine->m_ChildrenList.m_Count);

	VisibleIndex = 0;
	pChild = (axl_win_TPropertyLine*) axl_rtl_TList_GetHead(&pLine->m_ChildrenList);
	while (pChild)
	{
		axl_rtl_TPtrArray_AppendElement(&pMenuPropertyCtrl->m_VisibleLineArray, pChild);
		pChild->m_pPropertyCtrl = pMenuPropertyCtrl;
		pChild->m_VisibleIndex = VisibleIndex;
		pChild = (axl_win_TPropertyLine*) axl_rtl_TList_GetNext(&pLine->m_ChildrenList, (axl_rtl_TListEntry*) pChild);
		VisibleIndex++;
	}

	axl_win_TPropertyCtrl_UpdateAllLineWidths(pMenuPropertyCtrl);

	pPropertyCtrl->m_pMenuAnchorLine = pLine;
	
	axl_win_TPropertyCtrl_GetSize(pMenuPropertyCtrl, &Size);
	Size.cx += 2 * GetSystemMetrics(SM_CXBORDER) + GetSystemMetrics(SM_CXVSCROLL);
	Size.cy += 2 * GetSystemMetrics(SM_CYBORDER);

	axl_win_TPropertyCtrl_GetMenuPos(pPropertyCtrl, &Size, &Pos);

	if (!axl_win_TPropertyCtrl_IsMenu(pPropertyCtrl))
		Pos.x++;

	SetWindowPos(
		pMenuPropertyCtrl->m_hWnd, 
		NULL, 
		Pos.x, Pos.y, 
		Size.cx, Size.cy,
		SWP_NOZORDER | SWP_SHOWWINDOW | SWP_NOACTIVATE
		);

	if (axl_win_TPropertyCtrl_IsMenu(pPropertyCtrl))
		return;

	// this is only for the root property ctrl

	SetFocus(pPropertyCtrl->m_hWnd);

	while (axl_win_TPropertyCtrl_IsMenuActive(pPropertyCtrl))
	{
		PeekMessage(&Msg, NULL, 0, 0, PM_NOREMOVE);

		switch (Msg.message)
		{
		case WM_NCLBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
		case WM_NCMBUTTONDOWN:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
			if (!axl_win_TPropertyCtrl_IsMenuWindow(pPropertyCtrl, Msg.hwnd))
				axl_win_TPropertyCtrl_CancelMenu(pPropertyCtrl);
			break;
		}

        PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE);
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
}

void
AXL_API
axl_win_TPropertyCtrl_UpdateMenu(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	if (!axl_win_TPropertyCtrl_IsMenuActive(pPropertyCtrl))
		return;

	pPropertyCtrl = pPropertyCtrl->m_pMenuPropertyCtrl;

	while (pPropertyCtrl)
	{
		axl_win_TPropertyLine* pLine = axl_win_TPropertyCtrl_GetSelectedLine(pPropertyCtrl);

		if (!pLine || pPropertyCtrl->m_pMenuAnchorLine != pLine)
		{
			if (pPropertyCtrl->m_pMenuAnchorLine)
				axl_win_TPropertyCtrl_HideMenu(pPropertyCtrl);
		
			if (pLine && (pLine->m_Flags & axl_win_EPropertyLine_Menu))
				axl_win_TPropertyCtrl_TrackMenu(pPropertyCtrl, pLine);

			break;
		}

		pPropertyCtrl = pPropertyCtrl->m_pMenuPropertyCtrl;
	}	
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// message handlers

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnCreate(axl_win_TPropertyCtrl* pPropertyCtrl)
{	
	HINSTANCE hInstance = (HINSTANCE) (LONG_PTR) GetWindowLongPtr(pPropertyCtrl->m_hWnd, GWLP_HINSTANCE);
	TOOLINFO ToolInfo = { sizeof(ToolInfo) };

	pPropertyCtrl->m_hWndToolTip = CreateWindow(TOOLTIPS_CLASS, NULL, TTS_ALWAYSTIP | TTS_NOPREFIX, 0,0,0,0, pPropertyCtrl->m_hWnd, 0, hInstance, NULL);
	
	if (!pPropertyCtrl->m_hWndToolTip)
		return 0;

	ToolInfo.uFlags   = TTF_IDISHWND;
	ToolInfo.hwnd     = GetParent(pPropertyCtrl->m_hWnd); 
	ToolInfo.uId      = (UINT_PTR) pPropertyCtrl->m_hWnd;
    ToolInfo.hinst    = hInstance; 
	ToolInfo.lpszText = LPSTR_TEXTCALLBACK;  

	SendMessage(pPropertyCtrl->m_hWndToolTip, TTM_ADDTOOL, 0, (LPARAM) &ToolInfo);
	SendMessage(pPropertyCtrl->m_hWndToolTip, TTM_SETMAXTIPWIDTH, 0, 400);
	SendMessage(pPropertyCtrl->m_hWndToolTip, TTM_SETDELAYTIME, TTDT_AUTOPOP, SHRT_MAX);
	SendMessage(pPropertyCtrl->m_hWndToolTip, TTM_SETDELAYTIME, TTDT_INITIAL, pPropertyCtrl->m_ToolTipShowDelay);
	
	return 0;
}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnDestroy(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	DestroyWindow(pPropertyCtrl->m_hWndToolTip);
	pPropertyCtrl->m_hWndToolTip = NULL;

	// see http://pluralsight.com/blogs/keith/archive/2005/01/13/4940.aspx
#pragma warning(disable:4244)
	SetWindowLongPtr(pPropertyCtrl->m_hWnd, 0, 0);
#pragma warning(default:4244)

	pPropertyCtrl->m_hWnd = NULL;
	return 0;
}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnSetFocus(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	if (!axl_win_TPropertyCtrl_IsMenuActive(pPropertyCtrl) && pPropertyCtrl->m_pActiveInPlace)
		axl_win_TPropertyInPlace_SetFocus(pPropertyCtrl->m_pActiveInPlace);

	return 0;
}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnKillFocus(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	return 0;
}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnPaint(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	PAINTSTRUCT PaintStruct;
	HDC hdc = BeginPaint(pPropertyCtrl->m_hWnd, &PaintStruct);
	axl_win_TPropertyCtrl_PaintRect(pPropertyCtrl, hdc, &PaintStruct.rcPaint);
	EndPaint(pPropertyCtrl->m_hWnd, &PaintStruct);
	return 0;
}

/*

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnPaint(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	int Result;
	RECT UpdateRect;
	HDC hdc;
	DWORD i;

	BYTE Buffer[sizeof(RGNDATA) + 2 * sizeof(RECT)];
	RGNDATA* pRgnData = (RGNDATA*) Buffer;
	RECT* pRects = (RECT*) pRgnData->Buffer;

	GetUpdateRgn(pPropertyCtrl->m_hWnd, pPropertyCtrl->m_hUpdateRgn, false);
	if (Result != COMPLEXREGION)
		return axl_win_TPropertyCtrl_DefOnPaint(pPropertyCtrl);

	Result = GetRegionData(pPropertyCtrl->m_hUpdateRgn, sizeof(Buffer), pRgnData);
	if (!Result || pRgnData->rdh.nCount > 2)
		return axl_win_TPropertyCtrl_DefOnPaint(pPropertyCtrl);

	ValidateRgn(pPropertyCtrl->m_hWnd, pPropertyCtrl->m_hUpdateRgn);

	hdc = GetDC(pPropertyCtrl->m_hWnd);
	SelectClipRgn(hdc, pPropertyCtrl->m_hUpdateRgn);

	axl_dbg_Trace("get rgn: %d, count: %d (%d,%d,%d,%d)\n", 
		Result, 
		pRgnData->rdh.nCount, 
		pRgnData->rdh.rcBound.left,
		pRgnData->rdh.rcBound.top,
		pRgnData->rdh.rcBound.right,
		pRgnData->rdh.rcBound.bottom
		);

	for (i = 0; i < pRgnData->rdh.nCount; i++)
		axl_win_TPropertyCtrl_PaintRect(pPropertyCtrl, hdc, &pRects[i]);

	axl_gr_TColor_SetRgb(&pPropertyCtrl->m_TextPaint.m_StdAttr.m_BackColor, GetSysColor(COLOR_BTNFACE));

	ReleaseDC(pPropertyCtrl->m_hWnd, hdc);
	return 0;
}

*/

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnSize(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	bool_t DidFirstVisibleLineChange;
	bool_t DidFirstVisibleXChange;

	RECT ClientRect;
	GetClientRect(pPropertyCtrl->m_hWnd, &ClientRect);

	pPropertyCtrl->m_VisibleWidth = ClientRect.right;
	pPropertyCtrl->m_FullyVisibleLineCount = ClientRect.bottom / pPropertyCtrl->m_LineHeight;
	pPropertyCtrl->m_VisibleLineCount = pPropertyCtrl->m_FullyVisibleLineCount;
	
	if (ClientRect.bottom % pPropertyCtrl->m_LineHeight)
		pPropertyCtrl->m_VisibleLineCount++;

	DidFirstVisibleLineChange = axl_win_TPropertyCtrl_FixupFirstVisibleLine(pPropertyCtrl);
	DidFirstVisibleXChange = axl_win_TPropertyCtrl_FixupFirstVisibleX(pPropertyCtrl);

	axl_win_TPropertyCtrl_RecalcVScroll(pPropertyCtrl);
	axl_win_TPropertyCtrl_RecalcHScroll(pPropertyCtrl);

	axl_win_TPropertyCtrl_AdjustInPlacePosition(pPropertyCtrl);

	if (DidFirstVisibleLineChange || DidFirstVisibleXChange)
		axl_win_TPropertyCtrl_Invalidate(pPropertyCtrl);

	return 0;
}

void
AXL_API
axl_win_TPropertyCtrl_OnScroll(
	axl_win_TPropertyCtrl* pPropertyCtrl, 
	int Bar,
	int Code
	)
{
	int NewPos;
	int MaxPos;

	SCROLLINFO ScrollInfo = { sizeof(ScrollInfo) };
	ScrollInfo.fMask = SIF_ALL;
	
	GetScrollInfo(pPropertyCtrl->m_hWnd, Bar, &ScrollInfo);

	NewPos = ScrollInfo.nPos;
	MaxPos = ScrollInfo.nMax - ScrollInfo.nPage + 1;

	switch (Code)
	{
	case SB_TOP:
		NewPos = 0;
		break;

	case SB_LINEUP:
		if (Bar == SB_VERT)
			NewPos--;
		else
			NewPos -= pPropertyCtrl->m_LineHeight;
		break;

	case SB_LINEDOWN:
		if (Bar == SB_VERT)
			NewPos++;
		else
			NewPos += pPropertyCtrl->m_LineHeight;
		break;

	case SB_PAGEUP:
		NewPos -= ScrollInfo.nPage; 
		break;

	case SB_PAGEDOWN:
		NewPos += ScrollInfo.nPage; 
		break;

	case SB_BOTTOM:
		NewPos = ScrollInfo.nMax; 
		break;

	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		NewPos = ScrollInfo.nTrackPos; 
		break;
	}

	if (NewPos < 0)
		NewPos = 0;
	else if (NewPos > MaxPos)
		NewPos = MaxPos;

	if (NewPos == ScrollInfo.nPos)
		return;

	if (Bar == SB_VERT)
	{
		pPropertyCtrl->m_FirstVisibleLine = NewPos;
		axl_win_TPropertyCtrl_FixupFirstVisibleLine(pPropertyCtrl);
	}
	else
	{
		pPropertyCtrl->m_FirstVisibleX = NewPos;
		axl_win_TPropertyCtrl_FixupFirstVisibleX(pPropertyCtrl);
	}

	ScrollInfo.fMask = SIF_POS;
	ScrollInfo.nPos = NewPos;
	SetScrollInfo(pPropertyCtrl->m_hWnd, Bar, &ScrollInfo, TRUE);

	axl_win_TPropertyCtrl_Invalidate(pPropertyCtrl);
	axl_win_TPropertyCtrl_AdjustInPlacePosition(pPropertyCtrl);
	axl_win_TPropertyCtrl_CancelMenu(pPropertyCtrl);
}

void
AXL_API
axl_win_TPropertyCtrl_StdLButtonDownHandler(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	axl_win_TPropertyHitTest* pHitTest
	)
{
	if (!axl_win_TPropertyCtrl_IsMenu(pPropertyCtrl))
	{
		SetFocus(pPropertyCtrl->m_hWnd);
		axl_win_TPropertyCtrl_StartDrag(pPropertyCtrl, axl_win_EPropertyDrag_Selection, pHitTest->m_HitTest, pHitTest->m_pLine);
	}

}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnLButtonDown(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	long x,
	long y
	)
{
	axl_win_TPropertyHitTest HitTest;
	axl_win_TPropertyLine* pLine = axl_win_TPropertyCtrl_HitTest(pPropertyCtrl, x, y, &HitTest);
	
	switch (HitTest.m_HitTest)
	{
	case axl_win_EPropertyHitTest_Splitter:
		pPropertyCtrl->m_DragDelta.x = pPropertyCtrl->m_SplitterPos - (x + pPropertyCtrl->m_FirstVisibleX);
		pPropertyCtrl->m_DragDelta.y = 0;
		axl_win_TPropertyCtrl_StartDrag(pPropertyCtrl, axl_win_EPropertyDrag_Splitter, HitTest.m_HitTest, pLine);
		break;

	case axl_win_EPropertyHitTest_PlusMinus:
		axl_win_TPropertyCtrl_ExpandCollapse(pPropertyCtrl, pLine, (pLine->m_Flags & axl_win_EPropertyLine_IsExpanded) == 0);
		break;

	case axl_win_EPropertyHitTest_ReArrange:
		axl_win_TPropertyCtrl_StartDrag(pPropertyCtrl, axl_win_EPropertyDrag_ReArrange, HitTest.m_HitTest, pLine);
		break;

	case axl_win_EPropertyHitTest_OptionBox:
		if (!(pLine->m_Flags & axl_win_EPropertyLine_IsDisabled))
		{
//			axl_win_TPropertyCtrl_SetSelection(pPropertyCtrl, pLine->m_VisibleIndex);
			axl_win_TPropertyCtrl_StartDrag(pPropertyCtrl, axl_win_EPropertyDrag_OptionBox, HitTest.m_HitTest, pLine);
		}
		else
			axl_win_TPropertyCtrl_StdLButtonDownHandler(pPropertyCtrl, &HitTest);

		break;

	case axl_win_EPropertyHitTest_Hyperlink:
		if (!(pLine->m_Flags & axl_win_EPropertyLine_IsDisabled))
		{
			pPropertyCtrl->m_pDragAnchorHyperlink = HitTest.m_pHyperlink;
			axl_win_TPropertyCtrl_StartDrag(pPropertyCtrl, axl_win_EPropertyDrag_Hyperlink, HitTest.m_HitTest, pLine);
		}
		else
			axl_win_TPropertyCtrl_StdLButtonDownHandler(pPropertyCtrl, &HitTest);

		break;

	case axl_win_EPropertyHitTest_Menu:
		if (!(pLine->m_Flags & axl_win_EPropertyLine_IsDisabled))
		{
//			axl_win_TPropertyCtrl_SetSelection(pPropertyCtrl, pLine->m_VisibleIndex);
			axl_win_TPropertyCtrl_TrackMenu(pPropertyCtrl, pLine);
		}
		else
			axl_win_TPropertyCtrl_StdLButtonDownHandler(pPropertyCtrl, &HitTest);

		break;

	case axl_win_EPropertyHitTest_Button:
		if (!(pLine->m_Flags & axl_win_EPropertyLine_IsDisabled))
		{
//			axl_win_TPropertyCtrl_SetSelection(pPropertyCtrl, pLine->m_VisibleIndex);
			axl_win_TPropertyCtrl_StartDrag(pPropertyCtrl, axl_win_EPropertyDrag_Button, HitTest.m_HitTest, pLine);
		}
		else
			axl_win_TPropertyCtrl_StdLButtonDownHandler(pPropertyCtrl, &HitTest);

		break;

	default:
		axl_win_TPropertyCtrl_StdLButtonDownHandler(pPropertyCtrl, &HitTest);
	}

	return 0;
}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnLButtonDblClk(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	long x,
	long y
	)
{
	axl_win_TPropertyHitTest HitTest;
	axl_win_TPropertyLine* pLine = axl_win_TPropertyCtrl_HitTest(pPropertyCtrl, x, y, &HitTest);

	switch (HitTest.m_HitTest)
	{
	case axl_win_EPropertyHitTest_Name:
	case axl_win_EPropertyHitTest_Icon:
		if (!axl_rtl_TList_IsEmpty(&pLine->m_ChildrenList))
		{
			axl_win_TPropertyCtrl_ExpandCollapse(pPropertyCtrl, pLine, (pLine->m_Flags & axl_win_EPropertyLine_IsExpanded) == 0);
			break;
		}

	default:
		return axl_win_TPropertyCtrl_OnLButtonDown(pPropertyCtrl, x, y);
	}

	return 0;
}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnLButtonUp(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	long x,
	long y
	)
{
	axl_win_TPropertyHitTest HitTest;
	axl_win_TPropertyLine* pLine = axl_win_TPropertyCtrl_HitTest(pPropertyCtrl, x, y, &HitTest);

	switch (pPropertyCtrl->m_Drag)
	{
	case axl_win_EPropertyDrag_Selection:
		if (pPropertyCtrl->m_pActiveInPlace)
			axl_win_TPropertyInPlace_SetFocus(pPropertyCtrl->m_pActiveInPlace);
		break;

	case axl_win_EPropertyDrag_Hyperlink:
		if (HitTest.m_pHyperlink == pPropertyCtrl->m_pDragAnchorHyperlink)
			axl_win_TPropertyCtrl_Notify(pPropertyCtrl, axl_win_EPropertyNotify_HyperlinkClick, &HitTest);
		break;

	case axl_win_EPropertyDrag_OptionBox:
		if (pLine != pPropertyCtrl->m_pDragAnchorLine || HitTest.m_HitTest != pPropertyCtrl->m_DragAnchorHitTest)
			break;

		switch (pLine->m_OptionBoxType)
		{
		case axl_win_EPropertyOptionBoxType_RadioButton:
			axl_win_TPropertyLine_SetOptionBoxCheckState(pLine, true);
			break;

		case axl_win_EPropertyOptionBoxType_CheckBox:
			axl_win_TPropertyLine_SetOptionBoxCheckState(pLine, !pLine->m_OptionBoxCheckState);
			break;
		}

		axl_win_TPropertyCtrl_Notify(pPropertyCtrl, axl_win_EPropertyNotify_OptionBoxClick, &HitTest);
		break;

	case axl_win_EPropertyDrag_Button:
		if (pLine == pPropertyCtrl->m_pDragAnchorLine && pLine->m_ButtonVolatileState == axl_win_EPropertyButtonVolatileState_Pushed)
			axl_win_TPropertyCtrl_Notify(pPropertyCtrl, axl_win_EPropertyNotify_ButtonClick, &HitTest);
		break;

	default:
		if (axl_win_TPropertyCtrl_IsMenu(pPropertyCtrl) && axl_rtl_TList_IsEmpty(&pLine->m_ChildrenList) && !pLine->m_OptionBoxType)
		{
			axl_win_TPropertyCtrl_CancelMenu(pPropertyCtrl);
			
			if (pLine)
				axl_win_TPropertyCtrl_Notify(pPropertyCtrl, axl_win_EPropertyNotify_MenuClick, &HitTest);
		}
	}

	axl_win_TPropertyCtrl_StopDrag(pPropertyCtrl);
	return 0;
}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnRButtonDown(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	long x,
	long y
	)
{
	return 0;
}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnRButtonUp(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	long x,
	long y
	)
{
	return 0;
}

void
AXL_API
axl_win_TPropertyCtrl_SetToolTip(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	axl_win_TPropertyHitTest* pHitTest
	)
{
	axl_win_TPropertyLine* pLine = pHitTest->m_pLine;

	TOOLINFO ToolInfo = { sizeof(ToolInfo) };
	const tchar_t* pText;

	if (pLine == pPropertyCtrl->m_pLastTooltipLine)
		return;

	// handle different re-show time delay

	SendMessage(
		pPropertyCtrl->m_hWndToolTip, 
		TTM_SETDELAYTIME, TTDT_INITIAL, 
		IsWindowVisible(pPropertyCtrl->m_hWndToolTip) ? 
			pPropertyCtrl->m_ToolTipReShowDelay :
			pPropertyCtrl->m_ToolTipShowDelay
		);

	SendMessage(pPropertyCtrl->m_hWndToolTip, TTM_ACTIVATE, false, 0);

	if (!pLine)
	{
		pPropertyCtrl->m_pLastTooltipLine = NULL;
		return;
	}

	if (!axl_rtl_TString_IsEmpty(&pLine->m_ToolTipText))
		pText = pLine->m_ToolTipText.m_p;
	else if ((pHitTest->m_HitTest == axl_win_EPropertyHitTest_Name || pHitTest->m_HitTest == axl_win_EPropertyHitTest_OptionBox) && !axl_win_TPropertyLine_IsPartFullyVisible(pLine, axl_win_EPropertyLinePart_Name))
		pText = pLine->m_Name.m_p;
	else if (pHitTest->m_HitTest == axl_win_EPropertyHitTest_Value && !axl_win_TPropertyLine_IsPartFullyVisible(pLine, axl_win_EPropertyLinePart_Value))
		pText = pLine->m_Value.m_p;
	else
	{
		pPropertyCtrl->m_pLastTooltipLine = NULL;
		return;
	}

	ToolInfo.uFlags = TTF_IDISHWND;
	ToolInfo.hwnd = GetParent(pPropertyCtrl->m_hWnd);
	ToolInfo.uId = (uintptr_t) pPropertyCtrl->m_hWnd;
	ToolInfo.lpszText = (tchar_t*) pText;

	SendMessage(pPropertyCtrl->m_hWndToolTip, TTM_ACTIVATE, true, 0);

	if (!axl_rtl_TString_IsEmpty(&pLine->m_ToolTipTitle))
		SendMessage(pPropertyCtrl->m_hWndToolTip, TTM_SETTITLE, pLine->m_ToolTipIcon, (LPARAM) pLine->m_ToolTipTitle.m_p);
	else
		SendMessage(pPropertyCtrl->m_hWndToolTip, TTM_SETTITLE, 0, 0);

	SendMessage(pPropertyCtrl->m_hWndToolTip, TTM_UPDATETIPTEXT, 0, (LPARAM) &ToolInfo);
	pPropertyCtrl->m_pLastTooltipLine = pLine;
}

bool_t
AXL_API
axl_win_TPropertyCtrl_ReArrange(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	axl_win_TPropertyLine* pLineFrom,
	axl_win_TPropertyLine* pLineTo
	)
{
	size_t UpperLine;
	size_t LowerLine;

	if (pLineFrom == pLineTo || 
		pLineFrom->m_pParent != pLineTo->m_pParent ||
		!axl_rtl_TList_IsEmpty(&pLineFrom->m_ChildrenList) ||
		!axl_rtl_TList_IsEmpty(&pLineTo->m_ChildrenList))
		return false;

	ASSERT(pLineFrom->m_VisibleIndex != -1);
	ASSERT(pLineTo->m_VisibleIndex != -1);

	axl_rtl_TList_Remove(&pLineFrom->m_pParent->m_ChildrenList, (axl_rtl_TListEntry*) pLineFrom);
    
	if (pLineFrom->m_VisibleIndex < pLineTo->m_VisibleIndex)
	{
		UpperLine = pLineFrom->m_VisibleIndex;
		LowerLine = pLineTo->m_VisibleIndex;
		axl_rtl_TList_InsertAfter(&pLineFrom->m_pParent->m_ChildrenList, (axl_rtl_TListEntry*) pLineFrom, (axl_rtl_TListEntry*) pLineTo);
	}
	else
	{
		UpperLine = pLineTo->m_VisibleIndex;
		LowerLine = pLineFrom->m_VisibleIndex;
		axl_rtl_TList_InsertBefore(&pLineFrom->m_pParent->m_ChildrenList, (axl_rtl_TListEntry*) pLineFrom, (axl_rtl_TListEntry*) pLineTo);
	}

	axl_win_TPropertyCtrl_ReArrangeVisibleLines(pPropertyCtrl, pLineFrom->m_VisibleIndex, pLineTo->m_VisibleIndex);
	axl_win_TPropertyCtrl_InvalidateLineRange(pPropertyCtrl, UpperLine, LowerLine);
	axl_win_TPropertyCtrl_SetSelection(pPropertyCtrl, pLineFrom->m_VisibleIndex, false);
	return true;
}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnMouseMove(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	long x,
	long y
	)
{
	axl_win_TPropertyHitTest HitTest;
	axl_win_TPropertyLine* pLine = axl_win_TPropertyCtrl_HitTest(pPropertyCtrl, x, y, &HitTest);

	axl_win_TPropertyCtrl_SetToolTip(pPropertyCtrl, &HitTest);

	switch (pPropertyCtrl->m_Drag)
	{
	case axl_win_EPropertyDrag_Splitter:
		axl_win_TPropertyCtrl_SetSplitterPos(pPropertyCtrl, x + pPropertyCtrl->m_FirstVisibleX + pPropertyCtrl->m_DragDelta.x);
		break;

	case axl_win_EPropertyDrag_ReArrange:
		if (pLine && axl_win_TPropertyCtrl_ReArrange(pPropertyCtrl, pPropertyCtrl->m_pDragAnchorLine, pLine))
		{
			HitTest.m_pLine = pPropertyCtrl->m_pDragAnchorLine->m_pParent;
			axl_win_TPropertyCtrl_Notify(pPropertyCtrl, axl_win_EPropertyNotify_ReArrange, &HitTest);
		}
		break;

	case axl_win_EPropertyDrag_Selection:
		axl_win_TPropertyCtrl_SetSelection(pPropertyCtrl, y / pPropertyCtrl->m_LineHeight + pPropertyCtrl->m_FirstVisibleLine, false);
		break;

	case axl_win_EPropertyDrag_OptionBox:
		if (pLine == pPropertyCtrl->m_pDragAnchorLine && HitTest.m_HitTest == axl_win_EPropertyHitTest_OptionBox)
			axl_win_TPropertyLine_SetOptionBoxVolatileState(pPropertyCtrl->m_pDragAnchorLine, axl_win_EPropertyButtonVolatileState_Pushed);
		else
			axl_win_TPropertyLine_SetOptionBoxVolatileState(pPropertyCtrl->m_pDragAnchorLine, axl_win_EPropertyButtonVolatileState_Normal);
		break;

	case axl_win_EPropertyDrag_Button:
		if (pLine == pPropertyCtrl->m_pDragAnchorLine && HitTest.m_HitTest == axl_win_EPropertyHitTest_Button)
			axl_win_TPropertyLine_SetButtonVolatileState(pPropertyCtrl->m_pDragAnchorLine, axl_win_EPropertyButtonVolatileState_Pushed);
		else
			axl_win_TPropertyLine_SetButtonVolatileState(pPropertyCtrl->m_pDragAnchorLine, axl_win_EPropertyButtonVolatileState_Normal);
		break;

	default:
		if (!pLine)
			axl_win_TPropertyCtrl_Hover(pPropertyCtrl, axl_win_EPropertyHover_None, NULL);
		
		if (axl_win_TPropertyCtrl_IsMenuActive(pPropertyCtrl) && 
			!pPropertyCtrl->m_pMenuParentPropertyCtrl && 
			HitTest.m_HitTest == axl_win_EPropertyHitTest_Menu && 
			!(pLine->m_Flags & axl_win_EPropertyLine_IsDisabled))
			axl_win_TPropertyCtrl_TrackMenu(pPropertyCtrl, pLine);
		else if (axl_win_TPropertyCtrl_IsMenu(pPropertyCtrl) && HitTest.m_HitTest != axl_win_EPropertyHitTest_OptionBox && HitTest.m_HitTest != axl_win_EPropertyHitTest_Button)
			axl_win_TPropertyCtrl_Hover(pPropertyCtrl, axl_win_EPropertyHover_Menu, pLine);
		else if (HitTest.m_HitTest == axl_win_EPropertyHitTest_Button)
			axl_win_TPropertyCtrl_Hover(pPropertyCtrl, axl_win_EPropertyHover_Button, pLine);
		else if (HitTest.m_HitTest == axl_win_EPropertyHitTest_Menu)
			axl_win_TPropertyCtrl_Hover(pPropertyCtrl, axl_win_EPropertyHover_Button, pLine);
		else if (HitTest.m_HitTest == axl_win_EPropertyHitTest_OptionBox)
			axl_win_TPropertyCtrl_Hover(pPropertyCtrl, axl_win_EPropertyHover_OptionBox, pLine);
		else 
			axl_win_TPropertyCtrl_Hover(pPropertyCtrl, axl_win_EPropertyHover_None, NULL);
	}

	return 0;
}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnMouseLeave(axl_win_TPropertyCtrl* pPropertyCtrl)
{
	axl_win_TPropertyCtrl_Hover(pPropertyCtrl, axl_win_EPropertyHover_None | axl_win_EPropertyHover_Leave, NULL);
	return 0;
}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnMouseWheel(
	axl_win_TPropertyCtrl* pPropertyCtrl, 
	short Delta
	)
{
	int Code;
	int LineCount; 
	int i;

	if (Delta > 0)
	{
		Code = SB_LINEUP;
		LineCount = Delta / WHEEL_DELTA;
	}
	else
	{
		Code = SB_LINEDOWN;
		LineCount = -Delta / WHEEL_DELTA;
	}

	LineCount *= pPropertyCtrl->m_MouseWheelSpeed;
	
	for (i = 0; i < LineCount; i++)
		axl_win_TPropertyCtrl_OnScroll(pPropertyCtrl, SB_VERT, Code);

	return 0;
}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnSetCursor(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	WPARAM wParam,
	LPARAM lParam
	)
{
	axl_win_TPropertyHitTest HitTest;

	POINT Pos;
	GetCursorPos(&Pos);
	ScreenToClient(pPropertyCtrl->m_hWnd, &Pos);

	axl_win_TPropertyCtrl_HitTest(pPropertyCtrl, Pos.x, Pos.y, &HitTest);

	switch (HitTest.m_HitTest)
	{
	case axl_win_EPropertyHitTest_ReArrange:
		SetCursor(pPropertyCtrl->m_hReArrangeCursor);
		return TRUE;

	case axl_win_EPropertyHitTest_Splitter:
		SetCursor(pPropertyCtrl->m_hSplitterCursor);
		return TRUE;

	case axl_win_EPropertyHitTest_Hyperlink:
		SetCursor(pPropertyCtrl->m_hHyperlinkCursor);
		return TRUE;

	default:
		return DefWindowProc(pPropertyCtrl->m_hWnd, WM_SETCURSOR, wParam, lParam);
	}
}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnTimer(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	int Timer
	)
{
	if (Timer != axl_win_EPropertyTimer_MenuDelay)
		return 0;

	KillTimer(pPropertyCtrl->m_hWnd, Timer);
	axl_win_TPropertyCtrl_UpdateMenu(pPropertyCtrl);
	return 0;
}

LRESULT 
CALLBACK 
axl_win_TPropertyCtrl_WndProc(
	HWND hWnd,
	UINT Msg,
	WPARAM wParam,
	LPARAM lParam
	)
{
	axl_win_TPropertyCtrl* pPropertyCtrl;
	LRESULT lResult = 0;

	if (Msg == WM_CREATE) // special handling
	{
		// establish connection axl_win_TPropertyCtrl <-> HWND

		CREATESTRUCT* p = (CREATESTRUCT*) lParam;
		pPropertyCtrl = (axl_win_TPropertyCtrl*) p->lpCreateParams;
		pPropertyCtrl->m_hWnd = hWnd;

		// see http://pluralsight.com/blogs/keith/archive/2005/01/13/4940.aspx
#pragma warning(disable:4244)
		SetWindowLongPtr(hWnd, 0, (LONG_PTR) pPropertyCtrl);
#pragma warning(default:4244)

		return axl_win_TPropertyCtrl_OnCreate(pPropertyCtrl);
	}

	pPropertyCtrl = axl_win_TPropertyCtrl_FromHwnd(hWnd);
	if (!pPropertyCtrl) // control was detached
		return DefWindowProc(hWnd, Msg, wParam, lParam);

	if (Msg >= WM_MOUSEFIRST && Msg <= WM_MOUSELAST)
	{
		MSG MsgStruct = { hWnd, Msg, wParam, lParam };
		SendMessage(pPropertyCtrl->m_hWndToolTip, TTM_RELAYEVENT, 0, (LPARAM) &MsgStruct);
	}

	if (Msg >= WM_NCMOUSEFIRST && Msg <= WM_NCMOUSELAST)
		SendMessage(pPropertyCtrl->m_hWndToolTip, TTM_ACTIVATE, false, 0);

	switch (Msg)
	{
	case WM_MOUSEACTIVATE:
		lResult = axl_win_TPropertyCtrl_IsMenu(pPropertyCtrl) ? MA_NOACTIVATE : DefWindowProc(hWnd, Msg, wParam, lParam);
		break;

	case WM_DESTROY:
		lResult = axl_win_TPropertyCtrl_OnDestroy(pPropertyCtrl);
		break;

	case WM_GETDLGCODE:
		lResult = DLGC_WANTTAB | DLGC_WANTARROWS | DLGC_WANTCHARS;
		break;

	case WM_SETFOCUS:
		lResult = axl_win_TPropertyCtrl_OnSetFocus(pPropertyCtrl);
		break;

	case WM_KILLFOCUS:
		lResult = axl_win_TPropertyCtrl_OnKillFocus(pPropertyCtrl);
		break;

	case WM_ERASEBKGND:
		break;

	case WM_PAINT:
		lResult = axl_win_TPropertyCtrl_OnPaint(pPropertyCtrl);
		break;

    case WM_THEMECHANGED:
		axl_gr_TStockCtrlPaint_CloseThemes(&pPropertyCtrl->m_StockCtrlPaint);
		break;

	case WM_NCPAINT:
		axl_gr_TStockCtrlPaint_NcPaintEdge(&pPropertyCtrl->m_StockCtrlPaint, hWnd, (HRGN) wParam);
		break;

	case WM_SIZE:
		lResult = axl_win_TPropertyCtrl_OnSize(pPropertyCtrl);
		break;

	case WM_HSCROLL:
		axl_win_TPropertyCtrl_OnScroll(pPropertyCtrl, SB_HORZ, LOWORD(wParam));
		break;

	case WM_VSCROLL:
		axl_win_TPropertyCtrl_OnScroll(pPropertyCtrl, SB_VERT, LOWORD(wParam));
		break;

	case WM_SETCURSOR:
		lResult = axl_win_TPropertyCtrl_OnSetCursor(pPropertyCtrl, wParam, lParam);
		break;

	case WM_LBUTTONDOWN:
		lResult = axl_win_TPropertyCtrl_OnLButtonDown(pPropertyCtrl, (short) LOWORD(lParam), (short) HIWORD(lParam));
		break;

	case WM_LBUTTONUP:
		lResult = axl_win_TPropertyCtrl_OnLButtonUp(pPropertyCtrl, (short) LOWORD(lParam), (short) HIWORD(lParam));
		break;

	case WM_LBUTTONDBLCLK:
		lResult = axl_win_TPropertyCtrl_OnLButtonDblClk(pPropertyCtrl, (short) LOWORD(lParam), (short) HIWORD(lParam));
		break;		

	case WM_RBUTTONDOWN:
		lResult = axl_win_TPropertyCtrl_OnRButtonDown(pPropertyCtrl, (short) LOWORD(lParam), (short) HIWORD(lParam));
		break;

	case WM_RBUTTONUP:
		lResult = axl_win_TPropertyCtrl_OnRButtonUp(pPropertyCtrl, (short) LOWORD(lParam), (short) HIWORD(lParam));
		break;

	case WM_MOUSEMOVE:
		lResult = axl_win_TPropertyCtrl_OnMouseMove(pPropertyCtrl, (short) LOWORD(lParam), (short) HIWORD(lParam));
		break;

	case WM_MOUSELEAVE:
		lResult = axl_win_TPropertyCtrl_OnMouseLeave(pPropertyCtrl);
		break;

	case WM_MOUSEWHEEL:
		lResult = axl_win_TPropertyCtrl_OnMouseWheel(pPropertyCtrl, HIWORD(wParam));
		break;

	case WM_CAPTURECHANGED:
		axl_win_TPropertyCtrl_StopDrag(pPropertyCtrl);
		break;

	case WM_KEYDOWN:
		axl_win_TPropertyCtrl_OnKeyDown(pPropertyCtrl, (int) wParam);
		break;

	case WM_KEYUP:
		axl_win_TPropertyCtrl_OnKeyUp(pPropertyCtrl, (int) wParam);
		break;

	case WM_TIMER:
		lResult = axl_win_TPropertyCtrl_OnTimer(pPropertyCtrl, (int) wParam);
		break;

	default:
		lResult = DefWindowProc(hWnd, Msg, wParam, lParam);
	}
 
	return lResult;
}


//.............................................................................
