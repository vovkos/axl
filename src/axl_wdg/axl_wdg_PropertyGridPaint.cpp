#include "axl_Precomp.h"
#include "axl_win_PropertyCtrl.h"

//.............................................................................

HIMAGELIST 
AXL_API
axl_win_TPropertyCtrl_CreatePlusMinusImageList(
	ulong_t cx, 
	ulong_t cy,
	COLORREF* pPalette
	)
{
	HIMAGELIST hImageList;

	POINT Center =  { cx / 2, cy / 2 };

	RECT Rect      = { 0, 0, cx, cy };
	RECT RectInner = { 1, 1, cx - 1, cy - 1 };
	RECT RectMinus = { 2, Center.y, cx - 2, Center.y + 1 };
	RECT RectPlus  = { Center.x, 2, Center.x + 1, cy - 2 };

	HDC hWindowDC = GetDC(NULL);
	HDC hdc = CreateCompatibleDC(hWindowDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hWindowDC, cx, cy);
	HBITMAP hOldBitmap = SelectObject(hdc, hBitmap);
	ReleaseDC(NULL, hWindowDC);

	hImageList = ImageList_Create(cx, cy, ILC_COLORDDB | ILC_MASK, 0, 1);
	ImageList_SetBkColor(hImageList, CLR_NONE);

	// minus

	axl_gr_FastRectangleEx(hdc, &Rect,      pPalette[axl_gr_GetColorIndex(axl_win_EPropertyColor_PlusMinusRect)]);
	axl_gr_FastRectangleEx(hdc, &RectInner, pPalette[axl_gr_GetColorIndex(axl_win_EPropertyColor_PlusMinusBack)]);
	axl_gr_FastRectangleEx(hdc, &RectMinus, pPalette[axl_gr_GetColorIndex(axl_win_EPropertyColor_PlusMinus)]);

	SelectObject(hdc, hOldBitmap);
	ImageList_AddMasked(hImageList, hBitmap, pPalette[axl_gr_GetColorIndex(axl_win_EPropertyColor_Unused)]);
	SelectObject(hdc, hBitmap);

	// plus

	axl_gr_FastRectangleEx(hdc, &Rect,      pPalette[axl_gr_GetColorIndex(axl_win_EPropertyColor_PlusMinusRect)]);
	axl_gr_FastRectangleEx(hdc, &RectInner, pPalette[axl_gr_GetColorIndex(axl_win_EPropertyColor_PlusMinusBack)]);
	axl_gr_FastRectangleEx(hdc, &RectMinus, pPalette[axl_gr_GetColorIndex(axl_win_EPropertyColor_PlusMinus)]);
	axl_gr_FastRectangleEx(hdc, &RectPlus,  pPalette[axl_gr_GetColorIndex(axl_win_EPropertyColor_PlusMinus)]);

	SelectObject(hdc, hOldBitmap);
	ImageList_AddMasked(hImageList, hBitmap, pPalette[axl_gr_GetColorIndex(axl_win_EPropertyColor_Unused)]);
	SelectObject(hdc, hBitmap);

	SelectObject(hdc, hOldBitmap);
	DeleteDC(hdc);
	DeleteObject(hBitmap);

	return hImageList;
}

