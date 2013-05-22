#include "axl_Precomp.h"
#include "axl_win_PropertyCtrl.h"

//.............................................................................

void 
AXL_API
axl_win_TPropertyCtrl_OnKeyDown_Home(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	bool_t Shift, 
	bool_t Ctrl
	)
{
	axl_win_TPropertyCtrl_AdvanceSelection(pPropertyCtrl, MINLONG, true);
}

void 
AXL_API
axl_win_TPropertyCtrl_OnKeyDown_End(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	bool_t Shift, 
	bool_t Ctrl
	)
{
	axl_win_TPropertyCtrl_AdvanceSelection(pPropertyCtrl, MAXLONG, true);
}

void 
AXL_API
axl_win_TPropertyCtrl_OnKeyDown_Left(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	bool_t Shift, 
	bool_t Ctrl
	)
{
	axl_win_TPropertyLine* pLine;

	if (Ctrl)
	{
		axl_win_TPropertyCtrl_OnScroll(pPropertyCtrl, SB_HORZ, SB_LINELEFT);
		return;
	}

	if (pPropertyCtrl->m_SelectedLine == -1)
		return;

	pLine = ((axl_win_TPropertyLine**) pPropertyCtrl->m_VisibleLineArray.m_p)[pPropertyCtrl->m_SelectedLine]; 
	if (!axl_rtl_TList_IsEmpty(&pLine->m_ChildrenList))
		axl_win_TPropertyCtrl_Collapse(pPropertyCtrl, pLine);
}

void 
AXL_API
axl_win_TPropertyCtrl_OnKeyDown_Right(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	bool_t Shift, 
	bool_t Ctrl
	)
{
	axl_win_TPropertyLine* pLine;

	if (Ctrl)
	{
		axl_win_TPropertyCtrl_OnScroll(pPropertyCtrl, SB_HORZ, SB_LINERIGHT);
		return;
	}

	if (pPropertyCtrl->m_SelectedLine == -1)
		return;

	pLine = ((axl_win_TPropertyLine**) pPropertyCtrl->m_VisibleLineArray.m_p)[pPropertyCtrl->m_SelectedLine]; 
	if (!axl_rtl_TList_IsEmpty(&pLine->m_ChildrenList))
		axl_win_TPropertyCtrl_Expand(pPropertyCtrl, pLine);
}

void 
AXL_API
axl_win_TPropertyCtrl_OnKeyDown_Up(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	bool_t Shift, 
	bool_t Ctrl
	)
{
	if (Ctrl)
		axl_win_TPropertyCtrl_OnScroll(pPropertyCtrl, SB_VERT, SB_LINEUP);
	else
		axl_win_TPropertyCtrl_AdvanceSelection(pPropertyCtrl, -1, true);
}

void 
AXL_API
axl_win_TPropertyCtrl_OnKeyDown_Down(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	bool_t Shift, 
	bool_t Ctrl
	)
{
	if (Ctrl)
		axl_win_TPropertyCtrl_OnScroll(pPropertyCtrl, SB_VERT, SB_LINEDOWN);
	else
		axl_win_TPropertyCtrl_AdvanceSelection(pPropertyCtrl, 1, true);
}

void 
AXL_API
axl_win_TPropertyCtrl_OnKeyDown_PageUp(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	bool_t Shift, 
	bool_t Ctrl
	)
{
	axl_win_TPropertyCtrl_AdvanceSelection(pPropertyCtrl, - (long) pPropertyCtrl->m_VisibleLineCount, true);
}

void 
AXL_API
axl_win_TPropertyCtrl_OnKeyDown_PageDown(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	bool_t Shift, 
	bool_t Ctrl
	)
{
	axl_win_TPropertyCtrl_AdvanceSelection(pPropertyCtrl, (long) pPropertyCtrl->m_VisibleLineCount, true);
}

void 
AXL_API
axl_win_TPropertyCtrl_OnKeyDown_Space(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	bool_t Shift,
	bool_t Ctrl
	)
{
	axl_win_TPropertyLine* pLine;

	if (pPropertyCtrl->m_SelectedLine == -1)
		return;

	pLine = ((axl_win_TPropertyLine**) pPropertyCtrl->m_VisibleLineArray.m_p)[pPropertyCtrl->m_SelectedLine]; 
	
	if (pLine->m_OptionBoxType)
		axl_win_TPropertyLine_SetOptionBoxVolatileState(pLine, axl_win_EPropertyButtonVolatileState_Pushed);
	else if (axl_win_TPropertyLine_IsButton(pLine))
		axl_win_TPropertyLine_SetButtonVolatileState(pLine, axl_win_EPropertyButtonVolatileState_Pushed);
	else if (!axl_rtl_TList_IsEmpty(&pLine->m_ChildrenList))
		axl_win_TPropertyCtrl_ExpandCollapse(pPropertyCtrl, pLine, (pLine->m_Flags & axl_win_EPropertyLine_IsExpanded) == 0);
}

