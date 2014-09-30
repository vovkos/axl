#include "axl_Precomp.h"
#include "axl_win_PropertyCtrl.h"

//.............................................................................

void 
AXL_API
axl_win_TPropertyCtrl_OnKeyDown_Home(
	axl_win_TPropertyCtrl* propertyCtrl,
	bool_t shift, 
	bool_t ctrl
	)
{
	axl_win_TPropertyCtrl_AdvanceSelection(propertyCtrl, MINLONG, true);
}

void 
AXL_API
axl_win_TPropertyCtrl_OnKeyDown_End(
	axl_win_TPropertyCtrl* propertyCtrl,
	bool_t shift, 
	bool_t ctrl
	)
{
	axl_win_TPropertyCtrl_AdvanceSelection(propertyCtrl, MAXLONG, true);
}

void 
AXL_API
axl_win_TPropertyCtrl_OnKeyDown_Left(
	axl_win_TPropertyCtrl* propertyCtrl,
	bool_t shift, 
	bool_t ctrl
	)
{
	axl_win_TPropertyLine* line;

	if (ctrl)
	{
		axl_win_TPropertyCtrl_OnScroll(propertyCtrl, SB_HORZ, SB_LINELEFT);
		return;
	}

	if (propertyCtrl->m_selectedLine == -1)
		return;

	line = ((axl_win_TPropertyLine**) propertyCtrl->m_visibleLineArray.m_p)[propertyCtrl->m_selectedLine]; 
	if (!axl_rtl_TList_IsEmpty(&line->m_childrenList))
		axl_win_TPropertyCtrl_Collapse(propertyCtrl, line);
}

void 
AXL_API
axl_win_TPropertyCtrl_OnKeyDown_Right(
	axl_win_TPropertyCtrl* propertyCtrl,
	bool_t shift, 
	bool_t ctrl
	)
{
	axl_win_TPropertyLine* line;

	if (ctrl)
	{
		axl_win_TPropertyCtrl_OnScroll(propertyCtrl, SB_HORZ, SB_LINERIGHT);
		return;
	}

	if (propertyCtrl->m_selectedLine == -1)
		return;

	line = ((axl_win_TPropertyLine**) propertyCtrl->m_visibleLineArray.m_p)[propertyCtrl->m_selectedLine]; 
	if (!axl_rtl_TList_IsEmpty(&line->m_childrenList))
		axl_win_TPropertyCtrl_Expand(propertyCtrl, line);
}

void 
AXL_API
axl_win_TPropertyCtrl_OnKeyDown_Up(
	axl_win_TPropertyCtrl* propertyCtrl,
	bool_t shift, 
	bool_t ctrl
	)
{
	if (ctrl)
		axl_win_TPropertyCtrl_OnScroll(propertyCtrl, SB_VERT, SB_LINEUP);
	else
		axl_win_TPropertyCtrl_AdvanceSelection(propertyCtrl, -1, true);
}

void 
AXL_API
axl_win_TPropertyCtrl_OnKeyDown_Down(
	axl_win_TPropertyCtrl* propertyCtrl,
	bool_t shift, 
	bool_t ctrl
	)
{
	if (ctrl)
		axl_win_TPropertyCtrl_OnScroll(propertyCtrl, SB_VERT, SB_LINEDOWN);
	else
		axl_win_TPropertyCtrl_AdvanceSelection(propertyCtrl, 1, true);
}

void 
AXL_API
axl_win_TPropertyCtrl_OnKeyDown_PageUp(
	axl_win_TPropertyCtrl* propertyCtrl,
	bool_t shift, 
	bool_t ctrl
	)
{
	axl_win_TPropertyCtrl_AdvanceSelection(propertyCtrl, - (long) propertyCtrl->m_visibleLineCount, true);
}

void 
AXL_API
axl_win_TPropertyCtrl_OnKeyDown_PageDown(
	axl_win_TPropertyCtrl* propertyCtrl,
	bool_t shift, 
	bool_t ctrl
	)
{
	axl_win_TPropertyCtrl_AdvanceSelection(propertyCtrl, (long) propertyCtrl->m_visibleLineCount, true);
}

void 
AXL_API
axl_win_TPropertyCtrl_OnKeyDown_Space(
	axl_win_TPropertyCtrl* propertyCtrl,
	bool_t shift,
	bool_t ctrl
	)
{
	axl_win_TPropertyLine* line;

	if (propertyCtrl->m_selectedLine == -1)
		return;

	line = ((axl_win_TPropertyLine**) propertyCtrl->m_visibleLineArray.m_p)[propertyCtrl->m_selectedLine]; 
	
	if (line->m_optionBoxType)
		axl_win_TPropertyLine_SetOptionBoxVolatileState(line, axl_win_EPropertyButtonVolatileState_Pushed);
	else if (axl_win_TPropertyLine_IsButton(line))
		axl_win_TPropertyLine_SetButtonVolatileState(line, axl_win_EPropertyButtonVolatileState_Pushed);
	else if (!axl_rtl_TList_IsEmpty(&line->m_childrenList))
		axl_win_TPropertyCtrl_ExpandCollapse(propertyCtrl, line, (line->m_flags & axl_win_EPropertyLine_IsExpanded) == 0);
}