HIMAGELIST 
AXL_API
axl_win_TPropertyCtrl_CreateGridImageList(
	ulong_t cx, 
	ulong_t cy,
	COLORREF* pPalette
	)
{
	HIMAGELIST hImageList;

	POINT Center =  { (cx / 2 - 1) | 1, (cy / 2 - 1) | 1 }; // center must be odd

	RECT _GridLines[] = 
	{
		{ 1, 0, cx, 0 },                // H
		{ 0, 1, 0, cy },                // V
		{ Center.x, 1, cx, cy },        // T
		{ Center.x, 1, cx, Center.y },  // L
	};

	RECT* p = _GridLines;
	RECT* pEnd = p + sizeof(_GridLines) / sizeof(RECT);

	RECT Rect = { 0, 0, cx, cy };

	HDC hWindowDC = GetDC(NULL);
	HDC hdc = CreateCompatibleDC(hWindowDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hWindowDC, cx, cy);
	HBITMAP hOldBitmap = SelectObject(hdc, hBitmap);
	ReleaseDC(NULL, hWindowDC);

	hImageList = ImageList_Create(cx, cy, ILC_COLORDDB | ILC_MASK, 0, 1);
	ImageList_SetBkColor(hImageList, CLR_NONE);

	for (; p < pEnd; p++)
	{
		int i;

		axl_gr_FastRectangleEx(hdc, &Rect, pPalette[axl_gr_GetColorIndex(axl_win_EPropertyColor_Unused)]);

		for (i = p->top; i < p->bottom; i += 2)
			SetPixel(hdc, Center.x, i, pPalette[axl_gr_GetColorIndex(axl_win_EPropertyColor_Grid)]);
		
		for (i = p->left; i < p->right; i += 2)
			SetPixel(hdc, i, Center.y, pPalette[axl_gr_GetColorIndex(axl_win_EPropertyColor_Grid)]);

		SelectObject(hdc, hOldBitmap);
		ImageList_AddMasked(hImageList, hBitmap, pPalette[axl_gr_GetColorIndex(axl_win_EPropertyColor_Unused)]);
		SelectObject(hdc, hBitmap);
	}

	SelectObject(hdc, hOldBitmap);
	DeleteDC(hdc);
	DeleteObject(hBitmap);

	return hImageList;
}

static
HBITMAP 
AXL_API
CreateColorBitmap(
	ulong_t cx,
	ulong_t cy,
	COLORREF Color
	)
{
	HDC hdcDesktop = GetDC(NULL);
	HDC hdc = CreateCompatibleDC(hdcDesktop);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcDesktop, cx, cy);
	HBITMAP hOldBitmap = SelectObject(hdc, hBitmap);
	RECT Rect = { 0, 0, cx, cy };

	axl_gr_FastRectangleEx(hdc, &Rect, Color);

	SelectObject(hdc, hOldBitmap);
	DeleteDC(hdc);
	ReleaseDC(NULL, hdcDesktop);
	return hBitmap;
}


HIMAGELIST 
AXL_API
axl_win_TPropertyCtrl_CreateMenuArrowImageList()
{
	COLORREF Colors[] = 
	{
		GetSysColor(COLOR_WINDOWTEXT),
		GetSysColor(COLOR_HIGHLIGHTTEXT),
		GetSysColor(COLOR_GRAYTEXT),
	};

	size_t i;

	static WORD _Mask[8] = { 0xff, 0xef, 0xe7, 0xe3, 0xe1, 0xe3, 0xe7, 0xef };
	HBITMAP hMaskBitmap = CreateBitmap(8, 8, 1, 1, _Mask);

	HIMAGELIST hImageList = ImageList_Create(8, 8, ILC_COLORDDB | ILC_MASK, 0, 1);
	ImageList_SetBkColor(hImageList, CLR_NONE);

	for (i = 0; i < sizeof(Colors) / sizeof(COLORREF); i++)
	{
		HBITMAP hBitmap = CreateColorBitmap(8, 8, Colors[i]);
		ImageList_Add(hImageList, hBitmap, hMaskBitmap);
		DeleteObject(hBitmap);
	}

	DeleteObject(hMaskBitmap);
	return hImageList;
}

//.............................................................................

