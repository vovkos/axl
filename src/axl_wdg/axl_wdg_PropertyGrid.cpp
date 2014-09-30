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
	axl_win_TPropertyInPlace* inPlace,
	axl_obj_TAgent* agent
	)
{
	axl_obj_TObject_Construct(inPlace, agent);
	inPlace->m_pfLoad = NULL;
	inPlace->m_pfSave = NULL;
	inPlace->m_pfLayout = NULL;
	inPlace->m_pfSetFocus = NULL;
}

//.............................................................................

// forwards

LRESULT 
CALLBACK 
axl_win_TPropertyCtrl_WndProc(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
	);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
AXL_API
axl_win_TPropertyCtrl_CalcCenteredRect(
	axl_win_TPropertyCtrl* propertyCtrl,
	SIZE* size,
	long width,
	RECT* rect
	)
{
	rect->left = (width - size->cx) / 2;
	rect->top = (propertyCtrl->m_lineHeight - size->cy) / 2;
	rect->right = rect->left + size->cx;
	rect->bottom = rect->top + size->cy;
}

bool_t
AXL_API
axl_win_TPropertyCtrl_CalcLineHeight(axl_win_TPropertyCtrl* propertyCtrl)
{
	ulong_t height = max(propertyCtrl->m_charSize.cy, propertyCtrl->m_iconSize.cy);
	ulong_t mod;

	height += propertyCtrl->m_margins.top + propertyCtrl->m_margins.bottom;	
	mod = height % 4;

	height = mod ? height + 4 - mod : height;

	if (propertyCtrl->m_lineHeight == height)
		return false;

	propertyCtrl->m_lineHeight = height;
	propertyCtrl->m_textOrigin.x = propertyCtrl->m_margins.left;
	propertyCtrl->m_textOrigin.y = (propertyCtrl->m_lineHeight - propertyCtrl->m_charSize.cy) / 2;

	axl_win_TPropertyCtrl_CalcCenteredRect(propertyCtrl, &propertyCtrl->m_plusMinusSize, propertyCtrl->m_indentWidth, &propertyCtrl->m_plusMinusRect);
	axl_win_TPropertyCtrl_CalcCenteredRect(propertyCtrl, &propertyCtrl->m_optionBoxSize, propertyCtrl->m_optionBoxWidth, &propertyCtrl->m_optionBoxRect);
	axl_win_TPropertyCtrl_CalcCenteredRect(propertyCtrl, &propertyCtrl->m_iconSize, propertyCtrl->m_iconWidth, &propertyCtrl->m_iconRect);

	// v-gridline is at odd x, but indent is even, therefore we must shift plusminus rect if we want it to match v-gridline

	propertyCtrl->m_plusMinusRect.left++;
	propertyCtrl->m_plusMinusRect.right++;

	return true;
}

void
AXL_API
axl_win_TPropertyCtrl_Construct(axl_win_TPropertyCtrl* propertyCtrl)
{
	propertyCtrl->m_hWnd = NULL;
	propertyCtrl->m_hWndToolTip = NULL;
//	pPropertyCtrl->m_hUpdateRgn = CreateRectRgn(0, 0, 0, 0);
	propertyCtrl->m_hWndNotify = NULL;
	propertyCtrl->m_idNotify = 0;

	propertyCtrl->m_toolTipShowDelay = 1000;  // 1 sec initial
	propertyCtrl->m_toolTipReShowDelay = 200; // 200 msec reshow

	propertyCtrl->m_menuPropertyCtrl = NULL;
	propertyCtrl->m_menuParentPropertyCtrl = NULL;
	propertyCtrl->m_menuRootPropertyCtrl = NULL;
	propertyCtrl->m_menuAnchorLine = NULL;
	propertyCtrl->m_menuDelay = 500;

	propertyCtrl->m_hImageList = NULL;
	propertyCtrl->m_hArrowCursor = loadCursor(NULL, IDC_ARROW);
	propertyCtrl->m_hSplitterCursor = loadCursor(NULL, IDC_SIZEWE);
	propertyCtrl->m_hReArrangeCursor = loadCursor(NULL, IDC_SIZENS);
	propertyCtrl->m_hHyperlinkCursor = loadCursor(NULL, IDC_HAND);	

	propertyCtrl->m_selectedLine = -1;
	propertyCtrl->m_firstVisibleLine = 0;
	propertyCtrl->m_firstVisibleX = 0;
	propertyCtrl->m_visibleWidth = 0;
	propertyCtrl->m_fullyVisibleLineCount = 0;
	propertyCtrl->m_visibleLineCount = 0;
	propertyCtrl->m_splitterPos = 200;
	propertyCtrl->m_minNamePaneWidth = 50;

	propertyCtrl->m_drag = axl_win_EPropertyDrag_None;
	propertyCtrl->m_dragAnchorHitTest = axl_win_EPropertyHitTest_None;
	propertyCtrl->m_dragAnchorLine = NULL;
	propertyCtrl->m_dragDelta.x = 0;
	propertyCtrl->m_dragDelta.y = 0;

	propertyCtrl->m_hover = axl_win_EPropertyHover_None;
	propertyCtrl->m_hoverAnchorLine = NULL;
	propertyCtrl->m_lastTooltipLine = NULL;

	propertyCtrl->m_margins.left = 5;
	propertyCtrl->m_margins.top = 5;
	propertyCtrl->m_margins.right = 5;
	propertyCtrl->m_margins.bottom = 5;

	propertyCtrl->m_plusMinusSize.cx = 9;
	propertyCtrl->m_plusMinusSize.cy = 9;
	propertyCtrl->m_optionBoxSize.cx = 13;
	propertyCtrl->m_optionBoxSize.cy = 13;
	propertyCtrl->m_optionBoxWidth = propertyCtrl->m_optionBoxSize.cx + propertyCtrl->m_margins.left + propertyCtrl->m_margins.right;

	propertyCtrl->m_iconSize.cx = 0;
	propertyCtrl->m_iconSize.cy = 0;
	propertyCtrl->m_iconWidth = 0;
	memset(&propertyCtrl->m_iconRect, 0, sizeof(RECT));

	propertyCtrl->m_splitterProximityRect.left = -2;
	propertyCtrl->m_splitterProximityRect.top = -2;
	propertyCtrl->m_splitterProximityRect.right = 2;
	propertyCtrl->m_splitterProximityRect.bottom = 2;

	propertyCtrl->m_indentWidth = propertyCtrl->m_optionBoxWidth & ~1;  // indent width must be even
	propertyCtrl->m_hasGridLinesAtRoot = true;
	propertyCtrl->m_doAlwaysDrawButtons = false;

	axl_gr_TFont_Construct(&propertyCtrl->m_defaultFont);
	axl_gr_TFont_Attach(&propertyCtrl->m_defaultFont, getStockObject(DEFAULT_GUI_FONT));
	
	propertyCtrl->m_charSize = axl_gr_TFont_CalcCharSize(&propertyCtrl->m_defaultFont, '|');

	axl_win_TPropertyCtrl_CalcLineHeight(propertyCtrl);

	propertyCtrl->m_singlePaneWidth = 0;
	propertyCtrl->m_valueWidth = 0;

	propertyCtrl->m_mouseWheelSpeed = 3;

	axl_rtl_TList_Construct(&propertyCtrl->m_lineList);
	axl_rtl_TPtrArray_Construct(&propertyCtrl->m_visibleLineArray);

	propertyCtrl->m_lineList = &propertyCtrl->m_lineList;
	propertyCtrl->m_activeInPlace = NULL;
	propertyCtrl->m_activeInPlaceMinWidth = 0;

	memcpy(propertyCtrl->m_palette, g_axl_gr_StdPalette, sizeof(g_axl_gr_StdPalette));
	propertyCtrl->m_palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_Name)]            = getSysColor(COLOR_WINDOWTEXT);
	propertyCtrl->m_palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_NameBack)]        = getSysColor(COLOR_BTNFACE);
	propertyCtrl->m_palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_DisabledText)]    = getSysColor(COLOR_GRAYTEXT);	
	propertyCtrl->m_palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_Value)]           = RGB(0,0,255);
	propertyCtrl->m_palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_ValueBack)]       = getSysColor(COLOR_BTNFACE);
	propertyCtrl->m_palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_SelectedName)]    = getSysColor(COLOR_HIGHLIGHTTEXT);
	propertyCtrl->m_palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_SelectedValue)]   = RGB(255, 255, 0);
	propertyCtrl->m_palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_SelectedBack)]    = getSysColor(COLOR_HIGHLIGHT);
	propertyCtrl->m_palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_ButtonText)]      = getSysColor(COLOR_WINDOWTEXT);
	propertyCtrl->m_palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_HoverButtonText)] = getSysColor(COLOR_WINDOWTEXT);
	propertyCtrl->m_palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_Empty)]           = getSysColor(COLOR_WINDOW);
	propertyCtrl->m_palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_NameDelimiter)]   = getSysColor(COLOR_BTNSHADOW);
	propertyCtrl->m_palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_ValueDelimiter)]  = getSysColor(COLOR_BTNSHADOW);
	propertyCtrl->m_palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_Splitter)]        = getSysColor(COLOR_BTNSHADOW);
	propertyCtrl->m_palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_Grid)]            = getSysColor(COLOR_BTNSHADOW);
	propertyCtrl->m_palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_PlusMinus)]       = RGB(0, 0, 0);
	propertyCtrl->m_palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_PlusMinusRect)]   = RGB(128, 128, 128);
	propertyCtrl->m_palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_PlusMinusBack)]   = RGB(255, 255, 255);
	propertyCtrl->m_palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_Unused)]          = RGB(255, 0, 255);

#ifdef _AXL_USE_OFFSCREEN_BUFFER
	axl_gr_TOffscreenBuffer_Construct(&propertyCtrl->m_offscreenBuffer);
#endif

	axl_gr_TStockCtrlPaint_Construct(&propertyCtrl->m_stockCtrlPaint);
	axl_gr_TTextPaint_Construct(&propertyCtrl->m_textPaint);
	axl_gr_TFontMod_SetBaseFont(&propertyCtrl->m_textPaint.m_font, propertyCtrl->m_defaultFont.m_hFont);

	propertyCtrl->m_textPaint.m_palette = propertyCtrl->m_palette;
	propertyCtrl->m_textPaint.m_paletteSize = axl_win_EPropertyColor_Count;

	propertyCtrl->m_hPlusMinusImageList = axl_win_TPropertyCtrl_CreatePlusMinusImageList(propertyCtrl->m_plusMinusSize.cx, propertyCtrl->m_plusMinusSize.cy, propertyCtrl->m_palette);
	propertyCtrl->m_hGridImageList = axl_win_TPropertyCtrl_CreateGridImageList(propertyCtrl->m_indentWidth, propertyCtrl->m_lineHeight, propertyCtrl->m_palette);
	propertyCtrl->m_hMenuArrowImageList = axl_win_TPropertyCtrl_CreateMenuArrowImageList();
}

void
AXL_API
axl_win_TPropertyCtrl_Destruct(axl_win_TPropertyCtrl* propertyCtrl)
{
	axl_win_TPropertyLine_ClearLineList(&propertyCtrl->m_lineList);
	axl_rtl_TPtrArray_Destruct(&propertyCtrl->m_visibleLineArray);
	axl_gr_TFont_Destruct(&propertyCtrl->m_defaultFont);

//	DeleteObject(pPropertyCtrl->m_hUpdateRgn);

	if (propertyCtrl->m_menuPropertyCtrl)
	{
		axl_win_TPropertyCtrl_Destruct(propertyCtrl->m_menuPropertyCtrl);
		axl_mem_Free(propertyCtrl->m_menuPropertyCtrl);
	}

#ifdef _AXL_USE_OFFSCREEN_BUFFER
	axl_gr_TOffscreenBuffer_Destruct(&propertyCtrl->m_offscreenBuffer);
#endif

	axl_gr_TStockCtrlPaint_Destruct(&propertyCtrl->m_stockCtrlPaint);
	axl_gr_TTextPaint_Destruct(&propertyCtrl->m_textPaint);

	imageList_Destroy(propertyCtrl->m_hPlusMinusImageList);
	imageList_Destroy(propertyCtrl->m_hGridImageList);
	imageList_Destroy(propertyCtrl->m_hMenuArrowImageList);
}

LRESULT
AXL_API
axl_win_TPropertyCtrl_Notify(
	axl_win_TPropertyCtrl* propertyCtrl,
	int code,
	axl_win_TPropertyHitTest* hitTest
	)
{
	axl_win_TPropertyNotify notify;
	
	propertyCtrl = axl_win_TPropertyCtrl_GetMenuRootPropertyCtrl(propertyCtrl);

	notify.m_hdr.code = code;
	notify.m_hdr.hwndFrom = propertyCtrl->m_hWndNotify;
	notify.m_hdr.idFrom = propertyCtrl->m_hWndNotify ? propertyCtrl->m_idNotify : getDlgCtrlID(propertyCtrl->m_hWnd);
	notify.m_hitTest = *hitTest;	

	return propertyCtrl->m_hWndNotify ?
		sendMessage(propertyCtrl->m_hWndNotify, WM_NOTIFY, notify.m_hdr.idFrom, (LPARAM) &notify):
		sendMessage(getParent(propertyCtrl->m_hWnd), WM_NOTIFY, notify.m_hdr.idFrom, (LPARAM) &notify);
}