void 
AXL_API
axl_win_TPropertyCtrl_ClickFirstHyperlink(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	axl_win_TPropertyLine* pLine
	)
{
	axl_win_TPropertyHitTest HitTest = {0};

	if ((pLine->m_Flags & axl_win_EPropertyLine_NameHyperText) && !axl_rtl_TArray_IsEmpty(&pLine->m_NameHyperText.m_HyperlinkArray))
	{
		HitTest.m_HitTest =  axl_win_EPropertyHitTest_Button;
		HitTest.m_pLine = pLine;
		HitTest.m_pHyperlink = (axl_gr_THyperlinkAnchor*) pLine->m_NameHyperText.m_HyperlinkArray.m_p;
		axl_win_TPropertyCtrl_Notify(pPropertyCtrl, axl_win_EPropertyNotify_HyperlinkClick, &HitTest);
	}
	else if ((pLine->m_Flags & axl_win_EPropertyLine_ValueHyperText) && !axl_rtl_TArray_IsEmpty(&pLine->m_ValueHyperText.m_HyperlinkArray))
	{
		HitTest.m_HitTest =  axl_win_EPropertyHitTest_Button;
		HitTest.m_pLine = pLine;
		HitTest.m_pHyperlink = (axl_gr_THyperlinkAnchor*) pLine->m_ValueHyperText.m_HyperlinkArray.m_p;
		axl_win_TPropertyCtrl_Notify(pPropertyCtrl, axl_win_EPropertyNotify_HyperlinkClick, &HitTest);
	}
}

void 
AXL_API
axl_win_TPropertyCtrl_OnKeyUp_F4(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	bool_t Shift,
	bool_t Ctrl
	)
{
	axl_win_TPropertyLine* pLine;

	if (pPropertyCtrl->m_SelectedLine == -1)
		return;

	pLine = ((axl_win_TPropertyLine**) pPropertyCtrl->m_VisibleLineArray.m_p)[pPropertyCtrl->m_SelectedLine]; 
	axl_win_TPropertyCtrl_ClickFirstHyperlink(pPropertyCtrl, pLine);
}

void 
AXL_API
axl_win_TPropertyCtrl_OnKeyUp_Space(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	bool_t Shift,
	bool_t Ctrl
	)
{
	axl_win_TPropertyLine* pLine;
	axl_win_TPropertyHitTest HitTest = {0};

	if (pPropertyCtrl->m_SelectedLine == -1)
		return;

	pLine = ((axl_win_TPropertyLine**) pPropertyCtrl->m_VisibleLineArray.m_p)[pPropertyCtrl->m_SelectedLine]; 
	
	if (pLine->m_OptionBoxType)
	{
		if (pLine->m_OptionBoxVolatileState != axl_win_EPropertyButtonVolatileState_Pushed)
			return;

		HitTest.m_HitTest =  axl_win_EPropertyHitTest_OptionBox;
		HitTest.m_pLine = pLine;

		axl_win_TPropertyLine_SetOptionBoxVolatileState(pLine, 0);

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
	}
	else if (axl_win_TPropertyLine_IsButton(pLine))
	{
		if (pLine->m_ButtonVolatileState != axl_win_EPropertyButtonVolatileState_Pushed)
			return;

		HitTest.m_HitTest =  axl_win_EPropertyHitTest_Button;
		HitTest.m_pLine = pLine;

		axl_win_TPropertyLine_SetButtonVolatileState(pLine, 0);
		axl_win_TPropertyCtrl_Notify(pPropertyCtrl, axl_win_EPropertyNotify_ButtonClick, &HitTest);
	}
	else
	{
		axl_win_TPropertyCtrl_ClickFirstHyperlink(pPropertyCtrl, pLine);
	}
}


//.............................................................................

void
AXL_API
axl_win_TPropertyCtrl_OnKeyDown(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	int VkCode
	)
{
	bool_t Shift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
	bool_t Ctrl = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;

	switch (VkCode)
	{
	case VK_HOME:
		axl_win_TPropertyCtrl_OnKeyDown_Home(pPropertyCtrl, Shift, Ctrl);
		break;

	case VK_END:
		axl_win_TPropertyCtrl_OnKeyDown_End(pPropertyCtrl, Shift, Ctrl);
		break;

	case VK_LEFT:
		axl_win_TPropertyCtrl_OnKeyDown_Left(pPropertyCtrl, Shift, Ctrl);
		break;

	case VK_RIGHT:
		axl_win_TPropertyCtrl_OnKeyDown_Right(pPropertyCtrl, Shift, Ctrl);
		break;

	case VK_UP:
		axl_win_TPropertyCtrl_OnKeyDown_Up(pPropertyCtrl, Shift, Ctrl);
		break;

	case VK_DOWN:
		axl_win_TPropertyCtrl_OnKeyDown_Down(pPropertyCtrl, Shift, Ctrl);
		break;

	case VK_PRIOR:
		axl_win_TPropertyCtrl_OnKeyDown_PageUp(pPropertyCtrl, Shift, Ctrl);
		break;

	case VK_NEXT:
		axl_win_TPropertyCtrl_OnKeyDown_PageDown(pPropertyCtrl, Shift, Ctrl);
		break; 

	case VK_SPACE:
	case VK_RETURN:
 		axl_win_TPropertyCtrl_OnKeyDown_Space(pPropertyCtrl, Shift, Ctrl);
		break;
	}; 
}

void
AXL_API
axl_win_TPropertyCtrl_OnKeyUp(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	int VkCode
	)
{
	bool_t Shift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
	bool_t Ctrl = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;

	switch (VkCode)
	{
	case VK_F4:
		axl_win_TPropertyCtrl_OnKeyUp_F4(pPropertyCtrl, Shift, Ctrl);
		break; 

	case VK_SPACE:
	case VK_RETURN:
		axl_win_TPropertyCtrl_OnKeyUp_Space(pPropertyCtrl, Shift, Ctrl);
		break;
	}; 
}

//.............................................................................