void
AXL_API
axl_win_TPropertyCtrl_PaintGridLines(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	const RECT* pRect,
	axl_win_TPropertyLine* pLine
	)
{ 
	axl_gr_TTextPaint* pPaint = &pPropertyCtrl->m_TextPaint;

	int i;
	long x = axl_win_TPropertyCtrl_IsMenu(pPropertyCtrl) ? 
		(pLine->m_Indent) * pPropertyCtrl->m_IndentWidth - pPropertyCtrl->m_FirstVisibleX :
		(pLine->m_Level + pLine->m_Indent) * pPropertyCtrl->m_IndentWidth - pPropertyCtrl->m_FirstVisibleX;

	axl_win_TPropertyLine* pParent = pLine->m_pParent;
 
	if (axl_win_TPropertyLine_HasGridLines(pLine))
		for (i = 0; i < pLine->m_Indent; i++, x -= pPropertyCtrl->m_IndentWidth)
		{
			ImageList_Draw(
				pPropertyCtrl->m_hGridImageList, 
				axl_win_EPropertyGridLineImage_H,			
				pPaint->m_hdc, 
				x, pRect->top, 
				ILD_NORMAL
				);
		}

	if (axl_win_TPropertyLine_HasGridLines(pLine))
	{
		axl_win_TPropertyLine* pNext = axl_win_TPropertyLine_GetNextSibling(pLine);
		int Image = pNext && !(pNext->m_Flags & axl_win_EPropertyLine_ForceNoGridLines) ? 
			axl_win_EPropertyGridLineImage_T : 
			axl_win_EPropertyGridLineImage_L 
			;

		ImageList_Draw(
			pPropertyCtrl->m_hGridImageList, 
			Image,
			pPaint->m_hdc, 
			x, pRect->top, 
			ILD_NORMAL
			);
	}

	if (axl_win_TPropertyCtrl_IsMenu(pPropertyCtrl))
		return;

	pLine = pLine->m_pParent;

	while (pLine)
	{
		x -= (pLine->m_Indent + 1) * pPropertyCtrl->m_IndentWidth;

		if (axl_win_TPropertyLine_HasGridLines(pLine) && !axl_win_TPropertyLine_IsLastChild(pLine))
			ImageList_Draw(
				pPropertyCtrl->m_hGridImageList, 
				axl_win_EPropertyGridLineImage_V,
				pPaint->m_hdc, 
				x, pRect->top, 
				ILD_NORMAL
				);

		pLine = pLine->m_pParent;
	}
}

void
AXL_API
axl_win_TPropertyCtrl_PaintOptionBox(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	const RECT* pRect,
	axl_win_TPropertyLine* pLine
	)
{
	axl_gr_TTextPaint* pPaint = &pPropertyCtrl->m_TextPaint;

	RECT Rect;

	long x = axl_win_TPropertyCtrl_IsMenu(pPropertyCtrl) ? 0 : pLine->m_Level * pPropertyCtrl->m_IndentWidth;
	x += (pLine->m_Indent + 1) * pPropertyCtrl->m_IndentWidth - pPropertyCtrl->m_FirstVisibleX;

	Rect.left = pPropertyCtrl->m_OptionBoxRect.left + x;
	Rect.top = pPropertyCtrl->m_OptionBoxRect.top + pRect->top;
	Rect.right = pPropertyCtrl->m_OptionBoxRect.right + x;
	Rect.bottom = pPropertyCtrl->m_OptionBoxRect.bottom + pRect->top;

	switch (pLine->m_OptionBoxType)
	{
	case axl_win_EPropertyOptionBoxType_RadioButton:
		axl_gr_TStockCtrlPaint_DrawRadioButton(
			&pPropertyCtrl->m_StockCtrlPaint, 
			pPaint->m_hdc, 
			&Rect, 
			pLine->m_OptionBoxCheckState, 
			(pLine->m_Flags & axl_win_EPropertyLine_IsDisabled) ? axl_win_EPropertyButtonVolatileState_Disabled : pLine->m_OptionBoxVolatileState
			);

		break;

	case axl_win_EPropertyOptionBoxType_CheckBox:
		axl_gr_TStockCtrlPaint_DrawCheckBox(
			&pPropertyCtrl->m_StockCtrlPaint, 
			pPaint->m_hdc, 
			&Rect, 
			pLine->m_OptionBoxCheckState, 
			(pLine->m_Flags & axl_win_EPropertyLine_IsDisabled) ? axl_win_EPropertyButtonVolatileState_Disabled : pLine->m_OptionBoxVolatileState
			);

		break;
	}
}