void 
AXL_API
axl_win_TPropertyCtrl_SetImageList(
	axl_win_TPropertyCtrl* propertyCtrl,
	HIMAGELIST hImageList
	)
{
	bool_t hasChanged;

	if (!hImageList)
	{
		propertyCtrl->m_iconSize.cx = 0;
		propertyCtrl->m_iconSize.cy = 0;
		propertyCtrl->m_iconWidth = 0;
		memset(&propertyCtrl->m_iconRect, 0, sizeof(RECT));
	}
	else
	{
		int cx, cy;
		imageList_GetIconSize(hImageList, &cx, &cy);

		propertyCtrl->m_iconSize.cx = cx;
		propertyCtrl->m_iconSize.cy = cy;
		propertyCtrl->m_iconWidth = cx + propertyCtrl->m_margins.left + propertyCtrl->m_margins.right;
		axl_win_TPropertyCtrl_CalcCenteredRect(propertyCtrl, &propertyCtrl->m_iconSize, propertyCtrl->m_iconWidth, &propertyCtrl->m_iconRect);
	}

	propertyCtrl->m_hImageList = hImageList;

	hasChanged = axl_win_TPropertyCtrl_CalcLineHeight(propertyCtrl);
	
	if (hasChanged)
	{
		imageList_Destroy(propertyCtrl->m_hGridImageList);
		propertyCtrl->m_hGridImageList = axl_win_TPropertyCtrl_CreateGridImageList(propertyCtrl->m_indentWidth, propertyCtrl->m_lineHeight, propertyCtrl->m_palette);
	}

	axl_win_TPropertyCtrl_Invalidate(propertyCtrl);
}

bool_t
AXL_API
axl_win_TPropertyCtrl_RegisterWndClass(HINSTANCE hInstance)
{
	BOOL result;
	WNDCLASS wndClass;

	static bool_t _IsRegistered = false;
	if (_IsRegistered)
		return true;

	memset(&wndClass, 0, sizeof(wndClass));
	wndClass.style = CS_DBLCLKS;
    wndClass.lpfWndProc = axl_win_TPropertyCtrl_WndProc;
    wndClass.cbWndExtra = sizeof(axl_win_TPropertyCtrl*);
    wndClass.hInstance = hInstance;
	wndClass.hCursor = loadCursor(NULL, IDC_ARROW);
    wndClass.lpszClassName = axl_win_TPropertyCtrl_WndClassName;

	result = registerClass(&wndClass);
	if (!result)
		return false;

	_IsRegistered = true;
	return true;
}

HWND
AXL_API
axl_win_TPropertyCtrl_CreateEx(
	axl_win_TPropertyCtrl* propertyCtrl,
	DWORD exStyle,
	DWORD style,
	const RECT* rect,
	HWND hWndParent,
	UINT ID,
	HINSTANCE hInstance
	)
{
	bool_t result;
	
	static RECT _NullRect = {0};

	if (!rect)
		rect = &_NullRect;

	if (propertyCtrl->m_hWnd)
		return NULL;

	result = axl_win_TPropertyCtrl_RegisterWndClass(hInstance);
	if (!result)
		return false;

	style |= WS_CLIPCHILDREN;

	createWindowEx(
		exStyle, 
		axl_win_TPropertyCtrl_WndClassName, NULL, 
		style, 
		rect->left, rect->top, 
		rect->right - rect->left, 
		rect->bottom - rect->top,
		hWndParent, (HMENU)(uintptr_t) ID,
		hInstance,
		propertyCtrl
		);

	return propertyCtrl->m_hWnd;
}

void
AXL_API
axl_win_TPropertyCtrl_Destroy(axl_win_TPropertyCtrl* propertyCtrl)
{
	if (!propertyCtrl->m_hWnd)
		return;

	destroyWindow(propertyCtrl->m_hWnd);
	propertyCtrl->m_hWnd = NULL;
}

axl_win_TPropertyLine*
AXL_API
axl_win_TPropertyCtrl_GetSelectedLine(axl_win_TPropertyCtrl* propertyCtrl)
{
	if (propertyCtrl->m_selectedLine == -1)
		return NULL;

	ASSERT(propertyCtrl->m_selectedLine < axl_rtl_TPtrArray_GetCount(&propertyCtrl->m_visibleLineArray));

	return ((axl_win_TPropertyLine**) propertyCtrl->m_visibleLineArray.m_p)[propertyCtrl->m_selectedLine]; 
}

axl_win_TPropertyLine* 
AXL_API
axl_win_TPropertyCtrl_HitTest(
	axl_win_TPropertyCtrl* propertyCtrl,
	long x,
	long y,	
	axl_win_TPropertyHitTest* hitTest
	)
{
	long _x;

	axl_win_TPropertyLine* line;

	size_t lineCount = axl_rtl_TPtrArray_GetCount(&propertyCtrl->m_visibleLineArray);
	size_t line = y / propertyCtrl->m_lineHeight + propertyCtrl->m_firstVisibleLine;

	long indentWidth;

	hitTest->m_hitTest = axl_win_EPropertyHitTest_None;
	hitTest->m_line = NULL; 
	hitTest->m_hyperlink = NULL; 

	if (line >= lineCount)
		return NULL;

	x += propertyCtrl->m_firstVisibleX;
	y %= propertyCtrl->m_lineHeight;

	line = ((axl_win_TPropertyLine**) propertyCtrl->m_visibleLineArray.m_p)[line];
	hitTest->m_line = line;

	if (!axl_win_TPropertyLine_IsSinglePane(line))
	{
		if (line == propertyCtrl->m_selectedLine && propertyCtrl->m_activeInPlace)
		{
			// in this case, no proximity from the right

			if (x >= propertyCtrl->m_splitterPos + propertyCtrl->m_splitterProximityRect.left && x <= propertyCtrl->m_splitterPos)
			{
				hitTest->m_hitTest = axl_win_EPropertyHitTest_Splitter;
				return line;
			}
		}
		else
		{
			if (x >= propertyCtrl->m_splitterPos + propertyCtrl->m_splitterProximityRect.left && 
				x <= propertyCtrl->m_splitterPos + propertyCtrl->m_splitterProximityRect.right)
			{
				hitTest->m_hitTest = axl_win_EPropertyHitTest_Splitter;
				return line;
			}
		}

		if (x > propertyCtrl->m_splitterPos)
		{
			_x = x - propertyCtrl->m_splitterPos - propertyCtrl->m_margins.left;

			if (line->m_flags & axl_win_EPropertyLine_ValueHyperText && 
				y >= propertyCtrl->m_textOrigin.y && 
				y < propertyCtrl->m_textOrigin.y + propertyCtrl->m_charSize.cy)
			{
				axl_gr_THyperlinkAnchor* anchor = axl_gr_THyperText_FindHyperlinkByX(&line->m_valueHyperText, _x);
				if (anchor && !axl_rtl_TString_IsEmpty(&anchor->m_hyperlink))
				{
					hitTest->m_hyperlink = anchor;
					hitTest->m_hitTest = axl_win_EPropertyHitTest_Hyperlink;
					return line;
				}
			}

			hitTest->m_hitTest = axl_win_EPropertyHitTest_Value;
			return line;
		}
	}

	_x = x;

	if (!axl_win_TPropertyCtrl_IsMenu(propertyCtrl))
		_x -= line->m_level * propertyCtrl->m_indentWidth;

	if (axl_win_TPropertyLine_HasPlusMinus(line) &&
		_x >= propertyCtrl->m_plusMinusRect.left && _x < propertyCtrl->m_plusMinusRect.right && 
		y >= propertyCtrl->m_plusMinusRect.top && y < propertyCtrl->m_plusMinusRect.bottom)
	{
		hitTest->m_hitTest = axl_win_EPropertyHitTest_PlusMinus;
		return line;
	}

	indentWidth = (line->m_indent + 1) * propertyCtrl->m_indentWidth;

	if (line->m_parent && (line->m_parent->m_flags & axl_win_EPropertyLine_ReArrangeable) && 
		_x >= 0 && _x <= indentWidth &&
		y >= propertyCtrl->m_plusMinusRect.top && y < propertyCtrl->m_plusMinusRect.bottom)
	{
		hitTest->m_hitTest = axl_win_EPropertyHitTest_ReArrange;
		return line;
	}

	_x -= indentWidth;

	if (line->m_optionBoxType)
	{
		if (_x >= propertyCtrl->m_optionBoxRect.left && _x < (long) propertyCtrl->m_optionBoxWidth &&
			y >= propertyCtrl->m_optionBoxRect.top && y < propertyCtrl->m_optionBoxRect.bottom)
		{
			hitTest->m_hitTest = axl_win_EPropertyHitTest_OptionBox;
			return line;
		}

		_x -= propertyCtrl->m_optionBoxWidth;
	}

	if (_x < 0)
	{
		hitTest->m_hitTest = axl_win_EPropertyHitTest_Indent;
		return line;
	}

	if (line->m_flags & axl_win_EPropertyLine_Menu)
	{
		if (x < (long) line->m_menuWidth + propertyCtrl->m_margins.left + propertyCtrl->m_margins.right + getSystemMetrics(SM_CXVSCROLL))
		{
			hitTest->m_hitTest = axl_win_EPropertyHitTest_Menu;
			return line;
		}
	}

	if (line->m_flags & axl_win_EPropertyLine_Button)
	{
		if (x < (long) line->m_singlePaneWidth + propertyCtrl->m_margins.left + 2 * propertyCtrl->m_margins.right)
		{
			hitTest->m_hitTest = axl_win_EPropertyHitTest_Button;
			return line;
		}
	}

	if (line->m_icon != -1)
	{
		_x -= (long) propertyCtrl->m_iconWidth;
		if (_x < 0)
		{
			hitTest->m_hitTest = line->m_optionBoxType ? axl_win_EPropertyHitTest_OptionBox : axl_win_EPropertyHitTest_Icon;
			return line;
		}
	}

	_x -= propertyCtrl->m_margins.left;

	if (line->m_flags & axl_win_EPropertyLine_NameHyperText && 
		y >= propertyCtrl->m_textOrigin.y && 
		y < propertyCtrl->m_textOrigin.y + propertyCtrl->m_charSize.cy)
	{
		axl_gr_THyperlinkAnchor* anchor = axl_gr_THyperText_FindHyperlinkByX(&line->m_nameHyperText, _x);
		if (anchor && !axl_rtl_TString_IsEmpty(&anchor->m_hyperlink))
		{
			hitTest->m_hyperlink = anchor;
			hitTest->m_hitTest = axl_win_EPropertyHitTest_Hyperlink;
			return line;
		}
	}

	if (_x < (long) line->m_nameWidth)
		hitTest->m_hitTest = line->m_optionBoxType ? axl_win_EPropertyHitTest_OptionBox : axl_win_EPropertyHitTest_Name;
	else
		hitTest->m_hitTest = axl_win_EPropertyHitTest_NameTail;

	return line;
}

long
AXL_API
axl_win_TPropertyCtrl_GetFullWidth(axl_win_TPropertyCtrl* propertyCtrl)
{
	ulong_t valueWidth = propertyCtrl->m_valueWidth + propertyCtrl->m_margins.left + propertyCtrl->m_splitterPos + 1;
	ulong_t singlePaneWidth = propertyCtrl->m_singlePaneWidth + propertyCtrl->m_margins.left;
	ulong_t inPlaceMinWidth = propertyCtrl->m_activeInPlace ? propertyCtrl->m_activeInPlaceMinWidth + propertyCtrl->m_splitterPos + 1: 0;
	ulong_t width = max(valueWidth, max(singlePaneWidth, inPlaceMinWidth));
	width += propertyCtrl->m_margins.right;
	return width;
}

long
AXL_API
axl_win_TPropertyCtrl_GetFullHeight(axl_win_TPropertyCtrl* propertyCtrl)
{
	size_t lineCount = axl_rtl_TPtrArray_GetCount(&propertyCtrl->m_visibleLineArray);
	return (long) lineCount * propertyCtrl->m_lineHeight;
}

void
AXL_API
axl_win_TPropertyCtrl_GetSize(
	axl_win_TPropertyCtrl* propertyCtrl,
	SIZE* size
	)
{
	size->cx = axl_win_TPropertyCtrl_GetFullWidth(propertyCtrl);
	size->cy = axl_win_TPropertyCtrl_GetFullHeight(propertyCtrl);
}

