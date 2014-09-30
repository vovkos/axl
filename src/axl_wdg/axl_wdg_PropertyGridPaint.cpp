#include "axl_Precomp.h"
#include "axl_win_PropertyCtrl.h"

//.............................................................................

HIMAGELIST 
AXL_API
axl_win_TPropertyCtrl_CreatePlusMinusImageList(
	ulong_t cx, 
	ulong_t cy,
	COLORREF* palette
	)
{
	HIMAGELIST hImageList;

	POINT center =  { cx / 2, cy / 2 };

	RECT rect      = { 0, 0, cx, cy };
	RECT rectInner = { 1, 1, cx - 1, cy - 1 };
	RECT rectMinus = { 2, center.y, cx - 2, center.y + 1 };
	RECT rectPlus  = { center.x, 2, center.x + 1, cy - 2 };

	HDC hWindowDC = getDC(NULL);
	HDC hdc = createCompatibleDC(hWindowDC);
	HBITMAP hBitmap = createCompatibleBitmap(hWindowDC, cx, cy);
	HBITMAP hOldBitmap = selectObject(hdc, hBitmap);
	releaseDC(NULL, hWindowDC);

	hImageList = imageList_Create(cx, cy, ILC_COLORDDB | ILC_MASK, 0, 1);
	imageList_SetBkColor(hImageList, CLR_NONE);

	// minus

	axl_gr_FastRectangleEx(hdc, &rect,      palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_PlusMinusRect)]);
	axl_gr_FastRectangleEx(hdc, &rectInner, palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_PlusMinusBack)]);
	axl_gr_FastRectangleEx(hdc, &rectMinus, palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_PlusMinus)]);

	selectObject(hdc, hOldBitmap);
	imageList_AddMasked(hImageList, hBitmap, palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_Unused)]);
	selectObject(hdc, hBitmap);

	// plus

	axl_gr_FastRectangleEx(hdc, &rect,      palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_PlusMinusRect)]);
	axl_gr_FastRectangleEx(hdc, &rectInner, palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_PlusMinusBack)]);
	axl_gr_FastRectangleEx(hdc, &rectMinus, palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_PlusMinus)]);
	axl_gr_FastRectangleEx(hdc, &rectPlus,  palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_PlusMinus)]);

	selectObject(hdc, hOldBitmap);
	imageList_AddMasked(hImageList, hBitmap, palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_Unused)]);
	selectObject(hdc, hBitmap);

	selectObject(hdc, hOldBitmap);
	deleteDC(hdc);
	deleteObject(hBitmap);

	return hImageList;
}

HIMAGELIST 
AXL_API
axl_win_TPropertyCtrl_CreateGridImageList(
	ulong_t cx, 
	ulong_t cy,
	COLORREF* palette
	)
{
	HIMAGELIST hImageList;

	POINT center =  { (cx / 2 - 1) | 1, (cy / 2 - 1) | 1 }; // center must be odd

	RECT _GridLines[] = 
	{
		{ 1, 0, cx, 0 },                // H
		{ 0, 1, 0, cy },                // V
		{ center.x, 1, cx, cy },        // T
		{ center.x, 1, cx, center.y },  // L
	};

	RECT* p = _GridLines;
	RECT* end = p + sizeof(_GridLines) / sizeof(RECT);

	RECT rect = { 0, 0, cx, cy };

	HDC hWindowDC = getDC(NULL);
	HDC hdc = createCompatibleDC(hWindowDC);
	HBITMAP hBitmap = createCompatibleBitmap(hWindowDC, cx, cy);
	HBITMAP hOldBitmap = selectObject(hdc, hBitmap);
	releaseDC(NULL, hWindowDC);

	hImageList = imageList_Create(cx, cy, ILC_COLORDDB | ILC_MASK, 0, 1);
	imageList_SetBkColor(hImageList, CLR_NONE);

	for (; p < end; p++)
	{
		int i;

		axl_gr_FastRectangleEx(hdc, &rect, palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_Unused)]);

		for (i = p->top; i < p->bottom; i += 2)
			setPixel(hdc, center.x, i, palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_Grid)]);
		
		for (i = p->left; i < p->right; i += 2)
			setPixel(hdc, i, center.y, palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_Grid)]);

		selectObject(hdc, hOldBitmap);
		imageList_AddMasked(hImageList, hBitmap, palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_Unused)]);
		selectObject(hdc, hBitmap);
	}

	selectObject(hdc, hOldBitmap);
	deleteDC(hdc);
	deleteObject(hBitmap);

	return hImageList;
}