void
AXL_API
axl_win_TPropertyCtrl_PaintLineImages(
	axl_win_TPropertyCtrl* pPropertyCtrl, 
	const RECT* pRect,
	axl_win_TPropertyLine* pLine,
	bool_t IsSelected
	)
{
	axl_gr_TTextPaint* pPaint = &pPropertyCtrl->m_TextPaint;

	long x = axl_win_TPropertyCtrl_IsMenu(pPropertyCtrl) ? 
		-(long) pPropertyCtrl->m_FirstVisibleX :
		pLine->m_Level * pPropertyCtrl->m_IndentWidth - pPropertyCtrl->m_FirstVisibleX;

	if (axl_win_TPropertyLine_HasPlusMinus(pLine))
	{
		int Image = (pLine->m_Flags & axl_win_EPropertyLine_IsExpanded) ? 
			axl_win_EPropertyPlusMinusImage_Minus : 
			axl_win_EPropertyPlusMinusImage_Plus;

		ImageList_Draw(
			pPropertyCtrl->m_hPlusMinusImageList, 
			Image,			
			pPaint->m_hdc, 
			x + pPropertyCtrl->m_PlusMinusRect.left, 
			pRect->top + pPropertyCtrl->m_PlusMinusRect.top, 
			ILD_NORMAL
			); 
	}

	x += (pLine->m_Indent + 1) * pPropertyCtrl->m_IndentWidth;

	if (pLine->m_OptionBoxType)
		x += pPropertyCtrl->m_OptionBoxWidth;

	if (pLine->m_Icon != -1)
	{
		long xIcon = x + pPropertyCtrl->m_IconRect.left;
		long yIcon = pRect->top + pPropertyCtrl->m_IconRect.top;

		if (axl_win_TPropertyLine_IsButton(pLine) && pLine->m_ButtonVolatileState == axl_win_EPropertyButtonVolatileState_Pushed)
			xIcon++, yIcon++;

		ImageList_Draw(
			pPropertyCtrl->m_hImageList, 
			pLine->m_Icon,
			pPaint->m_hdc, 
			xIcon, 
			yIcon, 
			ILD_NORMAL
			); 
	}

	if ((pLine->m_Flags & axl_win_EPropertyLine_Menu) && !axl_rtl_TList_IsEmpty(&pLine->m_ChildrenList))
	{
		long xArrow = pLine->m_MenuWidth + pPropertyCtrl->m_Margins.left + pPropertyCtrl->m_Margins.right;
		long yArrow = (pRect->top + pRect->bottom - 8) / 2;	

		if (axl_win_TPropertyLine_IsButton(pLine) && pLine->m_ButtonVolatileState == axl_win_EPropertyButtonVolatileState_Pushed)
			xArrow++, yArrow++;

		ImageList_Draw(
			pPropertyCtrl->m_hMenuArrowImageList, 
			(pLine->m_Flags & axl_win_EPropertyLine_IsDisabled) ? 2 : IsSelected ? 1 : 0,
			pPaint->m_hdc, 
			xArrow,
			yArrow,
			ILD_NORMAL
			);
	}
}

