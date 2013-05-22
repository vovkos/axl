#include "axl_Precomp.h"
#include "axl_win_PropertyCtrl.h"
#include "axl_base_Trace.h"

//.............................................................................

void 
AXL_API
axl_win_TPropertyLine_Construct(axl_win_TPropertyLine* pLine)
{
	pLine->m_Flags = 0;
	pLine->m_Delimiters = axl_win_EPropertyLineDelimiter_All;
	pLine->m_OptionBoxType = axl_win_EPropertyOptionBoxType_None;
	pLine->m_OptionBoxCheckState = axl_win_EPropertyCheckState_Unchecked;
	pLine->m_OptionBoxVolatileState = axl_win_EPropertyButtonVolatileState_Normal;
	pLine->m_ButtonVolatileState = axl_win_EPropertyButtonVolatileState_Normal;
	pLine->m_ButtonCheckState = axl_win_EPropertyCheckState_Unchecked;
	pLine->m_Icon = -1;
	pLine->m_ToolTipIcon = 0;
	pLine->m_VisibleIndex = -1;
	pLine->m_Level = 0;
	pLine->m_Indent = 0;
	pLine->m_ChildrenHeight = 0;
	pLine->m_MenuWidth = 0;
	pLine->m_SinglePaneWidth = 0;
	pLine->m_NameWidth = 0;
	pLine->m_ValueWidth = 0;
	pLine->m_ChildrenSinglePaneWidth = 0;
	pLine->m_ChildrenValueWidth = 0;
	pLine->m_pPropertyCtrl = NULL;
	pLine->m_pParent = NULL;
	pLine->m_pInPlace = NULL;
	axl_obj_TData_Construct(&pLine->m_UserData);
	axl_rtl_TList_Construct(&pLine->m_ChildrenList);
	axl_gr_TTextAttr_Construct(&pLine->m_NameAttr);
	axl_gr_TTextAttr_Construct(&pLine->m_ValueAttr);
	axl_rtl_TString_Construct(&pLine->m_Name);
	axl_rtl_TString_Construct(&pLine->m_Value);
	axl_rtl_TString_Construct(&pLine->m_ToolTipTitle);
	axl_rtl_TString_Construct(&pLine->m_ToolTipText);
	axl_gr_THyperText_Construct(&pLine->m_NameHyperText);
	axl_gr_THyperText_Construct(&pLine->m_ValueHyperText);
}

void 
AXL_API
axl_win_TPropertyLine_Destruct(axl_win_TPropertyLine* pLine)
{
	axl_win_TPropertyLine_ClearLineList(&pLine->m_ChildrenList);
	axl_rtl_TString_Destruct(&pLine->m_Name);
	axl_rtl_TString_Destruct(&pLine->m_Value);
	axl_rtl_TString_Destruct(&pLine->m_ToolTipTitle);
	axl_rtl_TString_Destruct(&pLine->m_ToolTipText);
	axl_gr_THyperText_Destruct(&pLine->m_NameHyperText);
	axl_gr_THyperText_Destruct(&pLine->m_ValueHyperText);
	axl_obj_TData_Destruct(&pLine->m_UserData);
	axl_obj_TObject_Dereference(pLine->m_pInPlace);
}

void 
AXL_API
axl_win_TPropertyLine_ClearLineList(axl_rtl_TList* pLineList)
{
	while (!axl_rtl_TList_IsEmpty(pLineList))
	{
		axl_win_TPropertyLine* pLine = (axl_win_TPropertyLine*) axl_rtl_TList_RemoveHead(pLineList);
		axl_win_TPropertyLine_Destruct(pLine);
		axl_mem_Free(pLine);
	}
}

