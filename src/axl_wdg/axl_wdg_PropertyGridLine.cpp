#include "axl_Precomp.h"
#include "axl_win_PropertyCtrl.h"
#include "axl_base_Trace.h"

//.............................................................................

void 
AXL_API
axl_win_TPropertyLine_Construct(axl_win_TPropertyLine* line)
{
	line->m_flags = 0;
	line->m_delimiters = axl_win_EPropertyLineDelimiter_All;
	line->m_optionBoxType = axl_win_EPropertyOptionBoxType_None;
	line->m_optionBoxCheckState = axl_win_EPropertyCheckState_Unchecked;
	line->m_optionBoxVolatileState = axl_win_EPropertyButtonVolatileState_Normal;
	line->m_buttonVolatileState = axl_win_EPropertyButtonVolatileState_Normal;
	line->m_buttonCheckState = axl_win_EPropertyCheckState_Unchecked;
	line->m_icon = -1;
	line->m_toolTipIcon = 0;
	line->m_visibleIndex = -1;
	line->m_level = 0;
	line->m_indent = 0;
	line->m_childrenHeight = 0;
	line->m_menuWidth = 0;
	line->m_singlePaneWidth = 0;
	line->m_nameWidth = 0;
	line->m_valueWidth = 0;
	line->m_childrenSinglePaneWidth = 0;
	line->m_childrenValueWidth = 0;
	line->m_propertyCtrl = NULL;
	line->m_parent = NULL;
	line->m_inPlace = NULL;
	axl_obj_TData_Construct(&line->m_userData);
	axl_rtl_TList_Construct(&line->m_childrenList);
	axl_gr_TTextAttr_Construct(&line->m_nameAttr);
	axl_gr_TTextAttr_Construct(&line->m_valueAttr);
	axl_rtl_TString_Construct(&line->m_name);
	axl_rtl_TString_Construct(&line->m_value);
	axl_rtl_TString_Construct(&line->m_toolTipTitle);
	axl_rtl_TString_Construct(&line->m_toolTipText);
	axl_gr_THyperText_Construct(&line->m_nameHyperText);
	axl_gr_THyperText_Construct(&line->m_valueHyperText);
}

void 
AXL_API
axl_win_TPropertyLine_Destruct(axl_win_TPropertyLine* line)
{
	axl_win_TPropertyLine_ClearLineList(&line->m_childrenList);
	axl_rtl_TString_Destruct(&line->m_name);
	axl_rtl_TString_Destruct(&line->m_value);
	axl_rtl_TString_Destruct(&line->m_toolTipTitle);
	axl_rtl_TString_Destruct(&line->m_toolTipText);
	axl_gr_THyperText_Destruct(&line->m_nameHyperText);
	axl_gr_THyperText_Destruct(&line->m_valueHyperText);
	axl_obj_TData_Destruct(&line->m_userData);
	axl_obj_TObject_Dereference(line->m_inPlace);
}

void 
AXL_API
axl_win_TPropertyLine_ClearLineList(axl_rtl_TList* lineList)
{
	while (!axl_rtl_TList_IsEmpty(lineList))
	{
		axl_win_TPropertyLine* line = (axl_win_TPropertyLine*) axl_rtl_TList_RemoveHead(lineList);
		axl_win_TPropertyLine_Destruct(line);
		axl_mem_Free(line);
	}
}

axl_win_TPropertyLine* 
AXL_API
axl_win_TPropertyLine_GetNextSibling(axl_win_TPropertyLine* line)
{
	axl_rtl_TListEntry* p = line->m_parent ?
		axl_rtl_TList_GetNext(&line->m_parent->m_childrenList, (axl_rtl_TListEntry*) line) :
		axl_rtl_TList_GetNext(&line->m_propertyCtrl->m_lineList, (axl_rtl_TListEntry*) line);

	return (axl_win_TPropertyLine*) p;
}