void 
AXL_API
axl_win_TPropertyCtrl_PaintLeftPane(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	const RECT* pRect,
	axl_win_TPropertyLine* pLine,
	bool_t IsSelected
	)
{
	axl_gr_TTextPaint* pPaint = &pPropertyCtrl->m_TextPaint;
	axl_rtl_TString* pString;

	bool_t ShouldDrawButton;
	RECT TextRect = *pRect;

	long x = axl_win_TPropertyLine_GetPartX(pLine, axl_win_EPropertyLinePart_Name) - pPropertyCtrl->m_FirstVisibleX + pPropertyCtrl->m_TextOrigin.x;
	long y = pRect->top + pPropertyCtrl->m_TextOrigin.y;

	if (axl_win_TPropertyLine_HasNameDelimiter(pLine))
	{
		RECT DelimRect = *pRect;

		TextRect.bottom--;
		DelimRect.top = TextRect.bottom;

		axl_gr_TTextPaint_SetBackColor(pPaint, axl_win_EPropertyColor_NameDelimiter);
		axl_gr_FastRectangle(pPaint->m_hdc, &DelimRect);
	}

	if (axl_win_TPropertyLine_IsButton(pLine) && pLine->m_ButtonVolatileState == axl_win_EPropertyButtonVolatileState_Pushed)
		x++, y++;

	pPaint->m_StdAttr.m_TextColor = axl_win_EPropertyColor_Name;
	pPaint->m_StdAttr.m_BackColor = axl_win_EPropertyColor_NameBack;
	pPaint->m_StdAttr.m_FontFlags = 0;

	axl_gr_TTextAttr_Overlay(&pPaint->m_StdAttr, &pLine->m_NameAttr);

	if (IsSelected)
	{
		pPaint->m_StdAttr.m_TextColor = axl_win_EPropertyColor_SelectedName;
		pPaint->m_StdAttr.m_BackColor = axl_win_EPropertyColor_SelectedBack;
	}

	if (pLine->m_Flags & axl_win_EPropertyLine_IsDisabled)
		pPaint->m_StdAttr.m_TextColor = axl_win_EPropertyColor_DisabledText;
	else if (pLine->m_ButtonVolatileState == axl_win_EPropertyButtonVolatileState_Hover)
		pPaint->m_StdAttr.m_TextColor = axl_win_EPropertyColor_HoverButtonText;
	else if (pLine->m_ButtonVolatileState)
		pPaint->m_StdAttr.m_TextColor = axl_win_EPropertyColor_ButtonText;

	axl_gr_TTextPaint_SetStdAttr(pPaint);

	axl_gr_FastRectangle(pPaint->m_hdc, &TextRect);

	ShouldDrawButton = 
		axl_win_TPropertyLine_IsButton(pLine) && 
		(pPropertyCtrl->m_DoAlwaysDrawButtons || 
		(pLine->m_Flags & axl_win_EPropertyLine_AlwaysDrawButton) ||
		IsSelected || 
		pLine->m_ButtonVolatileState || 
		pLine->m_ButtonCheckState);

	if (ShouldDrawButton)
	{
		if (pLine->m_Flags & axl_win_EPropertyLine_Button)
		{
			RECT Rect;
			Rect.left = axl_win_TPropertyLine_GetPartX(pLine, axl_win_EPropertyLinePart_Button);
			Rect.right = pLine->m_SinglePaneWidth + pPropertyCtrl->m_Margins.left + 2 * pPropertyCtrl->m_Margins.right;
			Rect.top = pRect->top;
			Rect.bottom = pRect->bottom - 1;

			Rect.left -= pPropertyCtrl->m_FirstVisibleX;
			Rect.right -= pPropertyCtrl->m_FirstVisibleX;

			axl_gr_TStockCtrlPaint_DrawPushButton(
				&pPropertyCtrl->m_StockCtrlPaint, 
				pPaint->m_hdc, 
				&Rect, 
				pLine->m_ButtonCheckState,
				(pLine->m_Flags & axl_win_EPropertyLine_IsDisabled) ? axl_win_EPropertyButtonVolatileState_Disabled : pLine->m_ButtonVolatileState
				);
		}
		else if (pLine->m_Flags & axl_win_EPropertyLine_Menu)
		{
			RECT Rect;
			Rect.left = axl_win_TPropertyLine_GetPartX(pLine, axl_win_EPropertyLinePart_Button);
			Rect.right = pLine->m_MenuWidth + pPropertyCtrl->m_Margins.left + pPropertyCtrl->m_Margins.right + GetSystemMetrics(SM_CXVSCROLL);
			Rect.top = pRect->top;
			Rect.bottom = pRect->bottom - 1;

			Rect.left -= pPropertyCtrl->m_FirstVisibleX;
			Rect.right -= pPropertyCtrl->m_FirstVisibleX;

			axl_gr_TStockCtrlPaint_DrawToolbarButton(
				&pPropertyCtrl->m_StockCtrlPaint, 
				pPaint->m_hdc, 
				&Rect, 
				pLine->m_ButtonCheckState,
				(pLine->m_Flags & axl_win_EPropertyLine_IsDisabled) ? axl_win_EPropertyButtonVolatileState_Disabled : pLine->m_ButtonVolatileState
				);
		}
	}

	pString = (pLine->m_Flags & axl_win_EPropertyLine_NameHyperText) ? &pLine->m_NameHyperText.m_Text : &pLine->m_Name;

	SetBkMode(pPaint->m_hdc, TRANSPARENT);
	pPaint->m_EtoFlags = 0;
	pPaint->m_top = TextRect.top;
	pPaint->m_bottom = TextRect.bottom;

	if ((pLine->m_Flags & axl_win_EPropertyLine_NameShadow) && 
		!(pLine->m_Flags & axl_win_EPropertyLine_IsDisabled) &&
		!ShouldDrawButton)
	{
		COLORREF Color = GetBkColor(pPaint->m_hdc);
		Color = RGB(GetRValue(Color) * 4/ 5, GetGValue(Color) * 4 / 5, GetBValue(Color) * 4 / 5); // calc shadow color
		SetTextColor(pPaint->m_hdc, Color);

		pPaint->m_x = x + 2;
		pPaint->m_y = y + 2;

		axl_gr_TTextPaint_PaintText(pPaint, pString->m_p, axl_rtl_TString_GetLength(pString));
		axl_gr_TTextPaint_SetStdTextColor(pPaint);
	}

	pPaint->m_x = x;
	pPaint->m_y = y;

	if (pLine->m_Flags & axl_win_EPropertyLine_NameHyperText)
		axl_gr_TTextPaint_PaintHyperText(
			pPaint, 
			pString->m_p, 
			axl_rtl_TString_GetLength(pString),
			&pLine->m_NameHyperText.m_AttrArray
			);
	else
		axl_gr_TTextPaint_PaintText(pPaint, pString->m_p, axl_rtl_TString_GetLength(pString));

	// option box must be draw first to minimize flickering (BitBlt flushes DirectDraw buffer)

	if (pLine->m_OptionBoxType)
		axl_win_TPropertyCtrl_PaintOptionBox(pPropertyCtrl, &TextRect, pLine);

	axl_win_TPropertyCtrl_PaintGridLines(pPropertyCtrl, pRect, pLine);
	axl_win_TPropertyCtrl_PaintLineImages(pPropertyCtrl, &TextRect, pLine, IsSelected);

	if (axl_win_TPropertyLine_IsSinglePane(pLine) && axl_win_TPropertyLine_HasValueDelimiter(pLine))
	{
		RECT DelimRect = *pRect;

		DelimRect.top = pRect->bottom - 1;
		DelimRect.left = pPropertyCtrl->m_SplitterPos - pPropertyCtrl->m_FirstVisibleX;

		axl_gr_TTextPaint_SetBackColor(pPaint, axl_win_EPropertyColor_ValueDelimiter);
		axl_gr_FastRectangle(pPaint->m_hdc, &DelimRect);
	}
}