static
HBITMAP 
AXL_API
createColorBitmap(
	ulong_t cx,
	ulong_t cy,
	COLORREF color
	)
{
	HDC hdcDesktop = getDC(NULL);
	HDC hdc = createCompatibleDC(hdcDesktop);
	HBITMAP hBitmap = createCompatibleBitmap(hdcDesktop, cx, cy);
	HBITMAP hOldBitmap = selectObject(hdc, hBitmap);
	RECT rect = { 0, 0, cx, cy };

	axl_gr_FastRectangleEx(hdc, &rect, color);

	selectObject(hdc, hOldBitmap);
	deleteDC(hdc);
	releaseDC(NULL, hdcDesktop);
	return hBitmap;
}


HIMAGELIST 
AXL_API
axl_win_TPropertyCtrl_CreateMenuArrowImageList()
{
	COLORREF colors[] = 
	{
		getSysColor(COLOR_WINDOWTEXT),
		getSysColor(COLOR_HIGHLIGHTTEXT),
		getSysColor(COLOR_GRAYTEXT),
	};

	size_t i;

	static WORD _Mask[8] = { 0xff, 0xef, 0xe7, 0xe3, 0xe1, 0xe3, 0xe7, 0xef };
	HBITMAP hMaskBitmap = createBitmap(8, 8, 1, 1, _Mask);

	HIMAGELIST hImageList = imageList_Create(8, 8, ILC_COLORDDB | ILC_MASK, 0, 1);
	imageList_SetBkColor(hImageList, CLR_NONE);

	for (i = 0; i < sizeof(colors) / sizeof(COLORREF); i++)
	{
		HBITMAP hBitmap = createColorBitmap(8, 8, colors[i]);
		imageList_Add(hImageList, hBitmap, hMaskBitmap);
		deleteObject(hBitmap);
	}

	deleteObject(hMaskBitmap);
	return hImageList;
}

//.............................................................................

void
AXL_API
axl_win_TPropertyCtrl_PaintGridLines(
	axl_win_TPropertyCtrl* propertyCtrl,
	const RECT* rect,
	axl_win_TPropertyLine* line
	)
{ 
	axl_gr_TTextPaint* paint = &propertyCtrl->m_textPaint;

	int i;
	long x = axl_win_TPropertyCtrl_IsMenu(propertyCtrl) ? 
		(line->m_indent) * propertyCtrl->m_indentWidth - propertyCtrl->m_firstVisibleX :
		(line->m_level + line->m_indent) * propertyCtrl->m_indentWidth - propertyCtrl->m_firstVisibleX;

	axl_win_TPropertyLine* parent = line->m_parent;
 
	if (axl_win_TPropertyLine_HasGridLines(line))
		for (i = 0; i < line->m_indent; i++, x -= propertyCtrl->m_indentWidth)
		{
			imageList_Draw(
				propertyCtrl->m_hGridImageList, 
				axl_win_EPropertyGridLineImage_H,			
				paint->m_hdc, 
				x, rect->top, 
				ILD_NORMAL
				);
		}

	if (axl_win_TPropertyLine_HasGridLines(line))
	{
		axl_win_TPropertyLine* next = axl_win_TPropertyLine_GetNextSibling(line);
		int image = next && !(next->m_flags & axl_win_EPropertyLine_ForceNoGridLines) ? 
			axl_win_EPropertyGridLineImage_T : 
			axl_win_EPropertyGridLineImage_L 
			;

		imageList_Draw(
			propertyCtrl->m_hGridImageList, 
			image,
			paint->m_hdc, 
			x, rect->top, 
			ILD_NORMAL
			);
	}

	if (axl_win_TPropertyCtrl_IsMenu(propertyCtrl))
		return;

	line = line->m_parent;

	while (line)
	{
		x -= (line->m_indent + 1) * propertyCtrl->m_indentWidth;

		if (axl_win_TPropertyLine_HasGridLines(line) && !axl_win_TPropertyLine_IsLastChild(line))
			imageList_Draw(
				propertyCtrl->m_hGridImageList, 
				axl_win_EPropertyGridLineImage_V,
				paint->m_hdc, 
				x, rect->top, 
				ILD_NORMAL
				);

		line = line->m_parent;
	}
}