axl_win_TPropertyLine* 
AXL_API
axl_win_TPropertyLine_GetNextSibling(axl_win_TPropertyLine* pLine)
{
	axl_rtl_TListEntry* p = pLine->m_pParent ?
		axl_rtl_TList_GetNext(&pLine->m_pParent->m_ChildrenList, (axl_rtl_TListEntry*) pLine) :
		axl_rtl_TList_GetNext(&pLine->m_pPropertyCtrl->m_LineList, (axl_rtl_TListEntry*) pLine);

	return (axl_win_TPropertyLine*) p;
}

axl_win_TPropertyLine* 
AXL_API
axl_win_TPropertyLine_GetPrevSibling(axl_win_TPropertyLine* pLine)
{
	axl_rtl_TListEntry* p = pLine->m_pParent ?
		axl_rtl_TList_GetPrev(&pLine->m_pParent->m_ChildrenList, (axl_rtl_TListEntry*) pLine) :
		axl_rtl_TList_GetPrev(&pLine->m_pPropertyCtrl->m_LineList, (axl_rtl_TListEntry*) pLine);

	return (axl_win_TPropertyLine*) p;
}

long 
AXL_API
axl_win_TPropertyLine_GetPartWidth(
	axl_win_TPropertyLine* pLine,
	int Part
	)
{
	axl_win_TPropertyCtrl* pPropertyCtrl = pLine->m_pPropertyCtrl;

	switch (Part)
	{
	case axl_win_EPropertyLinePart_Name:
		return pLine->m_NameWidth + pPropertyCtrl->m_Margins.left + pPropertyCtrl->m_Margins.right;

	case axl_win_EPropertyLinePart_Value:
		return pLine->m_ValueWidth + pPropertyCtrl->m_Margins.left + pPropertyCtrl->m_Margins.right;

	case axl_win_EPropertyLinePart_Icon:
		return pLine->m_Icon != -1 ? pPropertyCtrl->m_IconWidth : 0;

	case axl_win_EPropertyLinePart_OptionBox:
		return pLine->m_OptionBoxType ? pPropertyCtrl->m_OptionBoxWidth : 0;

	case axl_win_EPropertyLinePart_Button:
		if (pLine->m_Flags & axl_win_EPropertyLine_Button)
			return axl_win_TPropertyLine_GetPartWidth(pLine, axl_win_EPropertyLinePart_Icon) + pLine->m_NameWidth + pPropertyCtrl->m_Margins.left + 2 * pPropertyCtrl->m_Margins.right;
		
		if (pLine->m_Flags & axl_win_EPropertyLine_Menu)
			return pLine->m_MenuWidth + pPropertyCtrl->m_Margins.left + pPropertyCtrl->m_Margins.right + GetSystemMetrics(SM_CXVSCROLL) - axl_win_TPropertyLine_GetPartX(pLine, axl_win_EPropertyLinePart_Button);
	}

	return 0;
}

long 
AXL_API
axl_win_TPropertyLine_GetPartX(
	axl_win_TPropertyLine* pLine,
	int Part
	)
{
	axl_win_TPropertyCtrl* pPropertyCtrl = pLine->m_pPropertyCtrl;

	long xBase = axl_win_TPropertyCtrl_IsMenu(pPropertyCtrl) ?
		(pLine->m_Indent + 1) * pPropertyCtrl->m_IndentWidth :
		(pLine->m_Level + pLine->m_Indent + 1) * pPropertyCtrl->m_IndentWidth;

	switch (Part)
	{
	case axl_win_EPropertyLinePart_Value:
		return pPropertyCtrl->m_SplitterPos;

	case axl_win_EPropertyLinePart_Grid:
		return xBase;

	case axl_win_EPropertyLinePart_Icon:
	case axl_win_EPropertyLinePart_Button:
		return 
			xBase + 
			axl_win_TPropertyLine_GetPartWidth(pLine, axl_win_EPropertyLinePart_OptionBox);

	case axl_win_EPropertyLinePart_Name:
		return 
			xBase + 
			axl_win_TPropertyLine_GetPartWidth(pLine, axl_win_EPropertyLinePart_OptionBox) +
			axl_win_TPropertyLine_GetPartWidth(pLine, axl_win_EPropertyLinePart_Icon);
	}

	return xBase;
}