void 
AXL_API
axl_win_TPropertyCtrl_PaintRightPane(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	const RECT* pRect,
	axl_win_TPropertyLine* pLine,
	bool_t IsSelected
	)
{
	axl_gr_TTextPaint* pPaint = &pPropertyCtrl->m_TextPaint;
	axl_rtl_TString* pString;

	RECT TextRect = *pRect;

	long x = pRect->left + pPropertyCtrl->m_TextOrigin.x;
	long y = pRect->top + pPropertyCtrl->m_TextOrigin.y;

	if (axl_win_TPropertyLine_HasValueDelimiter(pLine))
	{
		RECT DelimRect = *pRect;

		TextRect.bottom--;
		DelimRect.top = TextRect.bottom;

		axl_gr_TTextPaint_SetBackColor(pPaint, axl_win_EPropertyColor_ValueDelimiter);
		axl_gr_FastRectangle(pPaint->m_hdc, &DelimRect);
	}

	pPaint->m_StdAttr.m_TextColor = axl_win_EPropertyColor_Value;
	pPaint->m_StdAttr.m_BackColor = axl_win_EPropertyColor_ValueBack;
	pPaint->m_StdAttr.m_FontFlags = 0;

	axl_gr_TTextAttr_Overlay(&pPaint->m_StdAttr, &pLine->m_ValueAttr);

	if (IsSelected)
	{
		pPaint->m_StdAttr.m_TextColor = axl_win_EPropertyColor_SelectedValue;
		pPaint->m_StdAttr.m_BackColor = axl_win_EPropertyColor_SelectedBack;
	}

	if (pLine->m_Flags & axl_win_EPropertyLine_IsDisabled)
		pPaint->m_StdAttr.m_TextColor = axl_win_EPropertyColor_DisabledText;

	axl_gr_TTextPaint_SetStdAttr(pPaint);

	axl_gr_FastRectangle(pPaint->m_hdc, &TextRect);

	pString = (pLine->m_Flags & axl_win_EPropertyLine_ValueHyperText) ? &pLine->m_ValueHyperText.m_Text : &pLine->m_Value;

	SetBkMode(pPaint->m_hdc, TRANSPARENT);
	pPaint->m_EtoFlags = 0;
	pPaint->m_x = x;
	pPaint->m_y = y;
	pPaint->m_top = TextRect.top;
	pPaint->m_bottom = TextRect.bottom;

	if (pLine->m_Flags & axl_win_EPropertyLine_ValueHyperText)
		axl_gr_TTextPaint_PaintHyperText(
			pPaint, 
			pString->m_p, 
			axl_rtl_TString_GetLength(pString),
			&pLine->m_ValueHyperText.m_AttrArray
			);
	else
		axl_gr_TTextPaint_PaintText(pPaint, pString->m_p, axl_rtl_TString_GetLength(pString));
}