void
AXL_API
axl_win_TPropertyCtrl_PaintOptionBox(
	axl_win_TPropertyCtrl* propertyCtrl,
	const RECT* rect,
	axl_win_TPropertyLine* line
	)
{
	axl_gr_TTextPaint* paint = &propertyCtrl->m_textPaint;

	RECT rect;

	long x = axl_win_TPropertyCtrl_IsMenu(propertyCtrl) ? 0 : line->m_level * propertyCtrl->m_indentWidth;
	x += (line->m_indent + 1) * propertyCtrl->m_indentWidth - propertyCtrl->m_firstVisibleX;

	rect.left = propertyCtrl->m_optionBoxRect.left + x;
	rect.top = propertyCtrl->m_optionBoxRect.top + rect->top;
	rect.right = propertyCtrl->m_optionBoxRect.right + x;
	rect.bottom = propertyCtrl->m_optionBoxRect.bottom + rect->top;

	switch (line->m_optionBoxType)
	{
	case axl_win_EPropertyOptionBoxType_RadioButton:
		axl_gr_TStockCtrlPaint_DrawRadioButton(
			&propertyCtrl->m_stockCtrlPaint, 
			paint->m_hdc, 
			&rect, 
			line->m_optionBoxCheckState, 
			(line->m_flags & axl_win_EPropertyLine_IsDisabled) ? axl_win_EPropertyButtonVolatileState_Disabled : line->m_optionBoxVolatileState
			);

		break;

	case axl_win_EPropertyOptionBoxType_CheckBox:
		axl_gr_TStockCtrlPaint_DrawCheckBox(
			&propertyCtrl->m_stockCtrlPaint, 
			paint->m_hdc, 
			&rect, 
			line->m_optionBoxCheckState, 
			(line->m_flags & axl_win_EPropertyLine_IsDisabled) ? axl_win_EPropertyButtonVolatileState_Disabled : line->m_optionBoxVolatileState
			);

		break;
	}
}

void
AXL_API
axl_win_TPropertyCtrl_PaintLineImages(
	axl_win_TPropertyCtrl* propertyCtrl, 
	const RECT* rect,
	axl_win_TPropertyLine* line,
	bool_t isSelected
	)
{
	axl_gr_TTextPaint* paint = &propertyCtrl->m_textPaint;

	long x = axl_win_TPropertyCtrl_IsMenu(propertyCtrl) ? 
		-(long) propertyCtrl->m_firstVisibleX :
		line->m_level * propertyCtrl->m_indentWidth - propertyCtrl->m_firstVisibleX;

	if (axl_win_TPropertyLine_HasPlusMinus(line))
	{
		int image = (line->m_flags & axl_win_EPropertyLine_IsExpanded) ? 
			axl_win_EPropertyPlusMinusImage_Minus : 
			axl_win_EPropertyPlusMinusImage_Plus;

		imageList_Draw(
			propertyCtrl->m_hPlusMinusImageList, 
			image,			
			paint->m_hdc, 
			x + propertyCtrl->m_plusMinusRect.left, 
			rect->top + propertyCtrl->m_plusMinusRect.top, 
			ILD_NORMAL
			); 
	}

	x += (line->m_indent + 1) * propertyCtrl->m_indentWidth;

	if (line->m_optionBoxType)
		x += propertyCtrl->m_optionBoxWidth;

	if (line->m_icon != -1)
	{
		long xIcon = x + propertyCtrl->m_iconRect.left;
		long yIcon = rect->top + propertyCtrl->m_iconRect.top;

		if (axl_win_TPropertyLine_IsButton(line) && line->m_buttonVolatileState == axl_win_EPropertyButtonVolatileState_Pushed)
			xIcon++, yIcon++;

		imageList_Draw(
			propertyCtrl->m_hImageList, 
			line->m_icon,
			paint->m_hdc, 
			xIcon, 
			yIcon, 
			ILD_NORMAL
			); 
	}

	if ((line->m_flags & axl_win_EPropertyLine_Menu) && !axl_rtl_TList_IsEmpty(&line->m_childrenList))
	{
		long xArrow = line->m_menuWidth + propertyCtrl->m_margins.left + propertyCtrl->m_margins.right;
		long yArrow = (rect->top + rect->bottom - 8) / 2;	

		if (axl_win_TPropertyLine_IsButton(line) && line->m_buttonVolatileState == axl_win_EPropertyButtonVolatileState_Pushed)
			xArrow++, yArrow++;

		imageList_Draw(
			propertyCtrl->m_hMenuArrowImageList, 
			(line->m_flags & axl_win_EPropertyLine_IsDisabled) ? 2 : isSelected ? 1 : 0,
			paint->m_hdc, 
			xArrow,
			yArrow,
			ILD_NORMAL
			);
	}
}