bool_t
AXL_API
axl_win_TPropertyLine_IsPartFullyVisible(
	axl_win_TPropertyLine* pLine,
	int Part
	)
{
	long left; 
	long right;
	
	left = -pLine->m_pPropertyCtrl->m_FirstVisibleX + axl_win_TPropertyLine_GetPartX(pLine, Part);
	if (left < 0)
		return false;

	right = left + axl_win_TPropertyLine_GetPartWidth(pLine, Part);

	if ((pLine->m_Flags & axl_win_EPropertyLine_SinglePane) || Part == axl_win_EPropertyLinePart_Value)
		return right < (long) pLine->m_pPropertyCtrl->m_VisibleWidth;
	else
		return right < pLine->m_pPropertyCtrl->m_SplitterPos;
}

static void AXL_API _AddInterval(long* pLeft, long* pRight, long x, long Width)
{
	if (x < *pLeft)
		*pLeft = x;

	if (Width == MAXLONG)
		x = MAXLONG;
	else
		x += Width;

	if (x > *pRight)
		*pRight = x;
}

void 
AXL_API
axl_win_TPropertyLine_Invalidate(
	axl_win_TPropertyLine* pLine,
	ulong_t Mask
	)
{
	axl_win_TPropertyCtrl* pPropertyCtrl = pLine->m_pPropertyCtrl;

	RECT Rect;

	long Left = MAXLONG, Right = 0;
	long NameBase = (pLine->m_Level + pLine->m_Indent + 1) * pPropertyCtrl->m_IndentWidth;

	if (pLine->m_VisibleIndex == -1)
		return;

	if (Mask & axl_win_EPropertyLineInvalidate_Left)
		_AddInterval(
			&Left, &Right, 
			0, pPropertyCtrl->m_SplitterPos
			);

	if (Mask & axl_win_EPropertyLineInvalidate_Right)
		_AddInterval(
			&Left, &Right, 
			pPropertyCtrl->m_SplitterPos, MAXLONG
			);

	if (Mask & axl_win_EPropertyLineInvalidate_Name)
		if (axl_win_TPropertyLine_IsSinglePane(pLine))
			_AddInterval(
				&Left, &Right, 
				axl_win_TPropertyLine_GetPartX(pLine, axl_win_EPropertyLinePart_Name), 
				MAXLONG
				);
		else
			_AddInterval(
				&Left, &Right, 
				axl_win_TPropertyLine_GetPartX(pLine, axl_win_EPropertyLinePart_Name), 
				MAXLONG
				);

	if (Mask & axl_win_EPropertyLineInvalidate_Value)
		_AddInterval(
			&Left, &Right, 
			pPropertyCtrl->m_SplitterPos, MAXLONG
			);

	if (Mask & axl_win_EPropertyLineInvalidate_OptionBox)
		_AddInterval(
			&Left, &Right, 
			axl_win_TPropertyLine_GetPartX(pLine, axl_win_EPropertyLinePart_OptionBox), 
			axl_win_TPropertyLine_GetPartWidth(pLine, axl_win_EPropertyLinePart_OptionBox)
			);

	if (Mask & axl_win_EPropertyLineInvalidate_Icon)
		_AddInterval(
			&Left, &Right, 
			axl_win_TPropertyLine_GetPartX(pLine, axl_win_EPropertyLinePart_Icon), 
			axl_win_TPropertyLine_GetPartWidth(pLine, axl_win_EPropertyLinePart_Icon)
			);

	if (Mask & axl_win_EPropertyLineInvalidate_Button)
		_AddInterval(
			&Left, &Right, 
			axl_win_TPropertyLine_GetPartX(pLine, axl_win_EPropertyLinePart_Button), 
			axl_win_TPropertyLine_GetPartWidth(pLine, axl_win_EPropertyLinePart_Button)
			);

	Left -= pPropertyCtrl->m_FirstVisibleX;
	Right -= pPropertyCtrl->m_FirstVisibleX;

	GetClientRect(pPropertyCtrl->m_hWnd, &Rect);
	Rect.top = (long) (pLine->m_VisibleIndex - pPropertyCtrl->m_FirstVisibleLine) * pPropertyCtrl->m_LineHeight;
	Rect.bottom = Rect.top + pPropertyCtrl->m_LineHeight;
	Rect.left = max(Rect.left, Left);
	Rect.right = min(Rect.right, Right);
	InvalidateRect(pPropertyCtrl->m_hWnd, &Rect, false);
}