void 
AXL_API
axl_win_TPropertyCtrl_PaintLine(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	const RECT* pRect,
	axl_win_TPropertyLine* pLine,
	bool_t IsSelected
	)
{
	axl_gr_TTextPaint* pPaint = &pPropertyCtrl->m_TextPaint;

	RECT NameRect;
	RECT ValueRect;
	RECT SplitterRect;

#ifndef _AXL_USE_OFFSCREEN_BUFFER
	HRGN hRgnClip;
	HRGN hOldRgnClip;
	int OldClipRgn;
	POINT ViewportOrg;
#endif

	if (axl_win_TPropertyLine_IsSinglePane(pLine))
	{
		axl_win_TPropertyCtrl_PaintLeftPane(pPropertyCtrl, pRect, pLine, IsSelected);
		return;
	}

	NameRect = ValueRect = SplitterRect = *pRect;
	NameRect.right = pPropertyCtrl->m_SplitterPos - pPropertyCtrl->m_FirstVisibleX;
	SplitterRect.left = NameRect.right;
	SplitterRect.right = SplitterRect.left + 1;
	ValueRect.left = SplitterRect.right;

#ifndef _AXL_USE_OFFSCREEN_BUFFER
	hOldRgnClip = CreateRectRgn(0,0,0,0);
	OldClipRgn = GetClipRgn(pPaint->m_hdc, hOldRgnClip);

	// clip rgn is NOT affected by viewport org, so we have to adjust it accordingly
	
	GetViewportOrgEx(pPaint->m_hdc, &ViewportOrg);
	hRgnClip = CreateRectRgn(
		ViewportOrg.x, 
		ViewportOrg.y + pRect->top, 
		ViewportOrg.x + NameRect.right, 
		ViewportOrg.y + pRect->bottom
		);

	ExtSelectClipRgn(pPaint->m_hdc, hRgnClip, RGN_AND);
#endif
	
	axl_win_TPropertyCtrl_PaintLeftPane(pPropertyCtrl, &NameRect, pLine, IsSelected);

#ifndef _AXL_USE_OFFSCREEN_BUFFER
	SelectClipRgn(pPaint->m_hdc, OldClipRgn > 0 ? hOldRgnClip : NULL);	
	DeleteObject(hRgnClip);

	if (hOldRgnClip)
		DeleteObject(hOldRgnClip);
#endif

	axl_gr_TTextPaint_SetBackColor(pPaint, axl_win_EPropertyColor_Splitter);
	axl_gr_FastRectangle(pPaint->m_hdc, &SplitterRect);

	axl_win_TPropertyCtrl_PaintRightPane(pPropertyCtrl, &ValueRect, pLine, IsSelected);
}