void
AXL_API
axl_win_TPropertyCtrl_GetInPlaceRect(
	axl_win_TPropertyCtrl* propertyCtrl,
	RECT* rect
	)
{
	ASSERT(propertyCtrl->m_selectedLine != -1);

	getClientRect(propertyCtrl->m_hWnd, rect);

	rect->left = propertyCtrl->m_splitterPos - propertyCtrl->m_firstVisibleX + 1;
	rect->top = (long) (propertyCtrl->m_selectedLine - propertyCtrl->m_firstVisibleLine) * propertyCtrl->m_lineHeight;
	rect->bottom = rect->top + propertyCtrl->m_lineHeight;
	
	rect->top--;
}

void
AXL_API
axl_win_TPropertyCtrl_GetMenuPos(
	axl_win_TPropertyCtrl* propertyCtrl,
	const SIZE* size,
	POINT* pos
	)
{
	POINT referencePoint = { 0, 0 };
	SIZE screenSize = { getSystemMetrics(SM_CXSCREEN), getSystemMetrics(SM_CYSCREEN) };

	ASSERT(propertyCtrl->m_menuAnchorLine && propertyCtrl->m_menuAnchorLine->m_visibleIndex != -1);

	clientToScreen(propertyCtrl->m_hWnd, &referencePoint);

	pos->x = propertyCtrl->m_menuAnchorLine->m_menuWidth + propertyCtrl->m_margins.left + propertyCtrl->m_margins.right;
	pos->x += getSystemMetrics(SM_CXVSCROLL);

	if (referencePoint.x + pos->x + size->cx > screenSize.cx)
		pos->x = -size->cx;

	pos->y = (long) (propertyCtrl->m_menuAnchorLine->m_visibleIndex - propertyCtrl->m_firstVisibleLine) * propertyCtrl->m_lineHeight;
	pos->y -= getSystemMetrics(SM_CYBORDER);

	if (referencePoint.y + pos->y + size->cy > screenSize.cy)
	{
		long delta = referencePoint.y + pos->y + size->cy - screenSize.cy;
		pos->y -= delta;
	}

	clientToScreen(propertyCtrl->m_hWnd, pos);
}

bool_t
AXL_API
axl_win_TPropertyCtrl_FixupFirstVisibleLine(axl_win_TPropertyCtrl* propertyCtrl)
{
	size_t lineCount = axl_rtl_TPtrArray_GetCount(&propertyCtrl->m_visibleLineArray);
	size_t newFirstVisibleLine = propertyCtrl->m_firstVisibleLine;

	if (propertyCtrl->m_firstVisibleLine + propertyCtrl->m_fullyVisibleLineCount > lineCount)
	{
		if (lineCount > propertyCtrl->m_fullyVisibleLineCount)
			newFirstVisibleLine = lineCount - propertyCtrl->m_fullyVisibleLineCount;
		else
			newFirstVisibleLine = 0;
	}

	if (newFirstVisibleLine == propertyCtrl->m_firstVisibleLine)
		return false;

	propertyCtrl->m_firstVisibleLine = newFirstVisibleLine;
	return true;
}

bool_t
AXL_API
axl_win_TPropertyCtrl_FixupFirstVisibleX(axl_win_TPropertyCtrl* propertyCtrl)
{
	ulong_t newFirstVisibleX = propertyCtrl->m_firstVisibleX;
	ulong_t width = axl_win_TPropertyCtrl_GetFullWidth(propertyCtrl);

	if (propertyCtrl->m_firstVisibleX + propertyCtrl->m_visibleWidth > width)
	{
		if (width > propertyCtrl->m_visibleWidth)
			newFirstVisibleX = width - propertyCtrl->m_visibleWidth;
		else
			newFirstVisibleX = 0;
	}

	if (newFirstVisibleX == propertyCtrl->m_firstVisibleX)
		return false;

	propertyCtrl->m_firstVisibleX = newFirstVisibleX;
	return true;	
}

void 
AXL_API
axl_win_TPropertyCtrl_RecalcHScroll(axl_win_TPropertyCtrl* propertyCtrl)
{
	SCROLLINFO scrollInfo = { sizeof(scrollInfo) };
	scrollInfo.fMask = SIF_ALL;
	scrollInfo.nMin  = 0;
	scrollInfo.nMax  = axl_win_TPropertyCtrl_GetFullWidth(propertyCtrl) - 1;
    scrollInfo.nPage = propertyCtrl->m_visibleWidth;
    scrollInfo.nPos  = propertyCtrl->m_firstVisibleX;
	setScrollInfo(propertyCtrl->m_hWnd, SB_HORZ, &scrollInfo, TRUE);
}

void 
AXL_API
axl_win_TPropertyCtrl_RecalcVScroll(axl_win_TPropertyCtrl* propertyCtrl)
{
	size_t lineCount = axl_rtl_TPtrArray_GetCount(&propertyCtrl->m_visibleLineArray);

	SCROLLINFO scrollInfo = { sizeof(scrollInfo) };
	scrollInfo.fMask = SIF_ALL;
	scrollInfo.nMin  = 0;
    scrollInfo.nMax  = (long) lineCount - 1;
    scrollInfo.nPage = (long) propertyCtrl->m_fullyVisibleLineCount;
	scrollInfo.nPos  = (long) propertyCtrl->m_firstVisibleLine;
	setScrollInfo(propertyCtrl->m_hWnd, SB_VERT, &scrollInfo, TRUE);
}

bool_t
AXL_API
axl_win_TPropertyCtrl_CalcWidth(
	axl_win_TPropertyCtrl* propertyCtrl,
	int flags
	)
{
	bool_t hasChanged = false;

	axl_win_TPropertyLine* line;

	ulong_t maxSinglePaneWidth = 0;
	ulong_t maxValueWidth = 0;

	line = (axl_win_TPropertyLine*) axl_rtl_TList_GetHead(propertyCtrl->m_lineList);
	while (line)
	{
		ulong_t singlePaneWidth = 0;
		ulong_t valueWidth = 0;

		if (flags & axl_win_EPropertyCalcWidth_Recursive)
			axl_win_TPropertyLine_CalcWidth(line, flags);

		if (line->m_flags & axl_win_EPropertyLine_IsExpanded)
		{
			singlePaneWidth = max(line->m_childrenSinglePaneWidth, line->m_singlePaneWidth);
			valueWidth = max(line->m_childrenValueWidth, line->m_valueWidth);
		}
		else
		{
			singlePaneWidth = line->m_singlePaneWidth;
			valueWidth = line->m_valueWidth;
		}

		if (singlePaneWidth > maxSinglePaneWidth)
			maxSinglePaneWidth = singlePaneWidth;

		if (valueWidth > maxValueWidth)
			maxValueWidth = valueWidth;

		line = (axl_win_TPropertyLine*) axl_rtl_TList_GetNext(propertyCtrl->m_lineList, (axl_rtl_TListEntry*) line);
	}

	if (maxValueWidth != propertyCtrl->m_valueWidth || maxSinglePaneWidth != propertyCtrl->m_singlePaneWidth)
		hasChanged = true;

	propertyCtrl->m_valueWidth = maxValueWidth;
	propertyCtrl->m_singlePaneWidth = maxSinglePaneWidth;

	return hasChanged;
}

void 
AXL_API
axl_win_TPropertyCtrl_UpdateLineWidth(
	axl_win_TPropertyCtrl* propertyCtrl,
	axl_win_TPropertyLine* line
	)
{
	bool_t hasChanged = axl_win_TPropertyLine_CalcWidth(line, axl_win_EPropertyCalcWidth_Line | axl_win_EPropertyCalcWidth_Parent);
	if (!hasChanged)
		return;

	hasChanged = axl_win_TPropertyCtrl_CalcWidth(propertyCtrl, 0);
	if (!hasChanged)
		return;

	axl_win_TPropertyCtrl_RecalcHScroll(propertyCtrl);
}

void 
AXL_API
axl_win_TPropertyCtrl_UpdateAllLineWidths(axl_win_TPropertyCtrl* propertyCtrl)
{
	axl_win_TPropertyCtrl_CalcWidth(propertyCtrl, axl_win_EPropertyCalcWidth_Line | axl_win_EPropertyCalcWidth_Children | axl_win_EPropertyCalcWidth_Recursive);
	axl_win_TPropertyCtrl_CalcWidth(propertyCtrl, axl_win_EPropertyCalcWidth_Menu | axl_win_EPropertyCalcWidth_Children | axl_win_EPropertyCalcWidth_Recursive);
	axl_win_TPropertyCtrl_RecalcHScroll(propertyCtrl);
}

void
AXL_API
axl_win_TPropertyCtrl_ReArrangeVisibleLines(
	axl_win_TPropertyCtrl* propertyCtrl,
	size_t from,
	size_t to
	)
{
	size_t totalCount = axl_rtl_TPtrArray_GetCount(&propertyCtrl->m_visibleLineArray);

	axl_win_TPropertyLine** base = (axl_win_TPropertyLine**) propertyCtrl->m_visibleLineArray.m_p;
	axl_win_TPropertyLine* from = base[from];
	
	size_t i;
	size_t end;
	axl_win_TPropertyLine** pp;
	
	ASSERT(from < totalCount);
	ASSERT(to < totalCount);

	if (from == to)
		return;

	if (from < to)
	{
		memmove(base + from, base + from + 1, (to - from) * sizeof(void*));

		i = from;
		end = to + 1;
	}
	else
	{
		memmove(base + to + 1, base + to, (from - to) * sizeof(void*));

		i = to;
		end = from + 1;
	}

	base[to] = from;

	pp = base + i;
	for (; i < end; i++, pp++)
		(*pp)->m_visibleIndex = i;
}


axl_win_TPropertyLine**
AXL_API
axl_win_TPropertyCtrl_InsertVisibleLines(
	axl_win_TPropertyCtrl* propertyCtrl,
	size_t i,
	size_t count
	)
{
	axl_win_TPropertyLine** anchor;
	axl_win_TPropertyLine** pp;
	axl_win_TPropertyLine** end;

	axl_rtl_TPtrArray_Insert(&propertyCtrl->m_visibleLineArray, i, NULL, count);

	anchor = (axl_win_TPropertyLine**) propertyCtrl->m_visibleLineArray.m_p + i;
	end = (axl_win_TPropertyLine**) propertyCtrl->m_visibleLineArray.m_p + axl_rtl_TPtrArray_GetCount(&propertyCtrl->m_visibleLineArray);

	pp = anchor + count;

	for (; pp < end; pp++)
	{
		axl_win_TPropertyLine* line = *pp;
		ASSERT(line);
		line->m_visibleIndex += count;
	}

	return anchor;
}

void
AXL_API
axl_win_TPropertyCtrl_DeleteVisibleLines(
	axl_win_TPropertyCtrl* propertyCtrl,
	size_t i,
	size_t count
	)
{
	axl_win_TPropertyLine** pp;
	axl_win_TPropertyLine** end;

	// mark deleted lines as invisible

	pp = (axl_win_TPropertyLine**) propertyCtrl->m_visibleLineArray.m_p + i;
    end = pp + count;

	for (; pp < end; pp++)
	{
		axl_win_TPropertyLine* line = *pp;
		ASSERT(line);
		line->m_visibleIndex = -1;
	}

	// shift remaining visible lines

	end = (axl_win_TPropertyLine**) propertyCtrl->m_visibleLineArray.m_p + axl_rtl_TPtrArray_GetCount(&propertyCtrl->m_visibleLineArray);

	for (; pp < end; pp++)
	{
		axl_win_TPropertyLine* line = *pp;
		ASSERT(line);
		line->m_visibleIndex -= count;
	}

	axl_rtl_TPtrArray_Remove(&propertyCtrl->m_visibleLineArray, i, count);
}