axl_win_TPropertyLine* 
AXL_API
axl_win_TPropertyLine_GetPrevSibling(axl_win_TPropertyLine* line)
{
	axl_rtl_TListEntry* p = line->m_parent ?
		axl_rtl_TList_GetPrev(&line->m_parent->m_childrenList, (axl_rtl_TListEntry*) line) :
		axl_rtl_TList_GetPrev(&line->m_propertyCtrl->m_lineList, (axl_rtl_TListEntry*) line);

	return (axl_win_TPropertyLine*) p;
}

long 
AXL_API
axl_win_TPropertyLine_GetPartWidth(
	axl_win_TPropertyLine* line,
	int part
	)
{
	axl_win_TPropertyCtrl* propertyCtrl = line->m_propertyCtrl;

	switch (part)
	{
	case axl_win_EPropertyLinePart_Name:
		return line->m_nameWidth + propertyCtrl->m_margins.left + propertyCtrl->m_margins.right;

	case axl_win_EPropertyLinePart_Value:
		return line->m_valueWidth + propertyCtrl->m_margins.left + propertyCtrl->m_margins.right;

	case axl_win_EPropertyLinePart_Icon:
		return line->m_icon != -1 ? propertyCtrl->m_iconWidth : 0;

	case axl_win_EPropertyLinePart_OptionBox:
		return line->m_optionBoxType ? propertyCtrl->m_optionBoxWidth : 0;

	case axl_win_EPropertyLinePart_Button:
		if (line->m_flags & axl_win_EPropertyLine_Button)
			return axl_win_TPropertyLine_GetPartWidth(line, axl_win_EPropertyLinePart_Icon) + line->m_nameWidth + propertyCtrl->m_margins.left + 2 * propertyCtrl->m_margins.right;
		
		if (line->m_flags & axl_win_EPropertyLine_Menu)
			return line->m_menuWidth + propertyCtrl->m_margins.left + propertyCtrl->m_margins.right + getSystemMetrics(SM_CXVSCROLL) - axl_win_TPropertyLine_GetPartX(line, axl_win_EPropertyLinePart_Button);
	}

	return 0;
}

long 
AXL_API
axl_win_TPropertyLine_GetPartX(
	axl_win_TPropertyLine* line,
	int part
	)
{
	axl_win_TPropertyCtrl* propertyCtrl = line->m_propertyCtrl;

	long xBase = axl_win_TPropertyCtrl_IsMenu(propertyCtrl) ?
		(line->m_indent + 1) * propertyCtrl->m_indentWidth :
		(line->m_level + line->m_indent + 1) * propertyCtrl->m_indentWidth;

	switch (part)
	{
	case axl_win_EPropertyLinePart_Value:
		return propertyCtrl->m_splitterPos;

	case axl_win_EPropertyLinePart_Grid:
		return xBase;

	case axl_win_EPropertyLinePart_Icon:
	case axl_win_EPropertyLinePart_Button:
		return 
			xBase + 
			axl_win_TPropertyLine_GetPartWidth(line, axl_win_EPropertyLinePart_OptionBox);

	case axl_win_EPropertyLinePart_Name:
		return 
			xBase + 
			axl_win_TPropertyLine_GetPartWidth(line, axl_win_EPropertyLinePart_OptionBox) +
			axl_win_TPropertyLine_GetPartWidth(line, axl_win_EPropertyLinePart_Icon);
	}

	return xBase;
}

bool_t
AXL_API
axl_win_TPropertyLine_IsPartFullyVisible(
	axl_win_TPropertyLine* line,
	int part
	)
{
	long left; 
	long right;
	
	left = -line->m_propertyCtrl->m_firstVisibleX + axl_win_TPropertyLine_GetPartX(line, part);
	if (left < 0)
		return false;

	right = left + axl_win_TPropertyLine_GetPartWidth(line, part);

	if ((line->m_flags & axl_win_EPropertyLine_SinglePane) || part == axl_win_EPropertyLinePart_Value)
		return right < (long) line->m_propertyCtrl->m_visibleWidth;
	else
		return right < line->m_propertyCtrl->m_splitterPos;
}

static void AXL_API _AddInterval(long* left, long* right, long x, long width)
{
	if (x < *left)
		*left = x;

	if (width == MAXLONG)
		x = MAXLONG;
	else
		x += width;

	if (x > *right)
		*right = x;
}