void 
AXL_API
axl_win_TPropertyLine_SetFlags(
	axl_win_TPropertyLine* pLine,
	int Flags
	)
{
	if (pLine->m_Flags == Flags)
		return;

	pLine->m_Flags = Flags;
	axl_win_TPropertyCtrl_UpdateLineWidth(pLine->m_pPropertyCtrl, pLine);
	axl_win_TPropertyLine_Invalidate(pLine, axl_win_EPropertyLineInvalidate_All);
}

void 
AXL_API
axl_win_TPropertyLine_SetDelimiters(
	axl_win_TPropertyLine* pLine,
	int Delimiters
	)
{
	if (pLine->m_Delimiters == Delimiters)
		return;

	pLine->m_Delimiters = Delimiters;
	axl_win_TPropertyLine_Invalidate(pLine, axl_win_EPropertyLineInvalidate_All);
}

void 
AXL_API
axl_win_TPropertyLine_ModifyFlags(
	axl_win_TPropertyLine* pLine,
	int Modifier,
	bool_t Set
	)
{
	axl_win_TPropertyLine_SetFlags(
		pLine, Set ? 
		pLine->m_Flags | Modifier : 
		pLine->m_Flags & ~Modifier
		);
}

void 
AXL_API
axl_win_TPropertyLine_SetName(
	axl_win_TPropertyLine* pLine,
	const tchar_t* pString
	)
{
	axl_rtl_TString_CopyString(&pLine->m_Name, pString, -1);
	
	if (pLine->m_Flags & axl_win_EPropertyLine_NameHyperText)
	{
		axl_gr_THyperText_SetHyperText(&pLine->m_NameHyperText, pString, -1, NULL);
		axl_gr_THyperText_CalcHyperlinkXMap(&pLine->m_NameHyperText, &pLine->m_pPropertyCtrl->m_TextPaint.m_Font);
	}

	if (axl_win_TPropertyLine_IsSinglePane(pLine))
		axl_win_TPropertyCtrl_UpdateLineWidth(pLine->m_pPropertyCtrl, pLine);
	
	axl_win_TPropertyLine_Invalidate(pLine, axl_win_EPropertyLineInvalidate_Name);
}

void 
AXL_API
axl_win_TPropertyLine_SetValue(
	axl_win_TPropertyLine* pLine,
	const tchar_t* pString
	)
{
	axl_rtl_TString_CopyString(&pLine->m_Value, pString, -1);

	if (pLine->m_Flags & axl_win_EPropertyLine_ValueHyperText)
	{
		axl_gr_THyperText_SetHyperText(&pLine->m_ValueHyperText, pString, -1, NULL);
		axl_gr_THyperText_CalcHyperlinkXMap(&pLine->m_ValueHyperText, &pLine->m_pPropertyCtrl->m_TextPaint.m_Font);
	}

	if (!axl_win_TPropertyLine_IsSinglePane(pLine))
		axl_win_TPropertyCtrl_UpdateLineWidth(pLine->m_pPropertyCtrl, pLine);

	axl_win_TPropertyLine_Invalidate(pLine, axl_win_EPropertyLineInvalidate_Value);
}