void 
AXL_API
axl_win_TPropertyCtrl_PaintLeftPane(
	axl_win_TPropertyCtrl* propertyCtrl,
	const RECT* rect,
	axl_win_TPropertyLine* line,
	bool_t isSelected
	)
{
	axl_gr_TTextPaint* paint = &propertyCtrl->m_textPaint;
	axl_rtl_TString* string;

	bool_t shouldDrawButton;
	RECT textRect = *rect;

	long x = axl_win_TPropertyLine_GetPartX(line, axl_win_EPropertyLinePart_Name) - propertyCtrl->m_firstVisibleX + propertyCtrl->m_textOrigin.x;
	long y = rect->top + propertyCtrl->m_textOrigin.y;

	if (axl_win_TPropertyLine_HasNameDelimiter(line))
	{
		RECT delimRect = *rect;

		textRect.bottom--;
		delimRect.top = textRect.bottom;

		axl_gr_TTextPaint_SetBackColor(paint, axl_win_EPropertyColor_NameDelimiter);
		axl_gr_FastRectangle(paint->m_hdc, &delimRect);
	}

	if (axl_win_TPropertyLine_IsButton(line) && line->m_buttonVolatileState == axl_win_EPropertyButtonVolatileState_Pushed)
		x++, y++;

	paint->m_stdAttr.m_textColor = axl_win_EPropertyColor_Name;
	paint->m_stdAttr.m_backColor = axl_win_EPropertyColor_NameBack;
	paint->m_stdAttr.m_fontFlags = 0;

	axl_gr_TTextAttr_Overlay(&paint->m_stdAttr, &line->m_nameAttr);

	if (isSelected)
	{
		paint->m_stdAttr.m_textColor = axl_win_EPropertyColor_SelectedName;
		paint->m_stdAttr.m_backColor = axl_win_EPropertyColor_SelectedBack;
	}

	if (line->m_flags & axl_win_EPropertyLine_IsDisabled)
		paint->m_stdAttr.m_textColor = axl_win_EPropertyColor_DisabledText;
	else if (line->m_buttonVolatileState == axl_win_EPropertyButtonVolatileState_Hover)
		paint->m_stdAttr.m_textColor = axl_win_EPropertyColor_HoverButtonText;
	else if (line->m_buttonVolatileState)
		paint->m_stdAttr.m_textColor = axl_win_EPropertyColor_ButtonText;

	axl_gr_TTextPaint_SetStdAttr(paint);

	axl_gr_FastRectangle(paint->m_hdc, &textRect);

	shouldDrawButton = 
		axl_win_TPropertyLine_IsButton(line) && 
		(propertyCtrl->m_doAlwaysDrawButtons || 
		(line->m_flags & axl_win_EPropertyLine_AlwaysDrawButton) ||
		isSelected || 
		line->m_buttonVolatileState || 
		line->m_buttonCheckState);

	if (shouldDrawButton)
	{
		if (line->m_flags & axl_win_EPropertyLine_Button)
		{
			RECT rect;
			rect.left = axl_win_TPropertyLine_GetPartX(line, axl_win_EPropertyLinePart_Button);
			rect.right = line->m_singlePaneWidth + propertyCtrl->m_margins.left + 2 * propertyCtrl->m_margins.right;
			rect.top = rect->top;
			rect.bottom = rect->bottom - 1;

			rect.left -= propertyCtrl->m_firstVisibleX;
			rect.right -= propertyCtrl->m_firstVisibleX;

			axl_gr_TStockCtrlPaint_DrawPushButton(
				&propertyCtrl->m_stockCtrlPaint, 
				paint->m_hdc, 
				&rect, 
				line->m_buttonCheckState,
				(line->m_flags & axl_win_EPropertyLine_IsDisabled) ? axl_win_EPropertyButtonVolatileState_Disabled : line->m_buttonVolatileState
				);
		}
		else if (line->m_flags & axl_win_EPropertyLine_Menu)
		{
			RECT rect;
			rect.left = axl_win_TPropertyLine_GetPartX(line, axl_win_EPropertyLinePart_Button);
			rect.right = line->m_menuWidth + propertyCtrl->m_margins.left + propertyCtrl->m_margins.right + getSystemMetrics(SM_CXVSCROLL);
			rect.top = rect->top;
			rect.bottom = rect->bottom - 1;

			rect.left -= propertyCtrl->m_firstVisibleX;
			rect.right -= propertyCtrl->m_firstVisibleX;

			axl_gr_TStockCtrlPaint_DrawToolbarButton(
				&propertyCtrl->m_stockCtrlPaint, 
				paint->m_hdc, 
				&rect, 
				line->m_buttonCheckState,
				(line->m_flags & axl_win_EPropertyLine_IsDisabled) ? axl_win_EPropertyButtonVolatileState_Disabled : line->m_buttonVolatileState
				);
		}
	}

	string = (line->m_flags & axl_win_EPropertyLine_NameHyperText) ? &line->m_nameHyperText.m_text : &line->m_name;

	setBkMode(paint->m_hdc, TRANSPARENT);
	paint->m_etoFlags = 0;
	paint->m_top = textRect.top;
	paint->m_bottom = textRect.bottom;

	if ((line->m_flags & axl_win_EPropertyLine_NameShadow) && 
		!(line->m_flags & axl_win_EPropertyLine_IsDisabled) &&
		!shouldDrawButton)
	{
		COLORREF color = getBkColor(paint->m_hdc);
		color = RGB(getRValue(color) * 4/ 5, getGValue(color) * 4 / 5, getBValue(color) * 4 / 5); // calc shadow color
		setTextColor(paint->m_hdc, color);

		paint->m_x = x + 2;
		paint->m_y = y + 2;

		axl_gr_TTextPaint_PaintText(paint, string->m_p, axl_rtl_TString_GetLength(string));
		axl_gr_TTextPaint_SetStdTextColor(paint);
	}

	paint->m_x = x;
	paint->m_y = y;

	if (line->m_flags & axl_win_EPropertyLine_NameHyperText)
		axl_gr_TTextPaint_PaintHyperText(
			paint, 
			string->m_p, 
			axl_rtl_TString_GetLength(string),
			&line->m_nameHyperText.m_attrArray
			);
	else
		axl_gr_TTextPaint_PaintText(paint, string->m_p, axl_rtl_TString_GetLength(string));

	// option box must be draw first to minimize flickering (BitBlt flushes DirectDraw buffer)

	if (line->m_optionBoxType)
		axl_win_TPropertyCtrl_PaintOptionBox(propertyCtrl, &textRect, line);

	axl_win_TPropertyCtrl_PaintGridLines(propertyCtrl, rect, line);
	axl_win_TPropertyCtrl_PaintLineImages(propertyCtrl, &textRect, line, isSelected);

	if (axl_win_TPropertyLine_IsSinglePane(line) && axl_win_TPropertyLine_HasValueDelimiter(line))
	{
		RECT delimRect = *rect;

		delimRect.top = rect->bottom - 1;
		delimRect.left = propertyCtrl->m_splitterPos - propertyCtrl->m_firstVisibleX;

		axl_gr_TTextPaint_SetBackColor(paint, axl_win_EPropertyColor_ValueDelimiter);
		axl_gr_FastRectangle(paint->m_hdc, &delimRect);
	}
}