axl_win_TPropertyLine*
AXL_API
axl_win_TPropertyCtrl_InsertLineEx(
	axl_win_TPropertyCtrl* propertyCtrl,
	axl_win_TPropertyLine* parent,
	axl_win_TPropertyLine* before,
	axl_win_TPropertyLine* source
	)
{
	size_t visibleIndex = -1;

	axl_win_TPropertyLine* line = axl_mem_Allocate(sizeof(axl_win_TPropertyLine));
	axl_win_TPropertyLine_Construct(line);

	line->m_propertyCtrl = propertyCtrl;

	if (!parent)
	{
		axl_rtl_TList_InsertBefore(&propertyCtrl->m_lineList, (axl_rtl_TListEntry*) line, (axl_rtl_TListEntry*) before);

		visibleIndex = before ? 
			before->m_visibleIndex != -1 ? before->m_visibleIndex : -1 :
			axl_rtl_TPtrArray_GetCount(&propertyCtrl->m_visibleLineArray);
	}
	else
	{
		line->m_parent = parent;	
		line->m_level = parent->m_level + parent->m_indent + 1;
		line->m_flags = parent->m_flags &	axl_win_EPropertyLine_NoChildGridLines;
		line->m_delimiters = parent->m_delimiters;

		axl_win_TPropertyLine_ModifyChildrenHeight(parent, 1);

		axl_rtl_TList_InsertBefore(&parent->m_childrenList, (axl_rtl_TListEntry*) line, (axl_rtl_TListEntry*) before);

		visibleIndex = 
			parent->m_visibleIndex == -1 || !(parent->m_flags & axl_win_EPropertyLine_IsExpanded) ? -1 : before ? 
			before->m_visibleIndex != -1 ? before->m_visibleIndex : -1 :
			parent->m_visibleIndex + parent->m_childrenHeight;
	}

	if (visibleIndex != -1)
	{
		*axl_win_TPropertyCtrl_InsertVisibleLines(propertyCtrl, visibleIndex, 1) = line;
		line->m_visibleIndex = visibleIndex;

		axl_win_TPropertyCtrl_InvalidateLineRange(propertyCtrl, visibleIndex, -1);		
		axl_win_TPropertyCtrl_RecalcVScroll(propertyCtrl);
	}

	if (source)
	{
		axl_win_TPropertyLine* child;

		line->m_flags = source->m_flags;
		line->m_optionBoxType = source->m_optionBoxType;
		line->m_optionBoxCheckState = source->m_optionBoxCheckState;
		line->m_icon = source->m_icon;
		line->m_indent = source->m_indent;
		axl_obj_TObject_CopyPtr(&line->m_inPlace, source->m_inPlace);
		axl_obj_TData_Copy(&line->m_userData, &source->m_userData);
		line->m_nameAttr = source->m_nameAttr;
		line->m_valueAttr = source->m_valueAttr;
		axl_win_TPropertyLine_SetName(line, source->m_name.m_p);
		axl_win_TPropertyLine_SetValue(line, source->m_name.m_p);

		child = (axl_win_TPropertyLine*) axl_rtl_TList_GetHead(&source->m_childrenList);
		while (child)
		{
			axl_win_TPropertyCtrl_InsertLineEx(propertyCtrl, line, NULL, child);
			child = (axl_win_TPropertyLine*) axl_rtl_TList_GetNext(&source->m_childrenList, (axl_rtl_TListEntry*) child);
		}
	}

	return line;	
}

static
bool_t
AXL_API
_IsChildWindow(HWND hWnd, HWND hWndParent)
{
	while (hWnd)
	{
		hWnd = getParent(hWnd);
		if (hWnd == hWndParent)
			return true;
	}

	return false;
}

void
AXL_API
axl_win_TPropertyCtrl_AdjustInPlacePosition(axl_win_TPropertyCtrl* propertyCtrl)
{
	RECT rect;

	if (!propertyCtrl->m_activeInPlace)
		return;

	axl_win_TPropertyCtrl_GetInPlaceRect(propertyCtrl, &rect);
	axl_win_TPropertyInPlace_Layout(propertyCtrl->m_activeInPlace, &rect);
}

void
AXL_API
axl_win_TPropertyCtrl_Clear(axl_win_TPropertyCtrl* propertyCtrl)
{
	axl_win_TPropertyCtrl_KillSelection(propertyCtrl);
	axl_win_TPropertyLine_ClearLineList(&propertyCtrl->m_lineList);
	axl_rtl_TPtrArray_Clear(&propertyCtrl->m_visibleLineArray);
	axl_win_TPropertyCtrl_CalcWidth(propertyCtrl, 0);
	axl_win_TPropertyCtrl_RecalcHScroll(propertyCtrl);
	axl_win_TPropertyCtrl_RecalcVScroll(propertyCtrl);
	axl_win_TPropertyCtrl_Invalidate(propertyCtrl);
}

void
AXL_API
axl_win_TPropertyCtrl_DelayMenu(axl_win_TPropertyCtrl* propertyCtrl)
{
	axl_win_TPropertyCtrl* rootPropertyCtrl = axl_win_TPropertyCtrl_GetMenuRootPropertyCtrl(propertyCtrl);
	setTimer(rootPropertyCtrl->m_hWnd, axl_win_EPropertyTimer_MenuDelay, rootPropertyCtrl->m_menuDelay, NULL);
}

void
AXL_API
axl_win_TPropertyCtrl_SetSelection(
	axl_win_TPropertyCtrl* propertyCtrl,
	size_t line,
	bool_t setInPlaceFocus
	)
{
	axl_win_TPropertyLine* line;

	size_t lineCount = axl_rtl_TPtrArray_GetCount(&propertyCtrl->m_visibleLineArray);
	if (line >= lineCount)
		line = -1;

	line = line != -1 ? ((axl_win_TPropertyLine**) propertyCtrl->m_visibleLineArray.m_p)[line] : NULL; 

	if (line == propertyCtrl->m_selectedLine)
	{
		// make sure parent is selected -- even if selection didnt change!
		if (line && (line->m_flags & axl_win_EPropertyLine_Menu) && propertyCtrl->m_menuParentPropertyCtrl != propertyCtrl->m_menuRootPropertyCtrl) 
			axl_win_TPropertyCtrl_SetSelection(propertyCtrl->m_menuParentPropertyCtrl, propertyCtrl->m_menuParentPropertyCtrl->m_menuAnchorLine->m_visibleIndex, false);

		return;
	}

	axl_win_TPropertyCtrl_KillSelection(propertyCtrl);

	if (!line || line->m_flags & (axl_win_EPropertyLine_NotSelectable | axl_win_EPropertyLine_IsDisabled))
	{
		axl_win_TPropertyCtrl_RecalcHScroll(propertyCtrl);
		return;
	}

	propertyCtrl->m_selectedLine = line;
	axl_win_TPropertyCtrl_InvalidateLineRange(propertyCtrl, line, line);

	if (line->m_flags & axl_win_EPropertyLine_Menu)
	{
		// schedule menu update

		if (!axl_rtl_TList_IsEmpty(&line->m_childrenList))
			axl_win_TPropertyCtrl_DelayMenu(propertyCtrl);

		// make sure parent is selected

		if (propertyCtrl->m_menuParentPropertyCtrl != propertyCtrl->m_menuRootPropertyCtrl)
			axl_win_TPropertyCtrl_SetSelection(propertyCtrl->m_menuParentPropertyCtrl, propertyCtrl->m_menuParentPropertyCtrl->m_menuAnchorLine->m_visibleIndex, false);

		return;
	}

	if (line->m_inPlace)
	{
		RECT rect;
		propertyCtrl->m_activeInPlace = line->m_inPlace;
		propertyCtrl->m_activeInPlaceMinWidth = 0;

		axl_win_TPropertyCtrl_GetInPlaceRect(propertyCtrl, &rect);
		axl_win_TPropertyInPlace_Load(line->m_inPlace, line, &rect, &propertyCtrl->m_activeInPlaceMinWidth);

		if (setInPlaceFocus)
			axl_win_TPropertyInPlace_SetFocus(line->m_inPlace);
	}

	axl_win_TPropertyCtrl_EnsureVisible(propertyCtrl, line);
	axl_win_TPropertyCtrl_RecalcHScroll(propertyCtrl);
}

void 
AXL_API
axl_win_TPropertyCtrl_AdvanceSelection(
	axl_win_TPropertyCtrl* propertyCtrl,
	int delta,
	bool_t setInPlaceFocus
	)
{
	axl_win_TPropertyLine* line;
	size_t line;
	size_t lineCount = axl_rtl_TPtrArray_GetCount(&propertyCtrl->m_visibleLineArray);
	
	if (!lineCount || !delta) 
		return;

	if (propertyCtrl->m_selectedLine == -1)
	{
		line = 0;
		delta = 1;
	}
	else 
	{
		line = propertyCtrl->m_selectedLine + delta;
		
		if (delta < 0)
		{
			if (line < lineCount)			
				delta = -1;
			else
				line = 0, delta = 1;
		}
		else
		{
			if (line < lineCount)			
				delta = 1;
			else
				line = lineCount - 1, delta = -1;
		}
	}

	while (line < lineCount)
	{
		line = ((axl_win_TPropertyLine**) propertyCtrl->m_visibleLineArray.m_p)[line]; 
	
		if (!(line->m_flags & (axl_win_EPropertyLine_NotSelectable | axl_win_EPropertyLine_IsDisabled)))
		{
			axl_win_TPropertyCtrl_SetSelection(propertyCtrl, line, setInPlaceFocus);
			return;
		}

		line += delta;
	}
}

void 
AXL_API
axl_win_TPropertyCtrl_EnsureVisible(
	axl_win_TPropertyCtrl* propertyCtrl,
	size_t line
	)
{
	bool_t isUpdateNeeded = false;

	if (line >= propertyCtrl->m_firstVisibleLine + propertyCtrl->m_fullyVisibleLineCount)
	{
		propertyCtrl->m_firstVisibleLine = line - propertyCtrl->m_fullyVisibleLineCount + 1;
		isUpdateNeeded = true;
	}

	if (line < propertyCtrl->m_firstVisibleLine)
	{
		propertyCtrl->m_firstVisibleLine = line;
		isUpdateNeeded = true;
	}

	if (!isUpdateNeeded)
		return;

	axl_win_TPropertyCtrl_FixupFirstVisibleLine(propertyCtrl);
	axl_win_TPropertyCtrl_RecalcVScroll(propertyCtrl);
	axl_win_TPropertyCtrl_AdjustInPlacePosition(propertyCtrl);
	axl_win_TPropertyCtrl_Invalidate(propertyCtrl);
}

bool_t
AXL_API
axl_win_TPropertyCtrl_SaveActiveInPlace(axl_win_TPropertyCtrl* propertyCtrl)
{
	axl_win_TPropertyLine* line = axl_win_TPropertyCtrl_GetSelectedLine(propertyCtrl);
	return line && propertyCtrl->m_activeInPlace ? axl_win_TPropertyInPlace_Save(propertyCtrl->m_activeInPlace, line) : true;
}

void
AXL_API
axl_win_TPropertyCtrl_KillSelection(axl_win_TPropertyCtrl* propertyCtrl)
{
	axl_win_TPropertyInPlace* activeInPlace;

	axl_win_TPropertyLine* line = axl_win_TPropertyCtrl_GetSelectedLine(propertyCtrl);

	if (axl_win_TPropertyCtrl_IsMenuActive(propertyCtrl))
		axl_win_TPropertyCtrl_DelayMenu(propertyCtrl);

	if (!line)
		return;

	line->m_optionBoxVolatileState = 0;
	line->m_buttonVolatileState = 0;

	activeInPlace = propertyCtrl->m_activeInPlace;

	axl_win_TPropertyCtrl_InvalidateLineRange(propertyCtrl, propertyCtrl->m_selectedLine, propertyCtrl->m_selectedLine);

	propertyCtrl->m_selectedLine = -1;
	propertyCtrl->m_activeInPlace = NULL;

	if (activeInPlace)
	{		
		if (_IsChildWindow(getFocus(), propertyCtrl->m_hWnd))
			setFocus(propertyCtrl->m_hWnd);

		axl_win_TPropertyInPlace_Save(activeInPlace, line);
		axl_win_TPropertyInPlace_Layout(activeInPlace, NULL); // hide active inplace
	}
}

void
AXL_API
axl_win_TPropertyCtrl_InvalidateSplitterChange(
	axl_win_TPropertyCtrl* propertyCtrl,
	long x
	)
{
	size_t i;
	size_t iEnd;
	size_t lastVisibleLine = propertyCtrl->m_firstVisibleLine + propertyCtrl->m_visibleLineCount;

	RECT rect;
	getClientRect(propertyCtrl->m_hWnd, &rect);
	rect.left = x - propertyCtrl->m_firstVisibleX;
	rect.top = rect.bottom = 0;

    iEnd = axl_rtl_TPtrArray_GetCount(&propertyCtrl->m_visibleLineArray);
	if (iEnd > lastVisibleLine)
		iEnd = lastVisibleLine;

	for (i = propertyCtrl->m_firstVisibleLine; i < iEnd; i++)
	{
		axl_win_TPropertyLine* line = ((axl_win_TPropertyLine**) propertyCtrl->m_visibleLineArray.m_p)[i];

		if (!axl_win_TPropertyLine_IsSinglePane(line))
		{
			rect.bottom += propertyCtrl->m_lineHeight;
			continue;
		}

		if (rect.bottom > rect.top)
			invalidateRect(propertyCtrl->m_hWnd, &rect, false);
	
		rect.bottom += propertyCtrl->m_lineHeight;
		rect.top = rect.bottom;
	}

	if (rect.bottom > rect.top)
		invalidateRect(propertyCtrl->m_hWnd, &rect, false);
}

long
AXL_API
axl_win_TPropertyCtrl_SetSplitterPos(
	axl_win_TPropertyCtrl* propertyCtrl,
	long splitterPos
	)
{
	long x;
	bool_t didFirstVisibleXChange;

	if (splitterPos < (long) propertyCtrl->m_minNamePaneWidth)
		splitterPos = propertyCtrl->m_minNamePaneWidth;

	if (splitterPos == propertyCtrl->m_splitterPos)
		return splitterPos;

	x = min(propertyCtrl->m_splitterPos, splitterPos);

	propertyCtrl->m_splitterPos = splitterPos;

	didFirstVisibleXChange = axl_win_TPropertyCtrl_FixupFirstVisibleX(propertyCtrl);

	if (didFirstVisibleXChange)
		axl_win_TPropertyCtrl_Invalidate(propertyCtrl);
	else
		axl_win_TPropertyCtrl_InvalidateSplitterChange(propertyCtrl, x);
	
	axl_win_TPropertyCtrl_AdjustInPlacePosition(propertyCtrl);	
	axl_win_TPropertyCtrl_RecalcHScroll(propertyCtrl);
	return splitterPos;
}