void 
AXL_API
axl_win_TPropertyLine_SetOptionBoxType(
	axl_win_TPropertyLine* pLine,
	int Type
	)
{
	if (pLine->m_OptionBoxType == Type)
		return;

	pLine->m_OptionBoxType = Type;

	if (axl_win_TPropertyLine_IsSinglePane(pLine))
		axl_win_TPropertyCtrl_UpdateLineWidth(pLine->m_pPropertyCtrl, pLine);

	axl_win_TPropertyLine_Invalidate(pLine, axl_win_EPropertyLineInvalidate_Left);
}

void 
AXL_API
axl_win_TPropertyLine_UncheckRadioButtonGroup(axl_win_TPropertyLine* pLine)
{
	axl_win_TPropertyLine* pSibling;

	pSibling = axl_win_TPropertyLine_GetPrevSibling(pLine);
	while (pSibling && pSibling->m_OptionBoxType == axl_win_EPropertyOptionBoxType_RadioButton)
	{
		axl_win_TPropertyLine_SetOptionBoxCheckState(pSibling, false);
		pSibling = axl_win_TPropertyLine_GetPrevSibling(pSibling);
	}

	pSibling = axl_win_TPropertyLine_GetNextSibling(pLine);
	while (pSibling && pSibling->m_OptionBoxType == axl_win_EPropertyOptionBoxType_RadioButton)
	{
		axl_win_TPropertyLine_SetOptionBoxCheckState(pSibling, false);
		pSibling = axl_win_TPropertyLine_GetNextSibling(pSibling);
	}
}

void 
AXL_API
axl_win_TPropertyLine_SetOptionBoxCheckState(
	axl_win_TPropertyLine* pLine,
	int State
	)
{
	if (pLine->m_OptionBoxCheckState == State)
		return;

	if (pLine->m_OptionBoxType == axl_win_EPropertyOptionBoxType_RadioButton && State)
		axl_win_TPropertyLine_UncheckRadioButtonGroup(pLine);

	pLine->m_OptionBoxCheckState = State;
	axl_win_TPropertyLine_Invalidate(pLine, axl_win_EPropertyLineInvalidate_OptionBox);
}

void 
AXL_API
axl_win_TPropertyLine_SetOptionBoxVolatileState(
	axl_win_TPropertyLine* pLine,
	int State
	)
{
	if (pLine->m_OptionBoxVolatileState == State)
		return;

	pLine->m_OptionBoxVolatileState = State;
	axl_win_TPropertyLine_Invalidate(pLine, axl_win_EPropertyLineInvalidate_OptionBox);
}

void 
AXL_API
axl_win_TPropertyLine_SetButtonCheckState(
	axl_win_TPropertyLine* pLine,
	int State
	)
{
	if (pLine->m_ButtonCheckState == State)
		return;

	pLine->m_ButtonCheckState = State;
	axl_win_TPropertyLine_Invalidate(pLine, axl_win_EPropertyLineInvalidate_Button);
}


void 
AXL_API
axl_win_TPropertyLine_SetButtonVolatileState(
	axl_win_TPropertyLine* pLine,
	int State
	)
{
	if (pLine->m_ButtonVolatileState == State)
		return;

	pLine->m_ButtonVolatileState = State;
	axl_win_TPropertyLine_Invalidate(pLine, axl_win_EPropertyLineInvalidate_Button);
}

void 
AXL_API
axl_win_TPropertyLine_SetNameAttr(
	axl_win_TPropertyLine* pLine,
	axl_gr_TTextAttr* pAttr
	)
{
	ulong_t OldFontFlags = pLine->m_NameAttr.m_FontFlags;

	pLine->m_NameAttr = pAttr ? *pAttr : g_axl_gr_TTextAttr_NullAttr;

	if (axl_win_TPropertyLine_IsSinglePane(pLine))
		axl_win_TPropertyCtrl_UpdateLineWidth(pLine->m_pPropertyCtrl, pLine);

	axl_win_TPropertyLine_Invalidate(pLine, axl_win_EPropertyLineInvalidate_Name);
}