void 
AXL_API
axl_win_TPropertyCtrl_PaintRightPane(
	axl_win_TPropertyCtrl* propertyCtrl,
	const RECT* rect,
	axl_win_TPropertyLine* line,
	bool_t isSelected
	)
{
	axl_gr_TTextPaint* paint = &propertyCtrl->m_textPaint;
	axl_rtl_TString* string;

	RECT textRect = *rect;

	long x = rect->left + propertyCtrl->m_textOrigin.x;
	long y = rect->top + propertyCtrl->m_textOrigin.y;

	if (axl_win_TPropertyLine_HasValueDelimiter(line))
	{
		RECT delimRect = *rect;

		textRect.bottom--;
		delimRect.top = textRect.bottom;

		axl_gr_TTextPaint_SetBackColor(paint, axl_win_EPropertyColor_ValueDelimiter);
		axl_gr_FastRectangle(paint->m_hdc, &delimRect);
	}

	paint->m_stdAttr.m_textColor = axl_win_EPropertyColor_Value;
	paint->m_stdAttr.m_backColor = axl_win_EPropertyColor_ValueBack;
	paint->m_stdAttr.m_fontFlags = 0;

	axl_gr_TTextAttr_Overlay(&paint->m_stdAttr, &line->m_valueAttr);

	if (isSelected)
	{
		paint->m_stdAttr.m_textColor = axl_win_EPropertyColor_SelectedValue;
		paint->m_stdAttr.m_backColor = axl_win_EPropertyColor_SelectedBack;
	}

	if (line->m_flags & axl_win_EPropertyLine_IsDisabled)
		paint->m_stdAttr.m_textColor = axl_win_EPropertyColor_DisabledText;

	axl_gr_TTextPaint_SetStdAttr(paint);

	axl_gr_FastRectangle(paint->m_hdc, &textRect);

	string = (line->m_flags & axl_win_EPropertyLine_ValueHyperText) ? &line->m_valueHyperText.m_text : &line->m_value;

	setBkMode(paint->m_hdc, TRANSPARENT);
	paint->m_etoFlags = 0;
	paint->m_x = x;
	paint->m_y = y;
	paint->m_top = textRect.top;
	paint->m_bottom = textRect.bottom;

	if (line->m_flags & axl_win_EPropertyLine_ValueHyperText)
		axl_gr_TTextPaint_PaintHyperText(
			paint, 
			string->m_p, 
			axl_rtl_TString_GetLength(string),
			&line->m_valueHyperText.m_attrArray
			);
	else
		axl_gr_TTextPaint_PaintText(paint, string->m_p, axl_rtl_TString_GetLength(string));
}