void 
AXL_API
axl_win_TPropertyCtrl_ClickFirstHyperlink(
	axl_win_TPropertyCtrl* propertyCtrl,
	axl_win_TPropertyLine* line
	)
{
	axl_win_TPropertyHitTest hitTest = {0};

	if ((line->m_flags & axl_win_EPropertyLine_NameHyperText) && !axl_rtl_TArray_IsEmpty(&line->m_nameHyperText.m_hyperlinkArray))
	{
		hitTest.m_hitTest =  axl_win_EPropertyHitTest_Button;
		hitTest.m_line = line;
		hitTest.m_hyperlink = (axl_gr_THyperlinkAnchor*) line->m_nameHyperText.m_hyperlinkArray.m_p;
		axl_win_TPropertyCtrl_Notify(propertyCtrl, axl_win_EPropertyNotify_HyperlinkClick, &hitTest);
	}
	else if ((line->m_flags & axl_win_EPropertyLine_ValueHyperText) && !axl_rtl_TArray_IsEmpty(&line->m_valueHyperText.m_hyperlinkArray))
	{
		hitTest.m_hitTest =  axl_win_EPropertyHitTest_Button;
		hitTest.m_line = line;
		hitTest.m_hyperlink = (axl_gr_THyperlinkAnchor*) line->m_valueHyperText.m_hyperlinkArray.m_p;
		axl_win_TPropertyCtrl_Notify(propertyCtrl, axl_win_EPropertyNotify_HyperlinkClick, &hitTest);
	}
}

void 
AXL_API
axl_win_TPropertyCtrl_OnKeyUp_F4(
	axl_win_TPropertyCtrl* propertyCtrl,
	bool_t shift,
	bool_t ctrl
	)
{
	axl_win_TPropertyLine* line;

	if (propertyCtrl->m_selectedLine == -1)
		return;

	line = ((axl_win_TPropertyLine**) propertyCtrl->m_visibleLineArray.m_p)[propertyCtrl->m_selectedLine]; 
	axl_win_TPropertyCtrl_ClickFirstHyperlink(propertyCtrl, line);
}

void 
AXL_API
axl_win_TPropertyCtrl_OnKeyUp_Space(
	axl_win_TPropertyCtrl* propertyCtrl,
	bool_t shift,
	bool_t ctrl
	)
{
	axl_win_TPropertyLine* line;
	axl_win_TPropertyHitTest hitTest = {0};

	if (propertyCtrl->m_selectedLine == -1)
		return;

	line = ((axl_win_TPropertyLine**) propertyCtrl->m_visibleLineArray.m_p)[propertyCtrl->m_selectedLine]; 
	
	if (line->m_optionBoxType)
	{
		if (line->m_optionBoxVolatileState != axl_win_EPropertyButtonVolatileState_Pushed)
			return;

		hitTest.m_hitTest =  axl_win_EPropertyHitTest_OptionBox;
		hitTest.m_line = line;

		axl_win_TPropertyLine_SetOptionBoxVolatileState(line, 0);

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
	}
	else if (axl_win_TPropertyLine_IsButton(line))
	{
		if (line->m_buttonVolatileState != axl_win_EPropertyButtonVolatileState_Pushed)
			return;

		hitTest.m_hitTest =  axl_win_EPropertyHitTest_Button;
		hitTest.m_line = line;

		axl_win_TPropertyLine_SetButtonVolatileState(line, 0);
		axl_win_TPropertyCtrl_Notify(propertyCtrl, axl_win_EPropertyNotify_ButtonClick, &hitTest);
	}
	else
	{
		axl_win_TPropertyCtrl_ClickFirstHyperlink(propertyCtrl, line);
	}
}


//.............................................................................

void
AXL_API
axl_win_TPropertyCtrl_OnKeyDown(
	axl_win_TPropertyCtrl* propertyCtrl,
	int vkCode
	)
{
	bool_t shift = (getAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
	bool_t ctrl = (getAsyncKeyState(VK_CONTROL) & 0x8000) != 0;

	switch (vkCode)
	{
	case VK_HOME:
		axl_win_TPropertyCtrl_OnKeyDown_Home(propertyCtrl, shift, ctrl);
		break;

	case VK_END:
		axl_win_TPropertyCtrl_OnKeyDown_End(propertyCtrl, shift, ctrl);
		break;

	case VK_LEFT:
		axl_win_TPropertyCtrl_OnKeyDown_Left(propertyCtrl, shift, ctrl);
		break;

	case VK_RIGHT:
		axl_win_TPropertyCtrl_OnKeyDown_Right(propertyCtrl, shift, ctrl);
		break;

	case VK_UP:
		axl_win_TPropertyCtrl_OnKeyDown_Up(propertyCtrl, shift, ctrl);
		break;

	case VK_DOWN:
		axl_win_TPropertyCtrl_OnKeyDown_Down(propertyCtrl, shift, ctrl);
		break;

	case VK_PRIOR:
		axl_win_TPropertyCtrl_OnKeyDown_PageUp(propertyCtrl, shift, ctrl);
		break;

	case VK_NEXT:
		axl_win_TPropertyCtrl_OnKeyDown_PageDown(propertyCtrl, shift, ctrl);
		break; 

	case VK_SPACE:
	case VK_RETURN:
 		axl_win_TPropertyCtrl_OnKeyDown_Space(propertyCtrl, shift, ctrl);
		break;
	}; 
}

void
AXL_API
axl_win_TPropertyCtrl_OnKeyUp(
	axl_win_TPropertyCtrl* propertyCtrl,
	int vkCode
	)
{
	bool_t shift = (getAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
	bool_t ctrl = (getAsyncKeyState(VK_CONTROL) & 0x8000) != 0;

	switch (vkCode)
	{
	case VK_F4:
		axl_win_TPropertyCtrl_OnKeyUp_F4(propertyCtrl, shift, ctrl);
		break; 

	case VK_SPACE:
	case VK_RETURN:
		axl_win_TPropertyCtrl_OnKeyUp_Space(propertyCtrl, shift, ctrl);
		break;
	}; 
}

//.............................................................................