void 
AXL_API
axl_win_TPropertyLine_SetValueAttr(
	axl_win_TPropertyLine* pLine,
	axl_gr_TTextAttr* pAttr
	)
{
	ulong_t OldFontFlags = pLine->m_NameAttr.m_FontFlags;

	pLine->m_ValueAttr = pAttr ? *pAttr : g_axl_gr_TTextAttr_NullAttr;

	if (!axl_win_TPropertyLine_IsSinglePane(pLine))
		axl_win_TPropertyCtrl_UpdateLineWidth(pLine->m_pPropertyCtrl, pLine);

	axl_win_TPropertyLine_Invalidate(pLine, axl_win_EPropertyLineInvalidate_Value);
}

void 
AXL_API
axl_win_TPropertyLine_SetIcon(
	axl_win_TPropertyLine* pLine,
	int Icon
	)
{
	int Invalidate;

	if (pLine->m_Icon == Icon)
		return;

	if (Icon == -1 && pLine->m_Icon != -1 || Icon != -1 && pLine->m_Icon == -1)
		Invalidate = axl_win_EPropertyLineInvalidate_Left;
	else
		Invalidate = axl_win_EPropertyLineInvalidate_Icon;

	pLine->m_Icon = Icon;

	if (axl_win_TPropertyLine_IsSinglePane(pLine))
		axl_win_TPropertyCtrl_UpdateLineWidth(pLine->m_pPropertyCtrl, pLine);

	axl_win_TPropertyLine_Invalidate(pLine, Invalidate);
}

void 
AXL_API
axl_win_TPropertyLine_SetIndent(
	axl_win_TPropertyLine* pLine,
	int Indent
	)
{
	if (pLine->m_Indent == Indent)
		return;

	pLine->m_Indent = Indent;

	if (axl_win_TPropertyLine_IsSinglePane(pLine))
		axl_win_TPropertyCtrl_UpdateLineWidth(pLine->m_pPropertyCtrl, pLine);

	axl_win_TPropertyLine_Invalidate(pLine, axl_win_EPropertyLineInvalidate_Left);
}

void
AXL_API
axl_win_TPropertyLine_ModifyChildrenHeight(
	axl_win_TPropertyLine* pLine,
	long Delta	
	)
{
	pLine->m_ChildrenHeight += Delta;

	while ((pLine->m_Flags & axl_win_EPropertyLine_IsExpanded) && pLine->m_pParent)
	{
		pLine = pLine->m_pParent;
		pLine->m_ChildrenHeight += Delta;
	}
}