void 
AXL_API
axl_win_TPropertyCtrl_PaintRect(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	HDC hdc,
	const RECT* pRect
	)
{
	axl_gr_TTextPaint* pPaint = &pPropertyCtrl->m_TextPaint;

	RECT LineRect;

#ifdef _AXL_USE_OFFSCREEN_BUFFER
	RECT OffscreenRect;
	HDC hdcOffscreen;
#endif

	long FirstLine = pRect->top / pPropertyCtrl->m_LineHeight;
	long LastLine = pRect->bottom / pPropertyCtrl->m_LineHeight;
	size_t LineCount = axl_rtl_TPtrArray_GetCount(&pPropertyCtrl->m_VisibleLineArray);

	if (pRect->bottom % pPropertyCtrl->m_LineHeight)
		LastLine++;

	ASSERT(pRect->left >= 0 && pRect->top >= 0 && pRect->left <= pRect->right && pRect->top <= pRect->bottom);

	LineRect.left = pRect->left;
	LineRect.top = FirstLine * pPropertyCtrl->m_LineHeight;
	LineRect.right = pRect->right;
	LineRect.bottom = LineRect.top + pPropertyCtrl->m_LineHeight;

#ifdef _AXL_USE_OFFSCREEN_BUFFER
	OffscreenRect.left = pRect->left;
	OffscreenRect.top = 0;
	OffscreenRect.right = pRect->right;
	OffscreenRect.bottom = pPropertyCtrl->m_LineHeight;

	hdcOffscreen = axl_gr_TOffscreenBuffer_GetBuffer(&pPropertyCtrl->m_OffscreenBuffer, hdc, pRect->right - pRect->left, pPropertyCtrl->m_LineHeight);
	SetViewportOrgEx(hdcOffscreen, -pRect->left, 0, NULL);

	axl_gr_TTextPaint_Attach(&pPropertyCtrl->m_TextPaint, hdcOffscreen);
#else
	axl_gr_TTextPaint_Attach(&pPropertyCtrl->m_TextPaint, hdc);
#endif

	FirstLine += (long) pPropertyCtrl->m_FirstVisibleLine;
	LastLine += (long) pPropertyCtrl->m_FirstVisibleLine;

	if (LastLine > (long) LineCount)
		LastLine = (long) LineCount;

	if (FirstLine < LastLine)
	{
		axl_win_TPropertyLine** ppLine = (axl_win_TPropertyLine**) pPropertyCtrl->m_VisibleLineArray.m_p + FirstLine;
		axl_win_TPropertyLine** ppEnd = ppLine + LastLine - FirstLine;
		axl_win_TPropertyLine* pSelectedLine = axl_win_TPropertyCtrl_GetSelectedLine(pPropertyCtrl);

		while (ppLine < ppEnd)
		{	
			axl_win_TPropertyLine* pLine = *ppLine;

#ifdef _AXL_USE_OFFSCREEN_BUFFER
			axl_win_TPropertyCtrl_PaintLine(pPropertyCtrl, &OffscreenRect, pLine, pLine == pSelectedLine);
			BitBlt(
				hdc, 
				LineRect.left, LineRect.top, 
				LineRect.right - LineRect.left,
				LineRect.bottom - LineRect.top,
				hdcOffscreen,
				OffscreenRect.left, OffscreenRect.top,
				SRCCOPY
				);
#else
			axl_win_TPropertyCtrl_PaintLine(pPropertyCtrl, &LineRect, pLine, pLine == pSelectedLine);
#endif

			ppLine++;

			LineRect.top += pPropertyCtrl->m_LineHeight;
			LineRect.bottom += pPropertyCtrl->m_LineHeight;
		}
	}

	if (pRect->bottom > LineRect.top)
	{
		COLORREF Color = pPropertyCtrl->m_Palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_Empty)];
		LineRect.bottom = pRect->bottom;
		axl_gr_FastRectangleEx(hdc, &LineRect, Color);
	}

	axl_gr_TTextPaint_Detach(&pPropertyCtrl->m_TextPaint);
}

void 
AXL_API
axl_win_TPropertyCtrl_InvalidateLineRange(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	size_t FromLine,
	size_t ToLine
	)
{
	RECT Rect;

	if (!IsWindow(pPropertyCtrl->m_hWnd))
		return;

	GetClientRect(pPropertyCtrl->m_hWnd, &Rect);
	Rect.top = (long) (FromLine - pPropertyCtrl->m_FirstVisibleLine) * pPropertyCtrl->m_LineHeight;

	if (ToLine != -1)
		Rect.bottom = (long) (ToLine - pPropertyCtrl->m_FirstVisibleLine + 1) * pPropertyCtrl->m_LineHeight;

	InvalidateRect(pPropertyCtrl->m_hWnd, &Rect, false);
}

//.............................................................................