void 
AXL_API
axl_win_TPropertyLine_Invalidate(
	axl_win_TPropertyLine* line,
	ulong_t mask
	)
{
	axl_win_TPropertyCtrl* propertyCtrl = line->m_propertyCtrl;

	RECT rect;

	long left = MAXLONG, right = 0;
	long nameBase = (line->m_level + line->m_indent + 1) * propertyCtrl->m_indentWidth;

	if (line->m_visibleIndex == -1)
		return;

	if (mask & axl_win_EPropertyLineInvalidate_Left)
		_AddInterval(
			&left, &right, 
			0, propertyCtrl->m_splitterPos
			);

	if (mask & axl_win_EPropertyLineInvalidate_Right)
		_AddInterval(
			&left, &right, 
			propertyCtrl->m_splitterPos, MAXLONG
			);

	if (mask & axl_win_EPropertyLineInvalidate_Name)
		if (axl_win_TPropertyLine_IsSinglePane(line))
			_AddInterval(
				&left, &right, 
				axl_win_TPropertyLine_GetPartX(line, axl_win_EPropertyLinePart_Name), 
				MAXLONG
				);
		else
			_AddInterval(
				&left, &right, 
				axl_win_TPropertyLine_GetPartX(line, axl_win_EPropertyLinePart_Name), 
				MAXLONG
				);

	if (mask & axl_win_EPropertyLineInvalidate_Value)
		_AddInterval(
			&left, &right, 
			propertyCtrl->m_splitterPos, MAXLONG
			);

	if (mask & axl_win_EPropertyLineInvalidate_OptionBox)
		_AddInterval(
			&left, &right, 
			axl_win_TPropertyLine_GetPartX(line, axl_win_EPropertyLinePart_OptionBox), 
			axl_win_TPropertyLine_GetPartWidth(line, axl_win_EPropertyLinePart_OptionBox)
			);

	if (mask & axl_win_EPropertyLineInvalidate_Icon)
		_AddInterval(
			&left, &right, 
			axl_win_TPropertyLine_GetPartX(line, axl_win_EPropertyLinePart_Icon), 
			axl_win_TPropertyLine_GetPartWidth(line, axl_win_EPropertyLinePart_Icon)
			);

	if (mask & axl_win_EPropertyLineInvalidate_Button)
		_AddInterval(
			&left, &right, 
			axl_win_TPropertyLine_GetPartX(line, axl_win_EPropertyLinePart_Button), 
			axl_win_TPropertyLine_GetPartWidth(line, axl_win_EPropertyLinePart_Button)
			);

	left -= propertyCtrl->m_firstVisibleX;
	right -= propertyCtrl->m_firstVisibleX;

	getClientRect(propertyCtrl->m_hWnd, &rect);
	rect.top = (long) (line->m_visibleIndex - propertyCtrl->m_firstVisibleLine) * propertyCtrl->m_lineHeight;
	rect.bottom = rect.top + propertyCtrl->m_lineHeight;
	rect.left = max(rect.left, left);
	rect.right = min(rect.right, right);
	invalidateRect(propertyCtrl->m_hWnd, &rect, false);
}

void 
AXL_API
axl_win_TPropertyLine_SetFlags(
	axl_win_TPropertyLine* line,
	int flags
	)
{
	if (line->m_flags == flags)
		return;

	line->m_flags = flags;
	axl_win_TPropertyCtrl_UpdateLineWidth(line->m_propertyCtrl, line);
	axl_win_TPropertyLine_Invalidate(line, axl_win_EPropertyLineInvalidate_All);
}

void 
AXL_API
axl_win_TPropertyLine_SetDelimiters(
	axl_win_TPropertyLine* line,
	int delimiters
	)
{
	if (line->m_delimiters == delimiters)
		return;

	line->m_delimiters = delimiters;
	axl_win_TPropertyLine_Invalidate(line, axl_win_EPropertyLineInvalidate_All);
}