bool_t
AXL_API
axl_win_TPropertyLine_CalcWidth(
	axl_win_TPropertyLine* pLine,
	int Flags
	)
{
	axl_win_TPropertyCtrl* pPropertyCtrl = pLine->m_pPropertyCtrl;
	bool_t HasChanged = false;

	if ((Flags & axl_win_EPropertyCalcWidth_Menu) && (pLine->m_Flags & axl_win_EPropertyLine_Menu))
	{
		ulong_t MenuWidth = pLine->m_SinglePaneWidth;

		axl_win_TPropertyLine* pSibling;

		pSibling = axl_win_TPropertyLine_GetPrevSibling(pLine);
		while (pSibling && (pSibling->m_Flags & axl_win_EPropertyLine_Menu))
		{
			if (pSibling->m_SinglePaneWidth > MenuWidth)
				MenuWidth = pSibling->m_SinglePaneWidth;

			pSibling = axl_win_TPropertyLine_GetPrevSibling(pSibling);
		}

		pSibling = axl_win_TPropertyLine_GetNextSibling(pLine);
		while (pSibling && (pSibling->m_Flags & axl_win_EPropertyLine_Menu))
		{
			if (pSibling->m_SinglePaneWidth > MenuWidth)
				MenuWidth = pSibling->m_SinglePaneWidth;

			pSibling = axl_win_TPropertyLine_GetNextSibling(pSibling);
		}

		pLine->m_MenuWidth = MenuWidth;
	}

	if (Flags & axl_win_EPropertyCalcWidth_Line)
	{
		HFONT hFont = axl_gr_TFontMod_GetFont(&pPropertyCtrl->m_TextPaint.m_Font, pLine->m_NameAttr.m_FontFlags);
		size_t Length = axl_rtl_TString_GetLength(&pLine->m_Name);
		SIZE Size = axl_gr_TFont__CalcStringSize(hFont, pLine->m_Name.m_p, Length);

		pLine->m_NameWidth = Size.cx;

		if (axl_win_TPropertyLine_IsSinglePane(pLine))
		{
            Size.cx += axl_win_TPropertyLine_GetPartX(pLine, axl_win_EPropertyLinePart_Name);

			if (pLine->m_Flags & axl_win_EPropertyLine_Menu)
				Size.cx += pPropertyCtrl->m_IndentWidth;

			if (pLine->m_SinglePaneWidth != Size.cx || pLine->m_ValueWidth != 0)
			{
				pLine->m_SinglePaneWidth = Size.cx;
				pLine->m_ValueWidth = 0;
				HasChanged = true;
			}
		}
		else
		{
			HFONT hFont = axl_gr_TFontMod_GetFont(&pPropertyCtrl->m_TextPaint.m_Font, pLine->m_ValueAttr.m_FontFlags);
			size_t Length = axl_rtl_TString_GetLength(&pLine->m_Value);
			SIZE Size = axl_gr_TFont__CalcStringSize(hFont, pLine->m_Value.m_p, Length);

			if (pLine->m_SinglePaneWidth != 0 || pLine->m_ValueWidth != Size.cx)
			{
				pLine->m_SinglePaneWidth = 0;
				pLine->m_ValueWidth = Size.cx;
				HasChanged = true;
			}
		}
	}

	if (Flags & axl_win_EPropertyCalcWidth_Children)
	{
		axl_win_TPropertyLine* pChild;

		ulong_t MaxSinglePaneWidth = 0;
		ulong_t MaxValueWidth = 0;

		pChild = (axl_win_TPropertyLine*) axl_rtl_TList_GetHead(&pLine->m_ChildrenList);
		while (pChild)
		{
			ulong_t SinglePaneWidth = 0;
			ulong_t ValueWidth = 0;

			if (Flags & axl_win_EPropertyCalcWidth_Recursive)
				axl_win_TPropertyLine_CalcWidth(pChild, Flags);

			if (pChild->m_Flags & axl_win_EPropertyLine_IsExpanded)
			{
				SinglePaneWidth = max(pChild->m_ChildrenSinglePaneWidth, pChild->m_SinglePaneWidth);
				ValueWidth = max(pChild->m_ChildrenValueWidth, pChild->m_ValueWidth);
			}
			else
			{
				SinglePaneWidth = pChild->m_SinglePaneWidth;
				ValueWidth = pChild->m_ValueWidth;
			}

			if (SinglePaneWidth > MaxSinglePaneWidth)
				MaxSinglePaneWidth = SinglePaneWidth;

			if (ValueWidth > MaxValueWidth)
				MaxValueWidth = ValueWidth;

			pChild = (axl_win_TPropertyLine*) axl_rtl_TList_GetNext(&pLine->m_ChildrenList, (axl_rtl_TListEntry*) pChild);
		}

		if (MaxValueWidth != pLine->m_ChildrenValueWidth || MaxSinglePaneWidth != pLine->m_ChildrenSinglePaneWidth)
			HasChanged = true;

		pLine->m_ChildrenValueWidth = MaxValueWidth;
		pLine->m_ChildrenSinglePaneWidth = MaxSinglePaneWidth;
	}

	if (Flags & axl_win_EPropertyCalcWidth_Parent)
	{
		while (pLine->m_pParent)
		{
			HasChanged = axl_win_TPropertyLine_CalcWidth(pLine->m_pParent, axl_win_EPropertyCalcWidth_Children);
			if (!HasChanged)
				return false;

			pLine = pLine->m_pParent;
		}

		HasChanged = true;
	}

	return HasChanged;
}