void
AXL_API
axl_win_TPropertyCtrl_ExpandHelper(
	axl_win_TPropertyCtrl* propertyCtrl,
	axl_win_TPropertyLine* line
	)
{
	size_t i;
	axl_win_TPropertyLine* child;
	
	ASSERT(line->m_visibleIndex != -1);
	ASSERT(line->m_flags & axl_win_EPropertyLine_IsExpanded);

	child = (axl_win_TPropertyLine*) axl_rtl_TList_GetHead(&line->m_childrenList);
	i = line->m_visibleIndex + 1;

	while (child)
	{
		child->m_visibleIndex = i;
		((axl_win_TPropertyLine**) propertyCtrl->m_visibleLineArray.m_p)[i] =child;

		if (child->m_flags & axl_win_EPropertyLine_IsExpanded)
		{
			axl_win_TPropertyCtrl_ExpandHelper(propertyCtrl, child);
			i += child->m_childrenHeight;
		}

		child = (axl_win_TPropertyLine*) axl_rtl_TList_GetNext(&line->m_childrenList, (axl_rtl_TListEntry*) child);
		i++;
	}
}

bool_t
AXL_API
axl_win_TPropertyCtrl_Expand(
	axl_win_TPropertyCtrl* propertyCtrl,
	axl_win_TPropertyLine* line
	)
{
	axl_win_TPropertyHitTest hitTest = {0};

	if (line->m_flags & axl_win_EPropertyLine_IsExpanded)
		return true;

	line->m_flags |= axl_win_EPropertyLine_IsExpanded;

	if (axl_rtl_TList_IsEmpty(&line->m_childrenList))
		return false;

	if (line->m_parent)
	{
		axl_win_TPropertyLine_ModifyChildrenHeight(line->m_parent, (long) line->m_childrenHeight);
		axl_win_TPropertyLine_CalcWidth(line->m_parent, axl_win_EPropertyCalcWidth_Children | axl_win_EPropertyCalcWidth_Parent);
	}

	if (line->m_visibleIndex == -1)
		return true;

	if (propertyCtrl->m_selectedLine != -1 && propertyCtrl->m_selectedLine > line->m_visibleIndex)
		propertyCtrl->m_selectedLine += line->m_childrenHeight;

	axl_win_TPropertyCtrl_InsertVisibleLines(propertyCtrl, line->m_visibleIndex + 1, line->m_childrenHeight);
	axl_win_TPropertyCtrl_ExpandHelper(propertyCtrl, line);
	axl_win_TPropertyCtrl_InvalidateLineRange(propertyCtrl, line->m_visibleIndex, -1);
	axl_win_TPropertyCtrl_AdjustInPlacePosition(propertyCtrl);
	axl_win_TPropertyCtrl_CalcWidth(propertyCtrl, 0);
	axl_win_TPropertyCtrl_RecalcVScroll(propertyCtrl);
	axl_win_TPropertyCtrl_RecalcHScroll(propertyCtrl);

	hitTest.m_line = line;
	hitTest.m_hitTest = axl_win_EPropertyHitTest_PlusMinus;
	axl_win_TPropertyCtrl_Notify(propertyCtrl, axl_win_EPropertyNotify_ExpandCollapse, &hitTest);
	return true;
}

bool_t
AXL_API
axl_win_TPropertyCtrl_Collapse(
	axl_win_TPropertyCtrl* propertyCtrl,
	axl_win_TPropertyLine* line
	)
{
	axl_win_TPropertyHitTest hitTest = {0};

	if (!(line->m_flags & axl_win_EPropertyLine_IsExpanded))
		return true;

	line->m_flags &= ~axl_win_EPropertyLine_IsExpanded;

	if (axl_rtl_TList_IsEmpty(&line->m_childrenList))
		return false;

	if (line->m_parent)
	{
		axl_win_TPropertyLine_ModifyChildrenHeight(line->m_parent, -(long) line->m_childrenHeight);
		axl_win_TPropertyLine_CalcWidth(line->m_parent, axl_win_EPropertyCalcWidth_Children | axl_win_EPropertyCalcWidth_Parent);
	}

	if (line->m_visibleIndex == -1)
		return true;

	if (propertyCtrl->m_selectedLine != -1 && propertyCtrl->m_selectedLine > line->m_visibleIndex)
	{
		if (propertyCtrl->m_selectedLine <= line->m_visibleIndex + line->m_childrenHeight)
			axl_win_TPropertyCtrl_KillSelection(propertyCtrl);
		else
			propertyCtrl->m_selectedLine -= line->m_childrenHeight;
	}

	axl_win_TPropertyCtrl_DeleteVisibleLines(propertyCtrl, line->m_visibleIndex + 1, line->m_childrenHeight);
	axl_win_TPropertyCtrl_InvalidateLineRange(propertyCtrl, line->m_visibleIndex, -1);
	axl_win_TPropertyCtrl_AdjustInPlacePosition(propertyCtrl);
	axl_win_TPropertyCtrl_CalcWidth(propertyCtrl, 0);
	axl_win_TPropertyCtrl_RecalcVScroll(propertyCtrl);
	axl_win_TPropertyCtrl_RecalcHScroll(propertyCtrl);

	hitTest.m_line = line;
	hitTest.m_hitTest = axl_win_EPropertyHitTest_PlusMinus;
	axl_win_TPropertyCtrl_Notify(propertyCtrl, axl_win_EPropertyNotify_ExpandCollapse, &hitTest);
	return true;
}

void
AXL_API
axl_win_TPropertyCtrl_Hover(
	axl_win_TPropertyCtrl* propertyCtrl,
	int hover,
	axl_win_TPropertyLine* line
	)
{
	TRACKMOUSEEVENT track = { sizeof(track), 0, propertyCtrl->m_hWnd };

	bool_t isMouseLeave = hover & axl_win_EPropertyHover_Leave;
	hover &= ~axl_win_EPropertyHover_Leave;

	if (propertyCtrl->m_hover == hover && propertyCtrl->m_hoverAnchorLine == line)
		return;

	switch (propertyCtrl->m_hover)
	{
	case axl_win_EPropertyHover_None:
		track.dwFlags = TME_LEAVE;
		trackMouseEvent(&track);
		break;

	case axl_win_EPropertyHover_OptionBox:
		axl_win_TPropertyLine_SetOptionBoxVolatileState(propertyCtrl->m_hoverAnchorLine, axl_win_EPropertyButtonVolatileState_Normal);
		break;

	case axl_win_EPropertyHover_Button:
		axl_win_TPropertyLine_SetButtonVolatileState(propertyCtrl->m_hoverAnchorLine, axl_win_EPropertyButtonVolatileState_Normal);
		break;

	case axl_win_EPropertyHover_Menu:
		if (isMouseLeave && !axl_win_TPropertyCtrl_IsMenuActive(propertyCtrl))
			axl_win_TPropertyCtrl_KillSelection(propertyCtrl);
		break;
	}

	switch (hover)
	{
	case axl_win_EPropertyHover_None:
		track.dwFlags = TME_CANCEL;
		trackMouseEvent(&track);
		break;

	case axl_win_EPropertyHover_OptionBox:
		axl_win_TPropertyLine_SetOptionBoxVolatileState(line, axl_win_EPropertyButtonVolatileState_Hover);
		break;

	case axl_win_EPropertyHover_Button:
		axl_win_TPropertyLine_SetButtonVolatileState(line, axl_win_EPropertyButtonVolatileState_Hover);
		break;

	case axl_win_EPropertyHover_Menu:
		axl_win_TPropertyCtrl_SetSelection(propertyCtrl, line->m_visibleIndex, false);
		break;
	}

	propertyCtrl->m_hover = hover;
	propertyCtrl->m_hoverAnchorLine = line;
}

void
AXL_API
axl_win_TPropertyCtrl_StartDrag(
	axl_win_TPropertyCtrl* propertyCtrl,
	int drag,
	int hitTest,
	axl_win_TPropertyLine* line
	)
{
	axl_win_TPropertyCtrl_Hover(propertyCtrl, axl_win_EPropertyHover_None, NULL); // stop hover

	propertyCtrl->m_drag = drag;
	propertyCtrl->m_dragAnchorHitTest = hitTest;
	propertyCtrl->m_dragAnchorLine = line;

	switch (drag)
	{
	case axl_win_EPropertyDrag_ReArrange:
		axl_win_TPropertyCtrl_SetSelection(propertyCtrl, line->m_visibleIndex, false);
		break;

	case axl_win_EPropertyDrag_Selection:
		axl_win_TPropertyCtrl_SetSelection(propertyCtrl, line ? line->m_visibleIndex : -1, false);
		break;

	case axl_win_EPropertyDrag_OptionBox:
		axl_win_TPropertyLine_SetOptionBoxVolatileState(propertyCtrl->m_dragAnchorLine, axl_win_EPropertyButtonVolatileState_Pushed);
		break;

	case axl_win_EPropertyDrag_Button:
		axl_win_TPropertyLine_SetButtonVolatileState(propertyCtrl->m_dragAnchorLine, axl_win_EPropertyButtonVolatileState_Pushed);
		break;
	}

	if (getCapture() != propertyCtrl->m_hWnd)
		setCapture(propertyCtrl->m_hWnd);
}

void
AXL_API
axl_win_TPropertyCtrl_StopDrag(axl_win_TPropertyCtrl* propertyCtrl)
{
	switch (propertyCtrl->m_drag)
	{
	case axl_win_EPropertyDrag_None:
		return;

	case axl_win_EPropertyDrag_OptionBox:
		axl_win_TPropertyLine_SetOptionBoxVolatileState(propertyCtrl->m_dragAnchorLine, axl_win_EPropertyButtonVolatileState_Normal);
		break;

	case axl_win_EPropertyDrag_Button:
		axl_win_TPropertyLine_SetButtonVolatileState(propertyCtrl->m_dragAnchorLine, axl_win_EPropertyButtonVolatileState_Normal);
		break;
	}

	propertyCtrl->m_drag = axl_win_EPropertyDrag_None;
	propertyCtrl->m_dragAnchorHitTest = axl_win_EPropertyHitTest_None;
	propertyCtrl->m_dragAnchorLine = NULL;
	
	if (getCapture() == propertyCtrl->m_hWnd)
		releaseCapture();
}

HWND
AXL_API
axl_win_TPropertyCtrl_CreateMenuPropertyCtrl(axl_win_TPropertyCtrl* propertyCtrl)
{
	HINSTANCE hInstance = (HINSTANCE) (LONG_PTR) getWindowLongPtr(propertyCtrl->m_hWnd, GWLP_HINSTANCE);

	ASSERT(!propertyCtrl->m_menuPropertyCtrl);

	propertyCtrl->m_menuPropertyCtrl = axl_mem_Allocate(sizeof(axl_win_TPropertyCtrl));	
	axl_win_TPropertyCtrl_Construct(propertyCtrl->m_menuPropertyCtrl);
	axl_win_TPropertyCtrl_SetImageList(propertyCtrl->m_menuPropertyCtrl, propertyCtrl->m_hImageList);

	propertyCtrl->m_menuPropertyCtrl->m_menuRootPropertyCtrl = axl_win_TPropertyCtrl_GetMenuRootPropertyCtrl(propertyCtrl);
	propertyCtrl->m_menuPropertyCtrl->m_menuParentPropertyCtrl = propertyCtrl;
	propertyCtrl->m_menuPropertyCtrl->m_hasGridLinesAtRoot = false;
	propertyCtrl->m_menuPropertyCtrl->m_splitterPos = 0;

	return axl_win_TPropertyCtrl_Create(propertyCtrl->m_menuPropertyCtrl, WS_POPUP | WS_CHILD | WS_BORDER, NULL, propertyCtrl->m_hWnd, 0, hInstance);
}

bool_t 
AXL_API
axl_win_TPropertyCtrl_IsMenuWindow(
	axl_win_TPropertyCtrl* propertyCtrl,
	HWND hWnd
	)
{
	while (axl_win_TPropertyCtrl_IsMenuActive(propertyCtrl))
	{
		ASSERT(propertyCtrl->m_menuPropertyCtrl);
		if (propertyCtrl->m_menuPropertyCtrl->m_hWnd == hWnd)
			return true;

		propertyCtrl = propertyCtrl->m_menuPropertyCtrl;
	}

	return false;
}

void
AXL_API
axl_win_TPropertyCtrl_HideMenu(axl_win_TPropertyCtrl* propertyCtrl)
{
	if (!axl_win_TPropertyCtrl_IsMenuActive(propertyCtrl))
		return;

	if (!propertyCtrl->m_menuParentPropertyCtrl)
		axl_win_TPropertyLine_SetButtonVolatileState(propertyCtrl->m_menuAnchorLine, axl_win_EPropertyButtonVolatileState_Normal);

	while (propertyCtrl)
	{
		if (!axl_win_TPropertyCtrl_IsMenuActive(propertyCtrl))
			break;
		
		showWindow(propertyCtrl->m_menuPropertyCtrl->m_hWnd, SW_HIDE);
		propertyCtrl->m_menuAnchorLine = NULL;
		propertyCtrl = propertyCtrl->m_menuPropertyCtrl;
	}
}