void 
AXL_API
axl_win_TPropertyLine_ModifyFlags(
	axl_win_TPropertyLine* line,
	int modifier,
	bool_t set
	)
{
	axl_win_TPropertyLine_SetFlags(
		line, set ? 
		line->m_flags | modifier : 
		line->m_flags & ~modifier
		);
}

void 
AXL_API
axl_win_TPropertyLine_SetName(
	axl_win_TPropertyLine* line,
	const tchar_t* string
	)
{
	axl_rtl_TString_CopyString(&line->m_name, string, -1);
	
	if (line->m_flags & axl_win_EPropertyLine_NameHyperText)
	{
		axl_gr_THyperText_SetHyperText(&line->m_nameHyperText, string, -1, NULL);
		axl_gr_THyperText_CalcHyperlinkXMap(&line->m_nameHyperText, &line->m_propertyCtrl->m_textPaint.m_font);
	}

	if (axl_win_TPropertyLine_IsSinglePane(line))
		axl_win_TPropertyCtrl_UpdateLineWidth(line->m_propertyCtrl, line);
	
	axl_win_TPropertyLine_Invalidate(line, axl_win_EPropertyLineInvalidate_Name);
}

void 
AXL_API
axl_win_TPropertyLine_SetValue(
	axl_win_TPropertyLine* line,
	const tchar_t* string
	)
{
	axl_rtl_TString_CopyString(&line->m_value, string, -1);

	if (line->m_flags & axl_win_EPropertyLine_ValueHyperText)
	{
		axl_gr_THyperText_SetHyperText(&line->m_valueHyperText, string, -1, NULL);
		axl_gr_THyperText_CalcHyperlinkXMap(&line->m_valueHyperText, &line->m_propertyCtrl->m_textPaint.m_font);
	}

	if (!axl_win_TPropertyLine_IsSinglePane(line))
		axl_win_TPropertyCtrl_UpdateLineWidth(line->m_propertyCtrl, line);

	axl_win_TPropertyLine_Invalidate(line, axl_win_EPropertyLineInvalidate_Value);
}

void 
AXL_API
axl_win_TPropertyLine_SetOptionBoxType(
	axl_win_TPropertyLine* line,
	int type
	)
{
	if (line->m_optionBoxType == type)
		return;

	line->m_optionBoxType = type;

	if (axl_win_TPropertyLine_IsSinglePane(line))
		axl_win_TPropertyCtrl_UpdateLineWidth(line->m_propertyCtrl, line);

	axl_win_TPropertyLine_Invalidate(line, axl_win_EPropertyLineInvalidate_Left);
}

void 
AXL_API
axl_win_TPropertyLine_UncheckRadioButtonGroup(axl_win_TPropertyLine* line)
{
	axl_win_TPropertyLine* sibling;

	sibling = axl_win_TPropertyLine_GetPrevSibling(line);
	while (sibling && sibling->m_optionBoxType == axl_win_EPropertyOptionBoxType_RadioButton)
	{
		axl_win_TPropertyLine_SetOptionBoxCheckState(sibling, false);
		sibling = axl_win_TPropertyLine_GetPrevSibling(sibling);
	}

	sibling = axl_win_TPropertyLine_GetNextSibling(line);
	while (sibling && sibling->m_optionBoxType == axl_win_EPropertyOptionBoxType_RadioButton)
	{
		axl_win_TPropertyLine_SetOptionBoxCheckState(sibling, false);
		sibling = axl_win_TPropertyLine_GetNextSibling(sibling);
	}
}

void 
AXL_API
axl_win_TPropertyLine_SetOptionBoxCheckState(
	axl_win_TPropertyLine* line,
	int state
	)
{
	if (line->m_optionBoxCheckState == state)
		return;

	if (line->m_optionBoxType == axl_win_EPropertyOptionBoxType_RadioButton && state)
		axl_win_TPropertyLine_UncheckRadioButtonGroup(line);

	line->m_optionBoxCheckState = state;
	axl_win_TPropertyLine_Invalidate(line, axl_win_EPropertyLineInvalidate_OptionBox);
}