void 
AXL_API
axl_win_TPropertyCtrl_PaintLine(
	axl_win_TPropertyCtrl* propertyCtrl,
	const RECT* rect,
	axl_win_TPropertyLine* line,
	bool_t isSelected
	)
{
	axl_gr_TTextPaint* paint = &propertyCtrl->m_textPaint;

	RECT nameRect;
	RECT valueRect;
	RECT splitterRect;

#ifndef _AXL_USE_OFFSCREEN_BUFFER
	HRGN hRgnClip;
	HRGN hOldRgnClip;
	int oldClipRgn;
	POINT viewportOrg;
#endif

	if (axl_win_TPropertyLine_IsSinglePane(line))
	{
		axl_win_TPropertyCtrl_PaintLeftPane(propertyCtrl, rect, line, isSelected);
		return;
	}

	nameRect = valueRect = splitterRect = *rect;
	nameRect.right = propertyCtrl->m_splitterPos - propertyCtrl->m_firstVisibleX;
	splitterRect.left = nameRect.right;
	splitterRect.right = splitterRect.left + 1;
	valueRect.left = splitterRect.right;

#ifndef _AXL_USE_OFFSCREEN_BUFFER
	hOldRgnClip = createRectRgn(0,0,0,0);
	oldClipRgn = getClipRgn(paint->m_hdc, hOldRgnClip);

	// clip rgn is NOT affected by viewport org, so we have to adjust it accordingly
	
	getViewportOrgEx(paint->m_hdc, &viewportOrg);
	hRgnClip = createRectRgn(
		viewportOrg.x, 
		viewportOrg.y + rect->top, 
		viewportOrg.x + nameRect.right, 
		viewportOrg.y + rect->bottom
		);

	extSelectClipRgn(paint->m_hdc, hRgnClip, RGN_AND);
#endif
	
	axl_win_TPropertyCtrl_PaintLeftPane(propertyCtrl, &nameRect, line, isSelected);

#ifndef _AXL_USE_OFFSCREEN_BUFFER
	selectClipRgn(paint->m_hdc, oldClipRgn > 0 ? hOldRgnClip : NULL);	
	deleteObject(hRgnClip);

	if (hOldRgnClip)
		deleteObject(hOldRgnClip);
#endif

	axl_gr_TTextPaint_SetBackColor(paint, axl_win_EPropertyColor_Splitter);
	axl_gr_FastRectangle(paint->m_hdc, &splitterRect);

	axl_win_TPropertyCtrl_PaintRightPane(propertyCtrl, &valueRect, line, isSelected);
}