void
AXL_API
axl_win_TPropertyCtrl_CancelMenu(axl_win_TPropertyCtrl* propertyCtrl)
{
	axl_win_TPropertyCtrl* rootPropertyCtrl = axl_win_TPropertyCtrl_GetMenuRootPropertyCtrl(propertyCtrl);
	killTimer(rootPropertyCtrl->m_hWnd, axl_win_EPropertyTimer_MenuDelay);
	axl_win_TPropertyCtrl_HideMenu(rootPropertyCtrl);
}

void
AXL_API
axl_win_TPropertyCtrl_TrackMenu(
	axl_win_TPropertyCtrl* propertyCtrl,
	axl_win_TPropertyLine* line
	)
{
	MSG msg;
	POINT pos;
	SIZE size;

	axl_win_TPropertyCtrl* menuPropertyCtrl;
	axl_win_TPropertyLine* child;
	size_t visibleIndex;

	axl_win_TPropertyCtrl* rootPropertyCtrl = axl_win_TPropertyCtrl_GetMenuRootPropertyCtrl(propertyCtrl);
	killTimer(rootPropertyCtrl->m_hWnd, axl_win_EPropertyTimer_MenuDelay);

	axl_win_TPropertyCtrl_Hover(propertyCtrl, axl_win_EPropertyHover_None, NULL); // stop hover

	if (propertyCtrl->m_menuAnchorLine == line)
		return;

	axl_win_TPropertyCtrl_HideMenu(propertyCtrl);

	if (!propertyCtrl->m_menuParentPropertyCtrl)
		axl_win_TPropertyLine_SetButtonVolatileState(line, axl_win_EPropertyButtonVolatileState_Pushed);

	if (!line || axl_rtl_TList_IsEmpty(&line->m_childrenList))
		return;

	ASSERT(line->m_visibleIndex != -1);
	ASSERT(line->m_flags & axl_win_EPropertyLine_Menu);

	if (!propertyCtrl->m_menuPropertyCtrl)
		axl_win_TPropertyCtrl_CreateMenuPropertyCtrl(propertyCtrl);

	menuPropertyCtrl = propertyCtrl->m_menuPropertyCtrl;
	ASSERT(menuPropertyCtrl);

	axl_win_TPropertyCtrl_Clear(menuPropertyCtrl);
	menuPropertyCtrl->m_lineList = &line->m_childrenList;

	axl_rtl_TPtrArray_Reserve(&menuPropertyCtrl->m_visibleLineArray, line->m_childrenList.m_count);

	visibleIndex = 0;
	child = (axl_win_TPropertyLine*) axl_rtl_TList_GetHead(&line->m_childrenList);
	while (child)
	{
		axl_rtl_TPtrArray_AppendElement(&menuPropertyCtrl->m_visibleLineArray, child);
		child->m_propertyCtrl = menuPropertyCtrl;
		child->m_visibleIndex = visibleIndex;
		child = (axl_win_TPropertyLine*) axl_rtl_TList_GetNext(&line->m_childrenList, (axl_rtl_TListEntry*) child);
		visibleIndex++;
	}

	axl_win_TPropertyCtrl_UpdateAllLineWidths(menuPropertyCtrl);

	propertyCtrl->m_menuAnchorLine = line;
	
	axl_win_TPropertyCtrl_GetSize(menuPropertyCtrl, &size);
	size.cx += 2 * getSystemMetrics(SM_CXBORDER) + getSystemMetrics(SM_CXVSCROLL);
	size.cy += 2 * getSystemMetrics(SM_CYBORDER);

	axl_win_TPropertyCtrl_GetMenuPos(propertyCtrl, &size, &pos);

	if (!axl_win_TPropertyCtrl_IsMenu(propertyCtrl))
		pos.x++;

	setWindowPos(
		menuPropertyCtrl->m_hWnd, 
		NULL, 
		pos.x, pos.y, 
		size.cx, size.cy,
		SWP_NOZORDER | SWP_SHOWWINDOW | SWP_NOACTIVATE
		);

	if (axl_win_TPropertyCtrl_IsMenu(propertyCtrl))
		return;

	// this is only for the root property ctrl

	setFocus(propertyCtrl->m_hWnd);

	while (axl_win_TPropertyCtrl_IsMenuActive(propertyCtrl))
	{
		peekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);

		switch (msg.message)
		{
		case WM_NCLBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
		case WM_NCMBUTTONDOWN:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
			if (!axl_win_TPropertyCtrl_IsMenuWindow(propertyCtrl, msg.hwnd))
				axl_win_TPropertyCtrl_CancelMenu(propertyCtrl);
			break;
		}

        peekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		translateMessage(&msg);
		dispatchMessage(&msg);
	}
}