void 
AXL_API
axl_win_TPropertyLine_SetOptionBoxVolatileState(
	axl_win_TPropertyLine* line,
	int state
	)
{
	if (line->m_optionBoxVolatileState == state)
		return;

	line->m_optionBoxVolatileState = state;
	axl_win_TPropertyLine_Invalidate(line, axl_win_EPropertyLineInvalidate_OptionBox);
}

void 
AXL_API
axl_win_TPropertyLine_SetButtonCheckState(
	axl_win_TPropertyLine* line,
	int state
	)
{
	if (line->m_buttonCheckState == state)
		return;

	line->m_buttonCheckState = state;
	axl_win_TPropertyLine_Invalidate(line, axl_win_EPropertyLineInvalidate_Button);
}


void 
AXL_API
axl_win_TPropertyLine_SetButtonVolatileState(
	axl_win_TPropertyLine* line,
	int state
	)
{
	if (line->m_buttonVolatileState == state)
		return;

	line->m_buttonVolatileState = state;
	axl_win_TPropertyLine_Invalidate(line, axl_win_EPropertyLineInvalidate_Button);
}

void 
AXL_API
axl_win_TPropertyLine_SetNameAttr(
	axl_win_TPropertyLine* line,
	axl_gr_TTextAttr* attr
	)
{
	ulong_t oldFontFlags = line->m_nameAttr.m_fontFlags;

	line->m_nameAttr = attr ? *attr : g_axl_gr_TTextAttr_NullAttr;

	if (axl_win_TPropertyLine_IsSinglePane(line))
		axl_win_TPropertyCtrl_UpdateLineWidth(line->m_propertyCtrl, line);

	axl_win_TPropertyLine_Invalidate(line, axl_win_EPropertyLineInvalidate_Name);
}

void 
AXL_API
axl_win_TPropertyLine_SetValueAttr(
	axl_win_TPropertyLine* line,
	axl_gr_TTextAttr* attr
	)
{
	ulong_t oldFontFlags = line->m_nameAttr.m_fontFlags;

	line->m_valueAttr = attr ? *attr : g_axl_gr_TTextAttr_NullAttr;

	if (!axl_win_TPropertyLine_IsSinglePane(line))
		axl_win_TPropertyCtrl_UpdateLineWidth(line->m_propertyCtrl, line);

	axl_win_TPropertyLine_Invalidate(line, axl_win_EPropertyLineInvalidate_Value);
}

void 
AXL_API
axl_win_TPropertyLine_SetIcon(
	axl_win_TPropertyLine* line,
	int icon
	)
{
	int invalidate;

	if (line->m_icon == icon)
		return;

	if (icon == -1 && line->m_icon != -1 || icon != -1 && line->m_icon == -1)
		invalidate = axl_win_EPropertyLineInvalidate_Left;
	else
		invalidate = axl_win_EPropertyLineInvalidate_Icon;

	line->m_icon = icon;

	if (axl_win_TPropertyLine_IsSinglePane(line))
		axl_win_TPropertyCtrl_UpdateLineWidth(line->m_propertyCtrl, line);

	axl_win_TPropertyLine_Invalidate(line, invalidate);
}

void 
AXL_API
axl_win_TPropertyLine_SetIndent(
	axl_win_TPropertyLine* line,
	int indent
	)
{
	if (line->m_indent == indent)
		return;

	line->m_indent = indent;

	if (axl_win_TPropertyLine_IsSinglePane(line))
		axl_win_TPropertyCtrl_UpdateLineWidth(line->m_propertyCtrl, line);

	axl_win_TPropertyLine_Invalidate(line, axl_win_EPropertyLineInvalidate_Left);
}

void
AXL_API
axl_win_TPropertyLine_ModifyChildrenHeight(
	axl_win_TPropertyLine* line,
	long delta	
	)
{
	line->m_childrenHeight += delta;

	while ((line->m_flags & axl_win_EPropertyLine_IsExpanded) && line->m_parent)
	{
		line = line->m_parent;
		line->m_childrenHeight += delta;
	}
}