bool_t
AXL_API
axl_win_TPropertyLine_IsLastChild(axl_win_TPropertyLine* pLine)
{
	if (pLine->m_pParent)
		return pLine == (axl_win_TPropertyLine*) axl_rtl_TList_GetTail(&pLine->m_pParent->m_ChildrenList);
	else 
		return pLine == (axl_win_TPropertyLine*) axl_rtl_TList_GetTail(pLine->m_pPropertyCtrl->m_pLineList);
}

bool_t
AXL_API
axl_win_TPropertyLine_HasGridLines(axl_win_TPropertyLine* pLine)
{
	if (pLine->m_Flags & axl_win_EPropertyLine_ForceNoGridLines)
		return false;
	else if (pLine->m_pParent)
		return (pLine->m_pParent->m_Flags & axl_win_EPropertyLine_NoChildGridLines) == 0;
	else
		return pLine->m_pPropertyCtrl->m_HasGridLinesAtRoot;
}

bool_t
AXL_API
axl_win_TPropertyLine_HasNameDelimiter(axl_win_TPropertyLine* pLine)
{
	axl_win_TPropertyCtrl* pPropertyCtrl = pLine->m_pPropertyCtrl;
	axl_win_TPropertyLine* pNextLine;

	size_t LastVisibleLine = axl_rtl_TPtrArray_GetCount(&pPropertyCtrl->m_VisibleLineArray) - 1;

	if (pLine->m_VisibleIndex == LastVisibleLine) 
		return !axl_win_TPropertyCtrl_IsMenu(pPropertyCtrl);

	if (pLine->m_Delimiters & axl_win_EPropertyLineDelimiter_NameBottom)
		return true;

	if (pLine->m_VisibleIndex == -1 || pLine->m_VisibleIndex > axl_rtl_TPtrArray_GetCount(&pPropertyCtrl->m_VisibleLineArray))
		return false;

	pNextLine = ((axl_win_TPropertyLine**) pPropertyCtrl->m_VisibleLineArray.m_p)[pLine->m_VisibleIndex + 1];
	return (pNextLine->m_Delimiters & axl_win_EPropertyLineDelimiter_NameTop) != 0;
}

bool_t
AXL_API
axl_win_TPropertyLine_HasValueDelimiter(axl_win_TPropertyLine* pLine)
{
	axl_win_TPropertyCtrl* pPropertyCtrl = pLine->m_pPropertyCtrl;
	axl_win_TPropertyLine* pNextLine;

	size_t LastVisibleLine = axl_rtl_TPtrArray_GetCount(&pLine->m_pPropertyCtrl->m_VisibleLineArray) - 1;

	if (pLine->m_VisibleIndex == LastVisibleLine) 
		return !axl_win_TPropertyCtrl_IsMenu(pPropertyCtrl);

	if (pLine->m_Delimiters & axl_win_EPropertyLineDelimiter_ValueBottom)
		return true;

	if (pLine->m_VisibleIndex == -1 || pLine->m_VisibleIndex > axl_rtl_TPtrArray_GetCount(&pPropertyCtrl->m_VisibleLineArray))
		return false;

	pNextLine = ((axl_win_TPropertyLine**) pPropertyCtrl->m_VisibleLineArray.m_p)[pLine->m_VisibleIndex + 1];
	return (pNextLine->m_Delimiters & axl_win_EPropertyLineDelimiter_ValueTop) != 0;
}

void 
AXL_API
axl_win_TPropertyLine_ExpandCollapse(
	axl_win_TPropertyLine* pLine,
	bool_t IsExpanded
	)
{
	axl_win_TPropertyCtrl_ExpandCollapse(pLine->m_pPropertyCtrl, pLine, IsExpanded);
}

//.............................................................................