void
AXL_API
axl_win_TPropertyCtrl_UpdateMenu(axl_win_TPropertyCtrl* propertyCtrl)
{
	if (!axl_win_TPropertyCtrl_IsMenuActive(propertyCtrl))
		return;

	propertyCtrl = propertyCtrl->m_menuPropertyCtrl;

	while (propertyCtrl)
	{
		axl_win_TPropertyLine* line = axl_win_TPropertyCtrl_GetSelectedLine(propertyCtrl);

		if (!line || propertyCtrl->m_menuAnchorLine != line)
		{
			if (propertyCtrl->m_menuAnchorLine)
				axl_win_TPropertyCtrl_HideMenu(propertyCtrl);
		
			if (line && (line->m_flags & axl_win_EPropertyLine_Menu))
				axl_win_TPropertyCtrl_TrackMenu(propertyCtrl, line);

			break;
		}

		propertyCtrl = propertyCtrl->m_menuPropertyCtrl;
	}	
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// message handlers

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnCreate(axl_win_TPropertyCtrl* propertyCtrl)
{	
	HINSTANCE hInstance = (HINSTANCE) (LONG_PTR) getWindowLongPtr(propertyCtrl->m_hWnd, GWLP_HINSTANCE);
	TOOLINFO toolInfo = { sizeof(toolInfo) };

	propertyCtrl->m_hWndToolTip = createWindow(TOOLTIPS_CLASS, NULL, TTS_ALWAYSTIP | TTS_NOPREFIX, 0,0,0,0, propertyCtrl->m_hWnd, 0, hInstance, NULL);
	
	if (!propertyCtrl->m_hWndToolTip)
		return 0;

	toolInfo.uFlags   = TTF_IDISHWND;
	toolInfo.hwnd     = getParent(propertyCtrl->m_hWnd); 
	toolInfo.uId      = (UINT_PTR) propertyCtrl->m_hWnd;
    toolInfo.hinst    = hInstance; 
	toolInfo.lpszText = LPSTR_TEXTCALLBACK;  

	sendMessage(propertyCtrl->m_hWndToolTip, TTM_ADDTOOL, 0, (LPARAM) &toolInfo);
	sendMessage(propertyCtrl->m_hWndToolTip, TTM_SETMAXTIPWIDTH, 0, 400);
	sendMessage(propertyCtrl->m_hWndToolTip, TTM_SETDELAYTIME, TTDT_AUTOPOP, SHRT_MAX);
	sendMessage(propertyCtrl->m_hWndToolTip, TTM_SETDELAYTIME, TTDT_INITIAL, propertyCtrl->m_toolTipShowDelay);
	
	return 0;
}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnDestroy(axl_win_TPropertyCtrl* propertyCtrl)
{
	destroyWindow(propertyCtrl->m_hWndToolTip);
	propertyCtrl->m_hWndToolTip = NULL;

	// see http://pluralsight.com/blogs/keith/archive/2005/01/13/4940.aspx
#pragma warning(disable:4244)
	setWindowLongPtr(propertyCtrl->m_hWnd, 0, 0);
#pragma warning(default:4244)

	propertyCtrl->m_hWnd = NULL;
	return 0;
}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnSetFocus(axl_win_TPropertyCtrl* propertyCtrl)
{
	if (!axl_win_TPropertyCtrl_IsMenuActive(propertyCtrl) && propertyCtrl->m_activeInPlace)
		axl_win_TPropertyInPlace_SetFocus(propertyCtrl->m_activeInPlace);

	return 0;
}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnKillFocus(axl_win_TPropertyCtrl* propertyCtrl)
{
	return 0;
}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnPaint(axl_win_TPropertyCtrl* propertyCtrl)
{
	PAINTSTRUCT paintStruct;
	HDC hdc = beginPaint(propertyCtrl->m_hWnd, &paintStruct);
	axl_win_TPropertyCtrl_PaintRect(propertyCtrl, hdc, &paintStruct.rcPaint);
	endPaint(propertyCtrl->m_hWnd, &paintStruct);
	return 0;
}

/*

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnPaint(axl_win_TPropertyCtrl* propertyCtrl)
{
	int result;
	RECT updateRect;
	HDC hdc;
	DWORD i;

	BYTE buffer[sizeof(RGNDATA) + 2 * sizeof(RECT)];
	RGNDATA* rgnData = (RGNDATA*) buffer;
	RECT* rects = (RECT*) rgnData->Buffer;

	getUpdateRgn(propertyCtrl->m_hWnd, propertyCtrl->m_hUpdateRgn, false);
	if (result != COMPLEXREGION)
		return axl_win_TPropertyCtrl_DefOnPaint(propertyCtrl);

	result = getRegionData(propertyCtrl->m_hUpdateRgn, sizeof(buffer), rgnData);
	if (!result || rgnData->rdh.nCount > 2)
		return axl_win_TPropertyCtrl_DefOnPaint(propertyCtrl);

	validateRgn(propertyCtrl->m_hWnd, propertyCtrl->m_hUpdateRgn);

	hdc = getDC(propertyCtrl->m_hWnd);
	selectClipRgn(hdc, propertyCtrl->m_hUpdateRgn);

	axl_dbg_Trace("get rgn: %d, count: %d (%d,%d,%d,%d)\n", 
		result, 
		rgnData->rdh.nCount, 
		rgnData->rdh.rcBound.left,
		rgnData->rdh.rcBound.top,
		rgnData->rdh.rcBound.right,
		rgnData->rdh.rcBound.bottom
		);

	for (i = 0; i < rgnData->rdh.nCount; i++)
		axl_win_TPropertyCtrl_PaintRect(propertyCtrl, hdc, &rects[i]);

	axl_gr_TColor_SetRgb(&propertyCtrl->m_textPaint.m_stdAttr.m_backColor, getSysColor(COLOR_BTNFACE));

	releaseDC(propertyCtrl->m_hWnd, hdc);
	return 0;
}

*/

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnSize(axl_win_TPropertyCtrl* propertyCtrl)
{
	bool_t didFirstVisibleLineChange;
	bool_t didFirstVisibleXChange;

	RECT clientRect;
	getClientRect(propertyCtrl->m_hWnd, &clientRect);

	propertyCtrl->m_visibleWidth = clientRect.right;
	propertyCtrl->m_fullyVisibleLineCount = clientRect.bottom / propertyCtrl->m_lineHeight;
	propertyCtrl->m_visibleLineCount = propertyCtrl->m_fullyVisibleLineCount;
	
	if (clientRect.bottom % propertyCtrl->m_lineHeight)
		propertyCtrl->m_visibleLineCount++;

	didFirstVisibleLineChange = axl_win_TPropertyCtrl_FixupFirstVisibleLine(propertyCtrl);
	didFirstVisibleXChange = axl_win_TPropertyCtrl_FixupFirstVisibleX(propertyCtrl);

	axl_win_TPropertyCtrl_RecalcVScroll(propertyCtrl);
	axl_win_TPropertyCtrl_RecalcHScroll(propertyCtrl);

	axl_win_TPropertyCtrl_AdjustInPlacePosition(propertyCtrl);

	if (didFirstVisibleLineChange || didFirstVisibleXChange)
		axl_win_TPropertyCtrl_Invalidate(propertyCtrl);

	return 0;
}

void
AXL_API
axl_win_TPropertyCtrl_OnScroll(
	axl_win_TPropertyCtrl* propertyCtrl, 
	int bar,
	int code
	)
{
	int newPos;
	int maxPos;

	SCROLLINFO scrollInfo = { sizeof(scrollInfo) };
	scrollInfo.fMask = SIF_ALL;
	
	getScrollInfo(propertyCtrl->m_hWnd, bar, &scrollInfo);

	newPos = scrollInfo.nPos;
	maxPos = scrollInfo.nMax - scrollInfo.nPage + 1;

	switch (code)
	{
	case SB_TOP:
		newPos = 0;
		break;

	case SB_LINEUP:
		if (bar == SB_VERT)
			newPos--;
		else
			newPos -= propertyCtrl->m_lineHeight;
		break;

	case SB_LINEDOWN:
		if (bar == SB_VERT)
			newPos++;
		else
			newPos += propertyCtrl->m_lineHeight;
		break;

	case SB_PAGEUP:
		newPos -= scrollInfo.nPage; 
		break;

	case SB_PAGEDOWN:
		newPos += scrollInfo.nPage; 
		break;

	case SB_BOTTOM:
		newPos = scrollInfo.nMax; 
		break;

	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		newPos = scrollInfo.nTrackPos; 
		break;
	}

	if (newPos < 0)
		newPos = 0;
	else if (newPos > maxPos)
		newPos = maxPos;

	if (newPos == scrollInfo.nPos)
		return;

	if (bar == SB_VERT)
	{
		propertyCtrl->m_firstVisibleLine = newPos;
		axl_win_TPropertyCtrl_FixupFirstVisibleLine(propertyCtrl);
	}
	else
	{
		propertyCtrl->m_firstVisibleX = newPos;
		axl_win_TPropertyCtrl_FixupFirstVisibleX(propertyCtrl);
	}

	scrollInfo.fMask = SIF_POS;
	scrollInfo.nPos = newPos;
	setScrollInfo(propertyCtrl->m_hWnd, bar, &scrollInfo, TRUE);

	axl_win_TPropertyCtrl_Invalidate(propertyCtrl);
	axl_win_TPropertyCtrl_AdjustInPlacePosition(propertyCtrl);
	axl_win_TPropertyCtrl_CancelMenu(propertyCtrl);
}

void
AXL_API
axl_win_TPropertyCtrl_StdLButtonDownHandler(
	axl_win_TPropertyCtrl* propertyCtrl,
	axl_win_TPropertyHitTest* hitTest
	)
{
	if (!axl_win_TPropertyCtrl_IsMenu(propertyCtrl))
	{
		setFocus(propertyCtrl->m_hWnd);
		axl_win_TPropertyCtrl_StartDrag(propertyCtrl, axl_win_EPropertyDrag_Selection, hitTest->m_hitTest, hitTest->m_line);
	}

}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnLButtonDown(
	axl_win_TPropertyCtrl* propertyCtrl,
	long x,
	long y
	)
{
	axl_win_TPropertyHitTest hitTest;
	axl_win_TPropertyLine* line = axl_win_TPropertyCtrl_HitTest(propertyCtrl, x, y, &hitTest);
	
	switch (hitTest.m_hitTest)
	{
	case axl_win_EPropertyHitTest_Splitter:
		propertyCtrl->m_dragDelta.x = propertyCtrl->m_splitterPos - (x + propertyCtrl->m_firstVisibleX);
		propertyCtrl->m_dragDelta.y = 0;
		axl_win_TPropertyCtrl_StartDrag(propertyCtrl, axl_win_EPropertyDrag_Splitter, hitTest.m_hitTest, line);
		break;

	case axl_win_EPropertyHitTest_PlusMinus:
		axl_win_TPropertyCtrl_ExpandCollapse(propertyCtrl, line, (line->m_flags & axl_win_EPropertyLine_IsExpanded) == 0);
		break;

	case axl_win_EPropertyHitTest_ReArrange:
		axl_win_TPropertyCtrl_StartDrag(propertyCtrl, axl_win_EPropertyDrag_ReArrange, hitTest.m_hitTest, line);
		break;

	case axl_win_EPropertyHitTest_OptionBox:
		if (!(line->m_flags & axl_win_EPropertyLine_IsDisabled))
		{
//			axl_win_TPropertyCtrl_SetSelection(pPropertyCtrl, pLine->m_VisibleIndex);
			axl_win_TPropertyCtrl_StartDrag(propertyCtrl, axl_win_EPropertyDrag_OptionBox, hitTest.m_hitTest, line);
		}
		else
			axl_win_TPropertyCtrl_StdLButtonDownHandler(propertyCtrl, &hitTest);

		break;

	case axl_win_EPropertyHitTest_Hyperlink:
		if (!(line->m_flags & axl_win_EPropertyLine_IsDisabled))
		{
			propertyCtrl->m_dragAnchorHyperlink = hitTest.m_hyperlink;
			axl_win_TPropertyCtrl_StartDrag(propertyCtrl, axl_win_EPropertyDrag_Hyperlink, hitTest.m_hitTest, line);
		}
		else
			axl_win_TPropertyCtrl_StdLButtonDownHandler(propertyCtrl, &hitTest);

		break;

	case axl_win_EPropertyHitTest_Menu:
		if (!(line->m_flags & axl_win_EPropertyLine_IsDisabled))
		{
//			axl_win_TPropertyCtrl_SetSelection(pPropertyCtrl, pLine->m_VisibleIndex);
			axl_win_TPropertyCtrl_TrackMenu(propertyCtrl, line);
		}
		else
			axl_win_TPropertyCtrl_StdLButtonDownHandler(propertyCtrl, &hitTest);

		break;

	case axl_win_EPropertyHitTest_Button:
		if (!(line->m_flags & axl_win_EPropertyLine_IsDisabled))
		{
//			axl_win_TPropertyCtrl_SetSelection(pPropertyCtrl, pLine->m_VisibleIndex);
			axl_win_TPropertyCtrl_StartDrag(propertyCtrl, axl_win_EPropertyDrag_Button, hitTest.m_hitTest, line);
		}
		else
			axl_win_TPropertyCtrl_StdLButtonDownHandler(propertyCtrl, &hitTest);

		break;

	default:
		axl_win_TPropertyCtrl_StdLButtonDownHandler(propertyCtrl, &hitTest);
	}

	return 0;
}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnLButtonDblClk(
	axl_win_TPropertyCtrl* propertyCtrl,
	long x,
	long y
	)
{
	axl_win_TPropertyHitTest hitTest;
	axl_win_TPropertyLine* line = axl_win_TPropertyCtrl_HitTest(propertyCtrl, x, y, &hitTest);

	switch (hitTest.m_hitTest)
	{
	case axl_win_EPropertyHitTest_Name:
	case axl_win_EPropertyHitTest_Icon:
		if (!axl_rtl_TList_IsEmpty(&line->m_childrenList))
		{
			axl_win_TPropertyCtrl_ExpandCollapse(propertyCtrl, line, (line->m_flags & axl_win_EPropertyLine_IsExpanded) == 0);
			break;
		}

	default:
		return axl_win_TPropertyCtrl_OnLButtonDown(propertyCtrl, x, y);
	}

	return 0;
}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnLButtonUp(
	axl_win_TPropertyCtrl* propertyCtrl,
	long x,
	long y
	)
{
	axl_win_TPropertyHitTest hitTest;
	axl_win_TPropertyLine* line = axl_win_TPropertyCtrl_HitTest(propertyCtrl, x, y, &hitTest);

	switch (propertyCtrl->m_drag)
	{
	case axl_win_EPropertyDrag_Selection:
		if (propertyCtrl->m_activeInPlace)
			axl_win_TPropertyInPlace_SetFocus(propertyCtrl->m_activeInPlace);
		break;

	case axl_win_EPropertyDrag_Hyperlink:
		if (hitTest.m_hyperlink == propertyCtrl->m_dragAnchorHyperlink)
			axl_win_TPropertyCtrl_Notify(propertyCtrl, axl_win_EPropertyNotify_HyperlinkClick, &hitTest);
		break;

	case axl_win_EPropertyDrag_OptionBox:
		if (line != propertyCtrl->m_dragAnchorLine || hitTest.m_hitTest != propertyCtrl->m_dragAnchorHitTest)
			break;

		switch (line->m_optionBoxType)
		{
		case axl_win_EPropertyOptionBoxType_RadioButton:
			axl_win_TPropertyLine_SetOptionBoxCheckState(line, true);
			break;

		case axl_win_EPropertyOptionBoxType_CheckBox:
			axl_win_TPropertyLine_SetOptionBoxCheckState(line, !line->m_optionBoxCheckState);
			break;
		}

		axl_win_TPropertyCtrl_Notify(propertyCtrl, axl_win_EPropertyNotify_OptionBoxClick, &hitTest);
		break;

	case axl_win_EPropertyDrag_Button:
		if (line == propertyCtrl->m_dragAnchorLine && line->m_buttonVolatileState == axl_win_EPropertyButtonVolatileState_Pushed)
			axl_win_TPropertyCtrl_Notify(propertyCtrl, axl_win_EPropertyNotify_ButtonClick, &hitTest);
		break;

	default:
		if (axl_win_TPropertyCtrl_IsMenu(propertyCtrl) && axl_rtl_TList_IsEmpty(&line->m_childrenList) && !line->m_optionBoxType)
		{
			axl_win_TPropertyCtrl_CancelMenu(propertyCtrl);
			
			if (line)
				axl_win_TPropertyCtrl_Notify(propertyCtrl, axl_win_EPropertyNotify_MenuClick, &hitTest);
		}
	}

	axl_win_TPropertyCtrl_StopDrag(propertyCtrl);
	return 0;
}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnRButtonDown(
	axl_win_TPropertyCtrl* propertyCtrl,
	long x,
	long y
	)
{
	return 0;
}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnRButtonUp(
	axl_win_TPropertyCtrl* propertyCtrl,
	long x,
	long y
	)
{
	return 0;
}

void
AXL_API
axl_win_TPropertyCtrl_SetToolTip(
	axl_win_TPropertyCtrl* propertyCtrl,
	axl_win_TPropertyHitTest* hitTest
	)
{
	axl_win_TPropertyLine* line = hitTest->m_line;

	TOOLINFO toolInfo = { sizeof(toolInfo) };
	const tchar_t* text;

	if (line == propertyCtrl->m_lastTooltipLine)
		return;

	// handle different re-show time delay

	sendMessage(
		propertyCtrl->m_hWndToolTip, 
		TTM_SETDELAYTIME, TTDT_INITIAL, 
		isWindowVisible(propertyCtrl->m_hWndToolTip) ? 
			propertyCtrl->m_toolTipReShowDelay :
			propertyCtrl->m_toolTipShowDelay
		);

	sendMessage(propertyCtrl->m_hWndToolTip, TTM_ACTIVATE, false, 0);

	if (!line)
	{
		propertyCtrl->m_lastTooltipLine = NULL;
		return;
	}

	if (!axl_rtl_TString_IsEmpty(&line->m_toolTipText))
		text = line->m_toolTipText.m_p;
	else if ((hitTest->m_hitTest == axl_win_EPropertyHitTest_Name || hitTest->m_hitTest == axl_win_EPropertyHitTest_OptionBox) && !axl_win_TPropertyLine_IsPartFullyVisible(line, axl_win_EPropertyLinePart_Name))
		text = line->m_name.m_p;
	else if (hitTest->m_hitTest == axl_win_EPropertyHitTest_Value && !axl_win_TPropertyLine_IsPartFullyVisible(line, axl_win_EPropertyLinePart_Value))
		text = line->m_value.m_p;
	else
	{
		propertyCtrl->m_lastTooltipLine = NULL;
		return;
	}

	toolInfo.uFlags = TTF_IDISHWND;
	toolInfo.hwnd = getParent(propertyCtrl->m_hWnd);
	toolInfo.uId = (uintptr_t) propertyCtrl->m_hWnd;
	toolInfo.lpszText = (tchar_t*) text;

	sendMessage(propertyCtrl->m_hWndToolTip, TTM_ACTIVATE, true, 0);

	if (!axl_rtl_TString_IsEmpty(&line->m_toolTipTitle))
		sendMessage(propertyCtrl->m_hWndToolTip, TTM_SETTITLE, line->m_toolTipIcon, (LPARAM) line->m_toolTipTitle.m_p);
	else
		sendMessage(propertyCtrl->m_hWndToolTip, TTM_SETTITLE, 0, 0);

	sendMessage(propertyCtrl->m_hWndToolTip, TTM_UPDATETIPTEXT, 0, (LPARAM) &toolInfo);
	propertyCtrl->m_lastTooltipLine = line;
}

bool_t
AXL_API
axl_win_TPropertyCtrl_ReArrange(
	axl_win_TPropertyCtrl* propertyCtrl,
	axl_win_TPropertyLine* lineFrom,
	axl_win_TPropertyLine* lineTo
	)
{
	size_t upperLine;
	size_t lowerLine;

	if (lineFrom == lineTo || 
		lineFrom->m_parent != lineTo->m_parent ||
		!axl_rtl_TList_IsEmpty(&lineFrom->m_childrenList) ||
		!axl_rtl_TList_IsEmpty(&lineTo->m_childrenList))
		return false;

	ASSERT(lineFrom->m_visibleIndex != -1);
	ASSERT(lineTo->m_visibleIndex != -1);

	axl_rtl_TList_Remove(&lineFrom->m_parent->m_childrenList, (axl_rtl_TListEntry*) lineFrom);
    
	if (lineFrom->m_visibleIndex < lineTo->m_visibleIndex)
	{
		upperLine = lineFrom->m_visibleIndex;
		lowerLine = lineTo->m_visibleIndex;
		axl_rtl_TList_InsertAfter(&lineFrom->m_parent->m_childrenList, (axl_rtl_TListEntry*) lineFrom, (axl_rtl_TListEntry*) lineTo);
	}
	else
	{
		upperLine = lineTo->m_visibleIndex;
		lowerLine = lineFrom->m_visibleIndex;
		axl_rtl_TList_InsertBefore(&lineFrom->m_parent->m_childrenList, (axl_rtl_TListEntry*) lineFrom, (axl_rtl_TListEntry*) lineTo);
	}

	axl_win_TPropertyCtrl_ReArrangeVisibleLines(propertyCtrl, lineFrom->m_visibleIndex, lineTo->m_visibleIndex);
	axl_win_TPropertyCtrl_InvalidateLineRange(propertyCtrl, upperLine, lowerLine);
	axl_win_TPropertyCtrl_SetSelection(propertyCtrl, lineFrom->m_visibleIndex, false);
	return true;
}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnMouseMove(
	axl_win_TPropertyCtrl* propertyCtrl,
	long x,
	long y
	)
{
	axl_win_TPropertyHitTest hitTest;
	axl_win_TPropertyLine* line = axl_win_TPropertyCtrl_HitTest(propertyCtrl, x, y, &hitTest);

	axl_win_TPropertyCtrl_SetToolTip(propertyCtrl, &hitTest);

	switch (propertyCtrl->m_drag)
	{
	case axl_win_EPropertyDrag_Splitter:
		axl_win_TPropertyCtrl_SetSplitterPos(propertyCtrl, x + propertyCtrl->m_firstVisibleX + propertyCtrl->m_dragDelta.x);
		break;

	case axl_win_EPropertyDrag_ReArrange:
		if (line && axl_win_TPropertyCtrl_ReArrange(propertyCtrl, propertyCtrl->m_dragAnchorLine, line))
		{
			hitTest.m_line = propertyCtrl->m_dragAnchorLine->m_parent;
			axl_win_TPropertyCtrl_Notify(propertyCtrl, axl_win_EPropertyNotify_ReArrange, &hitTest);
		}
		break;

	case axl_win_EPropertyDrag_Selection:
		axl_win_TPropertyCtrl_SetSelection(propertyCtrl, y / propertyCtrl->m_lineHeight + propertyCtrl->m_firstVisibleLine, false);
		break;

	case axl_win_EPropertyDrag_OptionBox:
		if (line == propertyCtrl->m_dragAnchorLine && hitTest.m_hitTest == axl_win_EPropertyHitTest_OptionBox)
			axl_win_TPropertyLine_SetOptionBoxVolatileState(propertyCtrl->m_dragAnchorLine, axl_win_EPropertyButtonVolatileState_Pushed);
		else
			axl_win_TPropertyLine_SetOptionBoxVolatileState(propertyCtrl->m_dragAnchorLine, axl_win_EPropertyButtonVolatileState_Normal);
		break;

	case axl_win_EPropertyDrag_Button:
		if (line == propertyCtrl->m_dragAnchorLine && hitTest.m_hitTest == axl_win_EPropertyHitTest_Button)
			axl_win_TPropertyLine_SetButtonVolatileState(propertyCtrl->m_dragAnchorLine, axl_win_EPropertyButtonVolatileState_Pushed);
		else
			axl_win_TPropertyLine_SetButtonVolatileState(propertyCtrl->m_dragAnchorLine, axl_win_EPropertyButtonVolatileState_Normal);
		break;

	default:
		if (!line)
			axl_win_TPropertyCtrl_Hover(propertyCtrl, axl_win_EPropertyHover_None, NULL);
		
		if (axl_win_TPropertyCtrl_IsMenuActive(propertyCtrl) && 
			!propertyCtrl->m_menuParentPropertyCtrl && 
			hitTest.m_hitTest == axl_win_EPropertyHitTest_Menu && 
			!(line->m_flags & axl_win_EPropertyLine_IsDisabled))
			axl_win_TPropertyCtrl_TrackMenu(propertyCtrl, line);
		else if (axl_win_TPropertyCtrl_IsMenu(propertyCtrl) && hitTest.m_hitTest != axl_win_EPropertyHitTest_OptionBox && hitTest.m_hitTest != axl_win_EPropertyHitTest_Button)
			axl_win_TPropertyCtrl_Hover(propertyCtrl, axl_win_EPropertyHover_Menu, line);
		else if (hitTest.m_hitTest == axl_win_EPropertyHitTest_Button)
			axl_win_TPropertyCtrl_Hover(propertyCtrl, axl_win_EPropertyHover_Button, line);
		else if (hitTest.m_hitTest == axl_win_EPropertyHitTest_Menu)
			axl_win_TPropertyCtrl_Hover(propertyCtrl, axl_win_EPropertyHover_Button, line);
		else if (hitTest.m_hitTest == axl_win_EPropertyHitTest_OptionBox)
			axl_win_TPropertyCtrl_Hover(propertyCtrl, axl_win_EPropertyHover_OptionBox, line);
		else 
			axl_win_TPropertyCtrl_Hover(propertyCtrl, axl_win_EPropertyHover_None, NULL);
	}

	return 0;
}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnMouseLeave(axl_win_TPropertyCtrl* propertyCtrl)
{
	axl_win_TPropertyCtrl_Hover(propertyCtrl, axl_win_EPropertyHover_None | axl_win_EPropertyHover_Leave, NULL);
	return 0;
}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnMouseWheel(
	axl_win_TPropertyCtrl* propertyCtrl, 
	short delta
	)
{
	int code;
	int lineCount; 
	int i;

	if (delta > 0)
	{
		code = SB_LINEUP;
		lineCount = delta / WHEEL_DELTA;
	}
	else
	{
		code = SB_LINEDOWN;
		lineCount = -delta / WHEEL_DELTA;
	}

	lineCount *= propertyCtrl->m_mouseWheelSpeed;
	
	for (i = 0; i < lineCount; i++)
		axl_win_TPropertyCtrl_OnScroll(propertyCtrl, SB_VERT, code);

	return 0;
}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnSetCursor(
	axl_win_TPropertyCtrl* propertyCtrl,
	WPARAM wParam,
	LPARAM lParam
	)
{
	axl_win_TPropertyHitTest hitTest;

	POINT pos;
	getCursorPos(&pos);
	screenToClient(propertyCtrl->m_hWnd, &pos);

	axl_win_TPropertyCtrl_HitTest(propertyCtrl, pos.x, pos.y, &hitTest);

	switch (hitTest.m_hitTest)
	{
	case axl_win_EPropertyHitTest_ReArrange:
		setCursor(propertyCtrl->m_hReArrangeCursor);
		return TRUE;

	case axl_win_EPropertyHitTest_Splitter:
		setCursor(propertyCtrl->m_hSplitterCursor);
		return TRUE;

	case axl_win_EPropertyHitTest_Hyperlink:
		setCursor(propertyCtrl->m_hHyperlinkCursor);
		return TRUE;

	default:
		return defWindowProc(propertyCtrl->m_hWnd, WM_SETCURSOR, wParam, lParam);
	}
}

LRESULT 
AXL_API
axl_win_TPropertyCtrl_OnTimer(
	axl_win_TPropertyCtrl* propertyCtrl,
	int timer
	)
{
	if (timer != axl_win_EPropertyTimer_MenuDelay)
		return 0;

	killTimer(propertyCtrl->m_hWnd, timer);
	axl_win_TPropertyCtrl_UpdateMenu(propertyCtrl);
	return 0;
}

LRESULT 
CALLBACK 
axl_win_TPropertyCtrl_WndProc(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
	)
{
	axl_win_TPropertyCtrl* propertyCtrl;
	LRESULT lResult = 0;

	if (msg == WM_CREATE) // special handling
	{
		// establish connection axl_win_TPropertyCtrl <-> HWND

		CREATESTRUCT* p = (CREATESTRUCT*) lParam;
		propertyCtrl = (axl_win_TPropertyCtrl*) p->lpCreateParams;
		propertyCtrl->m_hWnd = hWnd;

		// see http://pluralsight.com/blogs/keith/archive/2005/01/13/4940.aspx
#pragma warning(disable:4244)
		setWindowLongPtr(hWnd, 0, (LONG_PTR) propertyCtrl);
#pragma warning(default:4244)

		return axl_win_TPropertyCtrl_OnCreate(propertyCtrl);
	}

	propertyCtrl = axl_win_TPropertyCtrl_FromHwnd(hWnd);
	if (!propertyCtrl) // control was detached
		return defWindowProc(hWnd, msg, wParam, lParam);

	if (msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST)
	{
		MSG msgStruct = { hWnd, msg, wParam, lParam };
		sendMessage(propertyCtrl->m_hWndToolTip, TTM_RELAYEVENT, 0, (LPARAM) &msgStruct);
	}

	if (msg >= WM_NCMOUSEFIRST && msg <= WM_NCMOUSELAST)
		sendMessage(propertyCtrl->m_hWndToolTip, TTM_ACTIVATE, false, 0);

	switch (msg)
	{
	case WM_MOUSEACTIVATE:
		lResult = axl_win_TPropertyCtrl_IsMenu(propertyCtrl) ? MA_NOACTIVATE : defWindowProc(hWnd, msg, wParam, lParam);
		break;

	case WM_DESTROY:
		lResult = axl_win_TPropertyCtrl_OnDestroy(propertyCtrl);
		break;

	case WM_GETDLGCODE:
		lResult = DLGC_WANTTAB | DLGC_WANTARROWS | DLGC_WANTCHARS;
		break;

	case WM_SETFOCUS:
		lResult = axl_win_TPropertyCtrl_OnSetFocus(propertyCtrl);
		break;

	case WM_KILLFOCUS:
		lResult = axl_win_TPropertyCtrl_OnKillFocus(propertyCtrl);
		break;

	case WM_ERASEBKGND:
		break;

	case WM_PAINT:
		lResult = axl_win_TPropertyCtrl_OnPaint(propertyCtrl);
		break;

    case WM_THEMECHANGED:
		axl_gr_TStockCtrlPaint_CloseThemes(&propertyCtrl->m_stockCtrlPaint);
		break;

	case WM_NCPAINT:
		axl_gr_TStockCtrlPaint_NcPaintEdge(&propertyCtrl->m_stockCtrlPaint, hWnd, (HRGN) wParam);
		break;

	case WM_SIZE:
		lResult = axl_win_TPropertyCtrl_OnSize(propertyCtrl);
		break;

	case WM_HSCROLL:
		axl_win_TPropertyCtrl_OnScroll(propertyCtrl, SB_HORZ, LOWORD(wParam));
		break;

	case WM_VSCROLL:
		axl_win_TPropertyCtrl_OnScroll(propertyCtrl, SB_VERT, LOWORD(wParam));
		break;

	case WM_SETCURSOR:
		lResult = axl_win_TPropertyCtrl_OnSetCursor(propertyCtrl, wParam, lParam);
		break;

	case WM_LBUTTONDOWN:
		lResult = axl_win_TPropertyCtrl_OnLButtonDown(propertyCtrl, (short) LOWORD(lParam), (short) HIWORD(lParam));
		break;

	case WM_LBUTTONUP:
		lResult = axl_win_TPropertyCtrl_OnLButtonUp(propertyCtrl, (short) LOWORD(lParam), (short) HIWORD(lParam));
		break;

	case WM_LBUTTONDBLCLK:
		lResult = axl_win_TPropertyCtrl_OnLButtonDblClk(propertyCtrl, (short) LOWORD(lParam), (short) HIWORD(lParam));
		break;		

	case WM_RBUTTONDOWN:
		lResult = axl_win_TPropertyCtrl_OnRButtonDown(propertyCtrl, (short) LOWORD(lParam), (short) HIWORD(lParam));
		break;

	case WM_RBUTTONUP:
		lResult = axl_win_TPropertyCtrl_OnRButtonUp(propertyCtrl, (short) LOWORD(lParam), (short) HIWORD(lParam));
		break;

	case WM_MOUSEMOVE:
		lResult = axl_win_TPropertyCtrl_OnMouseMove(propertyCtrl, (short) LOWORD(lParam), (short) HIWORD(lParam));
		break;

	case WM_MOUSELEAVE:
		lResult = axl_win_TPropertyCtrl_OnMouseLeave(propertyCtrl);
		break;

	case WM_MOUSEWHEEL:
		lResult = axl_win_TPropertyCtrl_OnMouseWheel(propertyCtrl, HIWORD(wParam));
		break;

	case WM_CAPTURECHANGED:
		axl_win_TPropertyCtrl_StopDrag(propertyCtrl);
		break;

	case WM_KEYDOWN:
		axl_win_TPropertyCtrl_OnKeyDown(propertyCtrl, (int) wParam);
		break;

	case WM_KEYUP:
		axl_win_TPropertyCtrl_OnKeyUp(propertyCtrl, (int) wParam);
		break;

	case WM_TIMER:
		lResult = axl_win_TPropertyCtrl_OnTimer(propertyCtrl, (int) wParam);
		break;

	default:
		lResult = defWindowProc(hWnd, msg, wParam, lParam);
	}
 
	return lResult;
}


//.............................................................................