bool_t
AXL_API
axl_win_TPropertyLine_CalcWidth(
	axl_win_TPropertyLine* line,
	int flags
	)
{
	axl_win_TPropertyCtrl* propertyCtrl = line->m_propertyCtrl;
	bool_t hasChanged = false;

	if ((flags & axl_win_EPropertyCalcWidth_Menu) && (line->m_flags & axl_win_EPropertyLine_Menu))
	{
		ulong_t menuWidth = line->m_singlePaneWidth;

		axl_win_TPropertyLine* sibling;

		sibling = axl_win_TPropertyLine_GetPrevSibling(line);
		while (sibling && (sibling->m_flags & axl_win_EPropertyLine_Menu))
		{
			if (sibling->m_singlePaneWidth > menuWidth)
				menuWidth = sibling->m_singlePaneWidth;

			sibling = axl_win_TPropertyLine_GetPrevSibling(sibling);
		}

		sibling = axl_win_TPropertyLine_GetNextSibling(line);
		while (sibling && (sibling->m_flags & axl_win_EPropertyLine_Menu))
		{
			if (sibling->m_singlePaneWidth > menuWidth)
				menuWidth = sibling->m_singlePaneWidth;

			sibling = axl_win_TPropertyLine_GetNextSibling(sibling);
		}

		line->m_menuWidth = menuWidth;
	}

	if (flags & axl_win_EPropertyCalcWidth_Line)
	{
		HFONT hFont = axl_gr_TFontMod_GetFont(&propertyCtrl->m_textPaint.m_font, line->m_nameAttr.m_fontFlags);
		size_t length = axl_rtl_TString_GetLength(&line->m_name);
		SIZE size = axl_gr_TFont__CalcStringSize(hFont, line->m_name.m_p, length);

		line->m_nameWidth = size.cx;

		if (axl_win_TPropertyLine_IsSinglePane(line))
		{
            size.cx += axl_win_TPropertyLine_GetPartX(line, axl_win_EPropertyLinePart_Name);

			if (line->m_flags & axl_win_EPropertyLine_Menu)
				size.cx += propertyCtrl->m_indentWidth;

			if (line->m_singlePaneWidth != size.cx || line->m_valueWidth != 0)
			{
				line->m_singlePaneWidth = size.cx;
				line->m_valueWidth = 0;
				hasChanged = true;
			}
		}
		else
		{
			HFONT hFont = axl_gr_TFontMod_GetFont(&propertyCtrl->m_textPaint.m_font, line->m_valueAttr.m_fontFlags);
			size_t length = axl_rtl_TString_GetLength(&line->m_value);
			SIZE size = axl_gr_TFont__CalcStringSize(hFont, line->m_value.m_p, length);

			if (line->m_singlePaneWidth != 0 || line->m_valueWidth != size.cx)
			{
				line->m_singlePaneWidth = 0;
				line->m_valueWidth = size.cx;
				hasChanged = true;
			}
		}
	}

	if (flags & axl_win_EPropertyCalcWidth_Children)
	{
		axl_win_TPropertyLine* child;

		ulong_t maxSinglePaneWidth = 0;
		ulong_t maxValueWidth = 0;

		child = (axl_win_TPropertyLine*) axl_rtl_TList_GetHead(&line->m_childrenList);
		while (child)
		{
			ulong_t singlePaneWidth = 0;
			ulong_t valueWidth = 0;

			if (flags & axl_win_EPropertyCalcWidth_Recursive)
				axl_win_TPropertyLine_CalcWidth(child, flags);

			if (child->m_flags & axl_win_EPropertyLine_IsExpanded)
			{
				singlePaneWidth = max(child->m_childrenSinglePaneWidth, child->m_singlePaneWidth);
				valueWidth = max(child->m_childrenValueWidth, child->m_valueWidth);
			}
			else
			{
				singlePaneWidth = child->m_singlePaneWidth;
				valueWidth = child->m_valueWidth;
			}

			if (singlePaneWidth > maxSinglePaneWidth)
				maxSinglePaneWidth = singlePaneWidth;

			if (valueWidth > maxValueWidth)
				maxValueWidth = valueWidth;

			child = (axl_win_TPropertyLine*) axl_rtl_TList_GetNext(&line->m_childrenList, (axl_rtl_TListEntry*) child);
		}

		if (maxValueWidth != line->m_childrenValueWidth || maxSinglePaneWidth != line->m_childrenSinglePaneWidth)
			hasChanged = true;

		line->m_childrenValueWidth = maxValueWidth;
		line->m_childrenSinglePaneWidth = maxSinglePaneWidth;
	}

	if (flags & axl_win_EPropertyCalcWidth_Parent)
	{
		while (line->m_parent)
		{
			hasChanged = axl_win_TPropertyLine_CalcWidth(line->m_parent, axl_win_EPropertyCalcWidth_Children);
			if (!hasChanged)
				return false;

			line = line->m_parent;
		}

		hasChanged = true;
	}

	return hasChanged;
}