void 
AXL_API
axl_win_TPropertyCtrl_PaintRect(
	axl_win_TPropertyCtrl* propertyCtrl,
	HDC hdc,
	const RECT* rect
	)
{
	axl_gr_TTextPaint* paint = &propertyCtrl->m_textPaint;

	RECT lineRect;

#ifdef _AXL_USE_OFFSCREEN_BUFFER
	RECT offscreenRect;
	HDC hdcOffscreen;
#endif

	long firstLine = rect->top / propertyCtrl->m_lineHeight;
	long lastLine = rect->bottom / propertyCtrl->m_lineHeight;
	size_t lineCount = axl_rtl_TPtrArray_GetCount(&propertyCtrl->m_visibleLineArray);

	if (rect->bottom % propertyCtrl->m_lineHeight)
		lastLine++;

	ASSERT(rect->left >= 0 && rect->top >= 0 && rect->left <= rect->right && rect->top <= rect->bottom);

	lineRect.left = rect->left;
	lineRect.top = firstLine * propertyCtrl->m_lineHeight;
	lineRect.right = rect->right;
	lineRect.bottom = lineRect.top + propertyCtrl->m_lineHeight;

#ifdef _AXL_USE_OFFSCREEN_BUFFER
	offscreenRect.left = rect->left;
	offscreenRect.top = 0;
	offscreenRect.right = rect->right;
	offscreenRect.bottom = propertyCtrl->m_lineHeight;

	hdcOffscreen = axl_gr_TOffscreenBuffer_GetBuffer(&propertyCtrl->m_offscreenBuffer, hdc, rect->right - rect->left, propertyCtrl->m_lineHeight);
	setViewportOrgEx(hdcOffscreen, -rect->left, 0, NULL);

	axl_gr_TTextPaint_Attach(&propertyCtrl->m_textPaint, hdcOffscreen);
#else
	axl_gr_TTextPaint_Attach(&propertyCtrl->m_textPaint, hdc);
#endif

	firstLine += (long) propertyCtrl->m_firstVisibleLine;
	lastLine += (long) propertyCtrl->m_firstVisibleLine;

	if (lastLine > (long) lineCount)
		lastLine = (long) lineCount;

	if (firstLine < lastLine)
	{
		axl_win_TPropertyLine** line = (axl_win_TPropertyLine**) propertyCtrl->m_visibleLineArray.m_p + firstLine;
		axl_win_TPropertyLine** end = line + lastLine - firstLine;
		axl_win_TPropertyLine* selectedLine = axl_win_TPropertyCtrl_GetSelectedLine(propertyCtrl);

		while (line < end)
		{	
			axl_win_TPropertyLine* line = *line;

#ifdef _AXL_USE_OFFSCREEN_BUFFER
			axl_win_TPropertyCtrl_PaintLine(propertyCtrl, &offscreenRect, line, line == selectedLine);
			bitBlt(
				hdc, 
				lineRect.left, lineRect.top, 
				lineRect.right - lineRect.left,
				lineRect.bottom - lineRect.top,
				hdcOffscreen,
				offscreenRect.left, offscreenRect.top,
				SRCCOPY
				);
#else
			axl_win_TPropertyCtrl_PaintLine(propertyCtrl, &lineRect, line, line == selectedLine);
#endif

			line++;

			lineRect.top += propertyCtrl->m_lineHeight;
			lineRect.bottom += propertyCtrl->m_lineHeight;
		}
	}

	if (rect->bottom > lineRect.top)
	{
		COLORREF color = propertyCtrl->m_palette[axl_gr_GetColorIndex(axl_win_EPropertyColor_Empty)];
		lineRect.bottom = rect->bottom;
		axl_gr_FastRectangleEx(hdc, &lineRect, color);
	}

	axl_gr_TTextPaint_Detach(&propertyCtrl->m_textPaint);
}

void 
AXL_API
axl_win_TPropertyCtrl_InvalidateLineRange(
	axl_win_TPropertyCtrl* propertyCtrl,
	size_t fromLine,
	size_t toLine
	)
{
	RECT rect;

	if (!isWindow(propertyCtrl->m_hWnd))
		return;

	getClientRect(propertyCtrl->m_hWnd, &rect);
	rect.top = (long) (fromLine - propertyCtrl->m_firstVisibleLine) * propertyCtrl->m_lineHeight;

	if (toLine != -1)
		rect.bottom = (long) (toLine - propertyCtrl->m_firstVisibleLine + 1) * propertyCtrl->m_lineHeight;

	invalidateRect(propertyCtrl->m_hWnd, &rect, false);
}

//.............................................................................