bool_t
AXL_API
axl_win_TPropertyLine_IsLastChild(axl_win_TPropertyLine* line)
{
	if (line->m_parent)
		return line == (axl_win_TPropertyLine*) axl_rtl_TList_GetTail(&line->m_parent->m_childrenList);
	else 
		return line == (axl_win_TPropertyLine*) axl_rtl_TList_GetTail(line->m_propertyCtrl->m_lineList);
}

bool_t
AXL_API
axl_win_TPropertyLine_HasGridLines(axl_win_TPropertyLine* line)
{
	if (line->m_flags & axl_win_EPropertyLine_ForceNoGridLines)
		return false;
	else if (line->m_parent)
		return (line->m_parent->m_flags & axl_win_EPropertyLine_NoChildGridLines) == 0;
	else
		return line->m_propertyCtrl->m_hasGridLinesAtRoot;
}

bool_t
AXL_API
axl_win_TPropertyLine_HasNameDelimiter(axl_win_TPropertyLine* line)
{
	axl_win_TPropertyCtrl* propertyCtrl = line->m_propertyCtrl;
	axl_win_TPropertyLine* nextLine;

	size_t lastVisibleLine = axl_rtl_TPtrArray_GetCount(&propertyCtrl->m_visibleLineArray) - 1;

	if (line->m_visibleIndex == lastVisibleLine) 
		return !axl_win_TPropertyCtrl_IsMenu(propertyCtrl);

	if (line->m_delimiters & axl_win_EPropertyLineDelimiter_NameBottom)
		return true;

	if (line->m_visibleIndex == -1 || line->m_visibleIndex > axl_rtl_TPtrArray_GetCount(&propertyCtrl->m_visibleLineArray))
		return false;

	nextLine = ((axl_win_TPropertyLine**) propertyCtrl->m_visibleLineArray.m_p)[line->m_visibleIndex + 1];
	return (nextLine->m_delimiters & axl_win_EPropertyLineDelimiter_NameTop) != 0;
}

bool_t
AXL_API
axl_win_TPropertyLine_HasValueDelimiter(axl_win_TPropertyLine* line)
{
	axl_win_TPropertyCtrl* propertyCtrl = line->m_propertyCtrl;
	axl_win_TPropertyLine* nextLine;

	size_t lastVisibleLine = axl_rtl_TPtrArray_GetCount(&line->m_propertyCtrl->m_visibleLineArray) - 1;

	if (line->m_visibleIndex == lastVisibleLine) 
		return !axl_win_TPropertyCtrl_IsMenu(propertyCtrl);

	if (line->m_delimiters & axl_win_EPropertyLineDelimiter_ValueBottom)
		return true;

	if (line->m_visibleIndex == -1 || line->m_visibleIndex > axl_rtl_TPtrArray_GetCount(&propertyCtrl->m_visibleLineArray))
		return false;

	nextLine = ((axl_win_TPropertyLine**) propertyCtrl->m_visibleLineArray.m_p)[line->m_visibleIndex + 1];
	return (nextLine->m_delimiters & axl_win_EPropertyLineDelimiter_ValueTop) != 0;
}

void 
AXL_API
axl_win_TPropertyLine_ExpandCollapse(
	axl_win_TPropertyLine* line,
	bool_t isExpanded
	)
{
	axl_win_TPropertyCtrl_ExpandCollapse(line->m_propertyCtrl, line, isExpanded);
}

//.............................................................................

