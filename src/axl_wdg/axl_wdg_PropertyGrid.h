// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_rtl_List.h"
#include "axl_rtl_Array.h"
#include "axl_rtl_String.h"
#include "axl_rtl_RbTree.h"

#include "axl_gr_TextPaint.h"
#include "axl_gr_HyperText.h"
#include "axl_gr_StockCtrlPaint.h"
#include "axl_gr_OffscreenBuffer.h"

#include "axl_obj.h"

#ifdef __cplusplus
extern "C" { 
#endif

//.............................................................................

typedef struct _axl_win_TPropertyCtrl    axl_win_TPropertyCtrl;
typedef struct _axl_win_TPropertyLine    axl_win_TPropertyLine;
typedef struct _axl_win_TPropertyInPlace axl_win_TPropertyInPlace;

#define axl_win_TPropertyCtrl_WndClassName _T("axl.propertyctrl")

//.............................................................................

enum
{
	axl_win_EPropertyLine_SinglePane           = 0x0001,
	axl_win_EPropertyLine_IsExpanded           = 0x0002,
	axl_win_EPropertyLine_IsDisabled           = 0x0004,
	axl_win_EPropertyLine_NotSelectable        = 0x0008,
	axl_win_EPropertyLine_Button               = 0x0010,
	axl_win_EPropertyLine_NameHyperText        = 0x0020,
	axl_win_EPropertyLine_ValueHyperText       = 0x0040,
	axl_win_EPropertyLine_Menu                 = 0x0080,
	axl_win_EPropertyLine_NoChildGridLines     = 0x0100,
	axl_win_EPropertyLine_ForceNoGridLines     = 0x0200,
	axl_win_EPropertyLine_AlwaysDrawButton     = 0x0400,
	axl_win_EPropertyLine_NameShadow           = 0x0800,
	axl_win_EPropertyLine_ValueShadow          = 0x1000,
	axl_win_EPropertyLine_ReArrangeable        = 0x2000,

	axl_win_EPropertyLine_Separator = 
		axl_win_EPropertyLine_SinglePane | 
		axl_win_EPropertyLine_NotSelectable | 
		axl_win_EPropertyLine_ForceNoGridLines |
		axl_win_EPropertyLine_IsExpanded,
};

enum
{
	axl_win_EPropertyLineDelimiter_None        = 0x0,
	axl_win_EPropertyLineDelimiter_NameTop     = 0x1,
	axl_win_EPropertyLineDelimiter_NameBottom  = 0x2,
	axl_win_EPropertyLineDelimiter_ValueTop    = 0x4,
	axl_win_EPropertyLineDelimiter_ValueBottom = 0x8,
	axl_win_EPropertyLineDelimiter_All         = 0xf,

	axl_win_EPropertyLineDelimiter_Top         = axl_win_EPropertyLineDelimiter_NameTop | axl_win_EPropertyLineDelimiter_ValueTop,
	axl_win_EPropertyLineDelimiter_Bottom      = axl_win_EPropertyLineDelimiter_NameBottom | axl_win_EPropertyLineDelimiter_ValueBottom,
	axl_win_EPropertyLineDelimiter_Name        = axl_win_EPropertyLineDelimiter_NameTop | axl_win_EPropertyLineDelimiter_NameBottom,
	axl_win_EPropertyLineDelimiter_Value       = axl_win_EPropertyLineDelimiter_ValueTop | axl_win_EPropertyLineDelimiter_ValueBottom,
};

enum
{
	axl_win_EPropertyOptionBoxType_None = 0,
	axl_win_EPropertyOptionBoxType_CheckBox,
	axl_win_EPropertyOptionBoxType_RadioButton,
};

enum
{
	axl_win_EPropertyCheckState_Unchecked = 0,
	axl_win_EPropertyCheckState_Checked,
	axl_win_EPropertyCheckState_State3,
};

enum
{
	axl_win_EPropertyButtonVolatileState_Normal = 0,
	axl_win_EPropertyButtonVolatileState_Hover,
	axl_win_EPropertyButtonVolatileState_Pushed,
	axl_win_EPropertyButtonVolatileState_Disabled,
};

enum
{
	axl_win_EPropertyDrag_None = 0,
	axl_win_EPropertyDrag_Splitter,
	axl_win_EPropertyDrag_Selection,
	axl_win_EPropertyDrag_OptionBox,
	axl_win_EPropertyDrag_Button,
	axl_win_EPropertyDrag_Hyperlink,
	axl_win_EPropertyDrag_ReArrange,
};

enum
{
	axl_win_EPropertyHover_None = 0,
	axl_win_EPropertyHover_OptionBox,
	axl_win_EPropertyHover_Button,
	axl_win_EPropertyHover_Menu,
	axl_win_EPropertyHover_Leave = 0x100,
};

enum
{
	axl_win_EPropertyPlusMinusImage_Minus = 0,
	axl_win_EPropertyPlusMinusImage_Plus,		
};

enum
{
	axl_win_EPropertyGridLineImage_H = 0,
	axl_win_EPropertyGridLineImage_V,
	axl_win_EPropertyGridLineImage_T,
	axl_win_EPropertyGridLineImage_L,
};

enum
{
	axl_win_EPropertyColor_Name = axl_gr_EStdPalette_Size | axl_gr_EColorFlags_Index,		
	axl_win_EPropertyColor_SelectedName,		
	axl_win_EPropertyColor_NameBack,
	axl_win_EPropertyColor_Value,
	axl_win_EPropertyColor_ValueBack,
	axl_win_EPropertyColor_SelectedValue,
	axl_win_EPropertyColor_SelectedBack,
	axl_win_EPropertyColor_DisabledText,		
	axl_win_EPropertyColor_ButtonText,
	axl_win_EPropertyColor_HoverButtonText,
	axl_win_EPropertyColor_Empty,
	axl_win_EPropertyColor_Splitter,
	axl_win_EPropertyColor_NameDelimiter,
	axl_win_EPropertyColor_ValueDelimiter,
	axl_win_EPropertyColor_Grid,
	axl_win_EPropertyColor_PlusMinus,
	axl_win_EPropertyColor_PlusMinusRect,
	axl_win_EPropertyColor_PlusMinusBack,
	axl_win_EPropertyColor_Unused,

	axl_win_EPropertyColor_Count = ((axl_win_EPropertyColor_Unused + 1) & ~axl_gr_EColorFlags_Index)
};

enum
{
	axl_win_EPropertyHitTest_None = 0,
	axl_win_EPropertyHitTest_Indent,
	axl_win_EPropertyHitTest_PlusMinus,
	axl_win_EPropertyHitTest_ReArrange,
	axl_win_EPropertyHitTest_OptionBox,
	axl_win_EPropertyHitTest_Button,
	axl_win_EPropertyHitTest_Menu,
	axl_win_EPropertyHitTest_Icon,
	axl_win_EPropertyHitTest_Name,
	axl_win_EPropertyHitTest_NameTail,
	axl_win_EPropertyHitTest_Splitter,
	axl_win_EPropertyHitTest_Value,
	axl_win_EPropertyHitTest_ValueTail,
	axl_win_EPropertyHitTest_Hyperlink,
};

enum
{
	axl_win_EPropertyLineInvalidate_All       = 0xff,
	axl_win_EPropertyLineInvalidate_Left      = 0x01,
	axl_win_EPropertyLineInvalidate_Right     = 0x02,
	axl_win_EPropertyLineInvalidate_Name      = 0x04,
	axl_win_EPropertyLineInvalidate_Value     = 0x08,
	axl_win_EPropertyLineInvalidate_Icon      = 0x10,
	axl_win_EPropertyLineInvalidate_OptionBox = 0x20,
	axl_win_EPropertyLineInvalidate_Button    = 0x40,
};

enum
{
	axl_win_EPropertyNotify_ButtonClick    = NM_FIRST - 500,
	axl_win_EPropertyNotify_OptionBoxClick,
	axl_win_EPropertyNotify_HyperlinkClick,
	axl_win_EPropertyNotify_MenuClick,
	axl_win_EPropertyNotify_ReArrange,
	axl_win_EPropertyNotify_ExpandCollapse,
};

//.............................................................................

// inplace editors

typedef
bool_t
(AXL_API*
 axl_win_FPropertyInPlaceLoad)(
	axl_win_TPropertyInPlace* pInPlace,
	axl_win_TPropertyLine* pLine,
	const RECT* pRect,
	ulong_t* pMinWidth
	);

typedef
bool_t
(AXL_API*
 axl_win_FPropertyInPlaceSave)(
	axl_win_TPropertyInPlace* pInPlace,
	axl_win_TPropertyLine* pLine
	);

typedef
void
(AXL_API*
 axl_win_FPropertyInPlaceLayout)(
	axl_win_TPropertyInPlace* pInPlace,
	const RECT* pRect
	);

typedef
void
(AXL_API*
 axl_win_FPropertyInPlaceSetFocus)(axl_win_TPropertyInPlace* pInPlace);

//.............................................................................

void
AXL_API
axl_win_TPropertyInPlace_Construct(
	axl_win_TPropertyInPlace* pInPlace,
	axl_obj_TAgent* pAgent
	);

#define axl_win_TPropertyInPlace_Load(pInPlace, pLine, pRect, pMinWidth) \
	((pInPlace)->m_pfLoad(pInPlace, pLine, pRect, pMinWidth))

#define axl_win_TPropertyInPlace_Save(pInPlace, pLine) \
	((pInPlace)->m_pfSave(pInPlace, pLine))

#define axl_win_TPropertyInPlace_Layout(pInPlace, pRect) \
	((pInPlace)->m_pfLayout(pInPlace, pRect))

#define axl_win_TPropertyInPlace_SetFocus(pInPlace) \
	((pInPlace)->m_pfSetFocus(pInPlace))

#ifdef __cplusplus
typedef struct _axl_win_TPropertyInPlace: axl_obj_TObject {
#else
typedef struct _axl_win_TPropertyInPlace { axl_obj_TObject;
#endif
	axl_win_FPropertyInPlaceLoad m_pfLoad;
	axl_win_FPropertyInPlaceSave m_pfSave;
	axl_win_FPropertyInPlaceLayout m_pfLayout;
	axl_win_FPropertyInPlaceSetFocus m_pfSetFocus;
} axl_win_TPropertyInPlace;

//.............................................................................

// property ctrl line

enum
{
	axl_win_EPropertyLinePart_Name = 0,
	axl_win_EPropertyLinePart_Grid,
	axl_win_EPropertyLinePart_Value,
	axl_win_EPropertyLinePart_Icon,
	axl_win_EPropertyLinePart_OptionBox,
	axl_win_EPropertyLinePart_Button,
};

enum
{
	axl_win_EPropertyCalcWidth_Line      = 0x01,
	axl_win_EPropertyCalcWidth_Menu      = 0x02,
	axl_win_EPropertyCalcWidth_Children  = 0x04,
	axl_win_EPropertyCalcWidth_Parent    = 0x08,
	axl_win_EPropertyCalcWidth_Recursive = 0x10,
};

void 
AXL_API
axl_win_TPropertyLine_Construct(axl_win_TPropertyLine* pLine);

void 
AXL_API
axl_win_TPropertyLine_Destruct(axl_win_TPropertyLine* pLine);

void 
AXL_API
axl_win_TPropertyLine_ClearLineList(axl_rtl_TList* pLineList);

bool_t 
AXL_API
axl_win_TPropertyLine_Clone(
	axl_win_TPropertyLine* pLine,
	axl_win_TPropertyLine* pSrc
	);

long 
AXL_API
axl_win_TPropertyLine_GetPartX(
	axl_win_TPropertyLine* pLine,
	int Part
	);

bool_t
AXL_API
axl_win_TPropertyLine_IsPartFullyVisible(
	axl_win_TPropertyLine* pLine,
	int Part
	);

bool_t
AXL_API
axl_win_TPropertyLine_CalcWidth(
	axl_win_TPropertyLine* pLine,
	int Flags
	);

void
AXL_API
axl_win_TPropertyLine_ModifyChildrenHeight(
	axl_win_TPropertyLine* pLine,
	long Delta	
	);

void 
AXL_API
axl_win_TPropertyLine_SetOptionBoxVolatileState(
	axl_win_TPropertyLine* pLine,
	int State
	);

void 
AXL_API
axl_win_TPropertyLine_SetButtonVolatileState(
	axl_win_TPropertyLine* pLine,
	int State
	);

bool_t
AXL_API
axl_win_TPropertyLine_IsLastChild(axl_win_TPropertyLine* pLine);
 
bool_t
AXL_API
axl_win_TPropertyLine_HasGridLines(axl_win_TPropertyLine* pLine);

bool_t
AXL_API
axl_win_TPropertyLine_HasNameDelimiter(axl_win_TPropertyLine* pLine);

bool_t
AXL_API
axl_win_TPropertyLine_HasValueDelimiter(axl_win_TPropertyLine* pLine);

long 
AXL_API
axl_win_TPropertyLine_GetPartWidth(
	axl_win_TPropertyLine* pLine,
	int Part
	);

#define axl_win_TPropertyLine_HasPlusMinus(pLine) \
	(!axl_rtl_TList_IsEmpty(&(pLine)->m_ChildrenList) && !((pLine)->m_Flags & axl_win_EPropertyLine_Menu))

#define axl_win_TPropertyLine_IsSinglePane(pLine) \
	(((pLine)->m_Flags & (axl_win_EPropertyLine_SinglePane | axl_win_EPropertyLine_Menu)) != 0)

#define axl_win_TPropertyLine_GetFirstChild(pLine) \
	((axl_win_TPropertyLine*) axl_rtl_TList_GetHead(&pLine->m_ChildrenList))

#define axl_win_TPropertyLine_GetLastChild(pLine) \
	((axl_win_TPropertyLine*) axl_rtl_TList_GetTail(&pLine->m_ChildrenList))

axl_win_TPropertyLine* 
AXL_API
axl_win_TPropertyLine_GetNextSibling(axl_win_TPropertyLine* pLine);

axl_win_TPropertyLine* 
AXL_API
axl_win_TPropertyLine_GetPrevSibling(axl_win_TPropertyLine* pLine);

#define axl_win_TPropertyLine_SetInPlace(pLine, pInPlace) \
	axl_obj_TObject_CopyPtr(&pLine->m_pInPlace, pInPlace)

#define axl_win_TPropertyLine_IsReArrangeable(pLine) \
	((pLine)->m_pParent && ((pLine)->m_pParent->m_Flags & axl_win_EPropertyLine_ReArrangeable))

#define axl_win_TPropertyLine_IsButton(pLine) \
	((pLine->m_Flags & axl_win_EPropertyLine_Button) || \
	((pLine->m_Flags & axl_win_EPropertyLine_Menu) && !pLine->m_pPropertyCtrl->m_pMenuParentPropertyCtrl))

void 
AXL_API
axl_win_TPropertyLine_SetName(
	axl_win_TPropertyLine* pLine,
	const tchar_t* pString
	);

void 
AXL_API
axl_win_TPropertyLine_SetValue(
	axl_win_TPropertyLine* pLine,
	const tchar_t* pString
	);

void 
AXL_API
axl_win_TPropertyLine_SetNameAttr(
	axl_win_TPropertyLine* pLine,
	axl_gr_TTextAttr* pAttr
	);

void 
AXL_API
axl_win_TPropertyLine_SetValueAttr(
	axl_win_TPropertyLine* pLine,
	axl_gr_TTextAttr* pAttr
	);

void 
AXL_API
axl_win_TPropertyLine_SetIcon(
	axl_win_TPropertyLine* pLine,
	int Icon
	);

void 
AXL_API
axl_win_TPropertyLine_SetIndent(
	axl_win_TPropertyLine* pLine,
	int Indent
	);

void 
AXL_API
axl_win_TPropertyLine_SetOptionBoxType(
	axl_win_TPropertyLine* pLine,
	int Type
	);

void 
AXL_API
axl_win_TPropertyLine_SetOptionBoxCheckState(
	axl_win_TPropertyLine* pLine,
	int State
	);

void 
AXL_API
axl_win_TPropertyLine_SetButtonCheckState(
	axl_win_TPropertyLine* pLine,
	int State
	);

void 
AXL_API
axl_win_TPropertyLine_SetFlags(
	axl_win_TPropertyLine* pLine,
	int Flags
	);

void 
AXL_API
axl_win_TPropertyLine_SetDelimiters(
	axl_win_TPropertyLine* pLine,
	int Delimiters
	);

void 
AXL_API
axl_win_TPropertyLine_ModifyFlags(
	axl_win_TPropertyLine* pLine,
	int Modifier,
	bool_t Set
	);

void 
AXL_API
axl_win_TPropertyLine_Invalidate(
	axl_win_TPropertyLine* pLine,
	ulong_t Mask
	);

void 
AXL_API
axl_win_TPropertyLine_ExpandCollapse(
	axl_win_TPropertyLine* pLine,
	bool_t IsExpanded
	);

#ifdef __cplusplus
typedef struct _axl_win_TPropertyLine: axl_rtl_TListEntry {
#else
typedef struct _axl_win_TPropertyLine { axl_rtl_TListEntry;
#endif	
	int m_Flags;
	int m_Delimiters;
	int m_OptionBoxType;
	int m_OptionBoxCheckState;
	int m_OptionBoxVolatileState;
	int m_ButtonVolatileState;
	int m_ButtonCheckState;
	int m_Icon;
	int m_ToolTipIcon;
	int m_Level;
	int m_Indent;
	size_t m_VisibleIndex;
	size_t m_ChildrenHeight;
	ulong_t m_MenuWidth;
	ulong_t m_SinglePaneWidth;
	ulong_t m_NameWidth;
	ulong_t m_ValueWidth;
	ulong_t m_ChildrenSinglePaneWidth;
	ulong_t m_ChildrenValueWidth;
	axl_win_TPropertyCtrl* m_pPropertyCtrl;
	axl_win_TPropertyLine* m_pParent;
	axl_rtl_TList m_ChildrenList;
	axl_rtl_TString m_Name;
	axl_rtl_TString m_Value;
	axl_rtl_TString m_ToolTipTitle;
	axl_rtl_TString m_ToolTipText;
	axl_gr_TTextAttr m_NameAttr;
	axl_gr_TTextAttr m_ValueAttr;
	axl_gr_THyperText m_NameHyperText;
	axl_gr_THyperText m_ValueHyperText;
	axl_win_TPropertyInPlace* m_pInPlace;
	axl_obj_TData m_UserData;

#ifdef __cplusplus
	axl_win_TPropertyLine* GetFirstChild()
		{ return axl_win_TPropertyLine_GetFirstChild(this); }

	axl_win_TPropertyLine* GetLastChild()
		{ return axl_win_TPropertyLine_GetLastChild(this); }

	axl_win_TPropertyLine* GetNextSibling()
		{ return axl_win_TPropertyLine_GetNextSibling(this); }

	axl_win_TPropertyLine* GetPrevSibling()
		{ return axl_win_TPropertyLine_GetPrevSibling(this); }

	void SetInPlace(axl_win_TPropertyInPlace* pInPlace)
		{ axl_win_TPropertyLine_SetInPlace(this, pInPlace); }

	void SetName(const tchar_t* pString)
		{ axl_win_TPropertyLine_SetName(this, pString); }

	void SetValue(const tchar_t* pString)
		{ axl_win_TPropertyLine_SetValue(this, pString); }

	void SetNameAttr(axl_gr_TTextAttr* pAttr)
		{ axl_win_TPropertyLine_SetNameAttr(this, pAttr); }

	void SetValueAttr(axl_gr_TTextAttr* pAttr)
		{ axl_win_TPropertyLine_SetValueAttr(this, pAttr); }

	void SetIcon(int Icon)
		{ axl_win_TPropertyLine_SetIcon(this, Icon); }

	void SetIndent(int Indent)
		{ axl_win_TPropertyLine_SetIndent(this, Indent); }

	void SetOptionBoxType(int Type)
		{ axl_win_TPropertyLine_SetOptionBoxType(this, Type); }

	void SetOptionBoxCheckState(int State)
		{ axl_win_TPropertyLine_SetOptionBoxCheckState(this, State); }

	void SetButtonCheckState(int State)
		{ axl_win_TPropertyLine_SetButtonCheckState(this, State); }

	void ModifyFlags(int Modifier, bool_t Set)
		{ axl_win_TPropertyLine_ModifyFlags(this, Modifier, Set); }

	void SetFlags(int Flags)
		{ axl_win_TPropertyLine_SetFlags(this, Flags); }

	void SetDelimiters(int Delimiters)
		{ axl_win_TPropertyLine_SetDelimiters(this, Delimiters); }

	void Invalidate(int Mask = axl_win_EPropertyLineInvalidate_All)
		{ axl_win_TPropertyLine_Invalidate(this, Mask); }

	void ExpandCollapse(bool_t IsExpanded)
		{ axl_win_TPropertyLine_ExpandCollapse(this, IsExpanded); }
#endif
} axl_win_TPropertyLine;

//.............................................................................

typedef struct _axl_win_TPropertyHitTest
{
	int m_HitTest;
	axl_win_TPropertyLine* m_pLine;
	axl_gr_THyperlinkAnchor* m_pHyperlink;
} axl_win_TPropertyHitTest;

typedef struct _axl_win_TPropertyNotify
{
	NMHDR m_Hdr;
	axl_win_TPropertyHitTest m_HitTest;
} axl_win_TPropertyNotify;

//.............................................................................

// property ctrl

void
AXL_API
axl_win_TPropertyCtrl_Construct(axl_win_TPropertyCtrl* pPropertyCtrl);

void
AXL_API
axl_win_TPropertyCtrl_Destruct(axl_win_TPropertyCtrl* pPropertyCtrl);

bool_t
AXL_API
axl_win_TPropertyCtrl_RegisterWndClass();

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
	);

#define axl_win_TPropertyCtrl_Create(pPropertyCtrl, Style, pRect, hWndParent, ID, hInstance) \
	axl_win_TPropertyCtrl_CreateEx(pPropertyCtrl, 0, Style, pRect, hWndParent, ID, hInstance)

#define axl_win_TPropertyCtrl_FromHwnd(hWnd) \
	((axl_win_TPropertyCtrl*) (LONG_PTR) GetWindowLongPtr(hWnd, 0))

void
AXL_API
axl_win_TPropertyCtrl_Destroy(axl_win_TPropertyCtrl* pPropertyCtrl);

void
AXL_API
axl_win_TPropertyCtrl_SetSelection(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	size_t Line,
	bool_t SetInPlaceFocus
	);

void 
AXL_API
axl_win_TPropertyCtrl_AdvanceSelection(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	int Delta,
	bool_t SetInPlaceFocus
	);

void
AXL_API
axl_win_TPropertyCtrl_KillSelection(axl_win_TPropertyCtrl* pPropertyCtrl);

void 
AXL_API
axl_win_TPropertyCtrl_EnsureVisible(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	size_t Line
	);

#define axl_win_TPropertyCtrl_IsMenuActive(pPropertyCtrl) \
	(pPropertyCtrl->m_pMenuAnchorLine != NULL)

#define axl_win_TPropertyCtrl_IsMenu(pPropertyCtrl) \
	(pPropertyCtrl->m_pMenuRootPropertyCtrl != NULL)

#define axl_win_TPropertyCtrl_GetMenuRootPropertyCtrl(pPropertyCtrl) \
	((pPropertyCtrl)->m_pMenuRootPropertyCtrl ? (pPropertyCtrl)->m_pMenuRootPropertyCtrl : (pPropertyCtrl))

#define axl_win_TPropertyCtrl_GetFirstLine(pPropertyCtrl) \
	((axl_win_TPropertyLine*) axl_rtl_TList_GetHead(&pPropertyCtrl->m_LineList))

#define axl_win_TPropertyCtrl_GetLastLine(pPropertyCtrl) \
	((axl_win_TPropertyLine*) axl_rtl_TList_GetTail(&pPropertyCtrl->m_LineList))

axl_win_TPropertyLine*
AXL_API
axl_win_TPropertyCtrl_GetSelectedLine(axl_win_TPropertyCtrl* pPropertyCtrl);

bool_t
AXL_API
axl_win_TPropertyCtrl_SaveActiveInPlace(axl_win_TPropertyCtrl* pPropertyCtrl);

axl_win_TPropertyLine*
AXL_API
axl_win_TPropertyCtrl_InsertLineEx(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	axl_win_TPropertyLine* pParent,
	axl_win_TPropertyLine* pBefore,
	axl_win_TPropertyLine* pSource
	);

#define axl_win_TPropertyCtrl_InsertLine(pPropertyCtrl, pParent, pBefore) \
	axl_win_TPropertyCtrl_InsertLineEx(pPropertyCtrl, pParent, pBefore, NULL)

void
AXL_API
axl_win_TPropertyCtrl_Clear(axl_win_TPropertyCtrl* pPropertyCtrl);

void
AXL_API
axl_win_TPropertyCtrl_GetSize(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	SIZE* pSize
	);

void 
AXL_API
axl_win_TPropertyCtrl_UpdateLineWidth(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	axl_win_TPropertyLine* pLine
	);

void 
AXL_API
axl_win_TPropertyCtrl_UpdateAllLineWidths(axl_win_TPropertyCtrl* pPropertyCtrl);

bool_t
AXL_API
axl_win_TPropertyCtrl_Expand(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	axl_win_TPropertyLine* pLine
	);

bool_t
AXL_API
axl_win_TPropertyCtrl_Collapse(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	axl_win_TPropertyLine* pLine
	);

#define axl_win_TPropertyCtrl_ExpandCollapse(pPropertyCtrl, pLine, DoExpand) \
	((DoExpand) ? axl_win_TPropertyCtrl_Expand(pPropertyCtrl, pLine) : axl_win_TPropertyCtrl_Collapse(pPropertyCtrl, pLine))

axl_win_TPropertyLine*
AXL_API
axl_win_TPropertyCtrl_GetSelectedLine(axl_win_TPropertyCtrl* pPropertyCtrl);

long
AXL_API
axl_win_TPropertyCtrl_SetSplitterPos(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	long SplitterPos
	);

void 
AXL_API
axl_win_TPropertyCtrl_SetImageList(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	HIMAGELIST hImageList
	);

axl_win_TPropertyLine* 
AXL_API
axl_win_TPropertyCtrl_HitTest(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	long x,
	long y,	
	axl_win_TPropertyHitTest* pHitTest
	);

#ifdef __cplusplus

#define axl_win_TPropertyCtrl_SetFocus(pPropertyCtrl) \
	(::SetFocus((pPropertyCtrl)->m_hWnd))

#define axl_win_TPropertyCtrl_Invalidate(pPropertyCtrl) \
	(::InvalidateRect((pPropertyCtrl)->m_hWnd, NULL, FALSE))

#define axl_win_TPropertyCtrl_ClientToScreen(pPropertyCtrl, pPoint) \
	(::ClientToScreen((pPropertyCtrl)->m_hWnd, pPoint))

#else

#define axl_win_TPropertyCtrl_SetFocus(pPropertyCtrl) \
	SetFocus((pPropertyCtrl)->m_hWnd)

#define axl_win_TPropertyCtrl_Invalidate(pPropertyCtrl) \
	InvalidateRect((pPropertyCtrl)->m_hWnd, NULL, FALSE)

#define axl_win_TPropertyCtrl_ClientToScreen(pPropertyCtrl, pPoint) \
	ClientToScreen((pPropertyCtrl)->m_hWnd, pPoint)

#endif

HIMAGELIST 
AXL_API
axl_win_TPropertyCtrl_CreatePlusMinusImageList(
	ulong_t cx, 
	ulong_t cy,
	COLORREF* pPalette
	);

HIMAGELIST 
AXL_API
axl_win_TPropertyCtrl_CreateGridImageList(
	ulong_t cx, 
	ulong_t cy,
	COLORREF* pPalette
	);

HIMAGELIST 
AXL_API
axl_win_TPropertyCtrl_CreateMenuArrowImageList();

void 
AXL_API
axl_win_TPropertyCtrl_PaintRect(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	HDC hdc,
	const RECT* pRect
	);

void 
AXL_API
axl_win_TPropertyCtrl_InvalidateLineRange(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	size_t FromLine,
	size_t ToLine
	);

long
AXL_API
axl_win_TPropertyCtrl_GetFullWidth(axl_win_TPropertyCtrl* pPropertyCtrl);

long
AXL_API
axl_win_TPropertyCtrl_GetFullHeight(axl_win_TPropertyCtrl* pPropertyCtrl);

void 
AXL_API
axl_win_TPropertyCtrl_UpdateAllLineWidth(axl_win_TPropertyCtrl* pPropertyCtrl);

#define axl_win_TPropertyCtrl_SetNotify(pPropertyCtrl, hWndNotify, IdNotify) \
	((pPropertyCtrl)->m_hWndNotify = (hWndNotify), (pPropertyCtrl)->m_IdNotify = (IdNotify))

LRESULT
AXL_API
axl_win_TPropertyCtrl_Notify(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	int Code,
	axl_win_TPropertyHitTest* pHitTest
	);

void
AXL_API
axl_win_TPropertyCtrl_OnScroll(
	axl_win_TPropertyCtrl* pPropertyCtrl, 
	int Bar,
	int Code
	);

void
AXL_API
axl_win_TPropertyCtrl_OnKeyDown(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	int VkCode
	);

void
AXL_API
axl_win_TPropertyCtrl_OnKeyUp(
	axl_win_TPropertyCtrl* pPropertyCtrl,
	int VkCode
	);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

typedef struct _axl_win_TPropertyCtrl 
{
	HWND m_hWnd;
	HWND m_hWndToolTip;
	HWND m_hWndNotify;
	UINT m_IdNotify;

	axl_win_TPropertyCtrl* m_pMenuPropertyCtrl;
	axl_win_TPropertyCtrl* m_pMenuParentPropertyCtrl;
	axl_win_TPropertyCtrl* m_pMenuRootPropertyCtrl;
	axl_win_TPropertyLine* m_pMenuAnchorLine;
	DWORD m_MenuDelay;

	axl_gr_TTextPaint m_TextPaint;
	axl_gr_TStockCtrlPaint m_StockCtrlPaint;
	axl_gr_TOffscreenBuffer m_OffscreenBuffer;

//	HRGN m_hUpdateRgn;

	HIMAGELIST m_hImageList;
	HIMAGELIST m_hPlusMinusImageList;
	HIMAGELIST m_hGridImageList;
	HIMAGELIST m_hMenuArrowImageList;
	axl_gr_TFont m_DefaultFont;
	HCURSOR m_hArrowCursor;
	HCURSOR m_hSplitterCursor;
	HCURSOR m_hHyperlinkCursor;
	HCURSOR m_hReArrangeCursor;
	RECT m_SplitterProximityRect;
	RECT m_Margins;
	SIZE m_CharSize;
	SIZE m_PlusMinusSize;
	SIZE m_OptionBoxSize;
	SIZE m_IconSize;
	RECT m_PlusMinusRect;
	RECT m_OptionBoxRect;
	RECT m_IconRect;
	POINT m_TextOrigin;

	DWORD m_ToolTipShowDelay;
	DWORD m_ToolTipReShowDelay;

	int m_Drag;
	int m_DragAnchorHitTest;
	axl_win_TPropertyLine* m_pDragAnchorLine;
	axl_gr_THyperlinkAnchor* m_pDragAnchorHyperlink;
	POINT m_DragDelta;

	int m_Hover;
	axl_win_TPropertyLine* m_pHoverAnchorLine;
	axl_win_TPropertyLine* m_pLastTooltipLine;

	size_t m_FirstVisibleLine;
	size_t m_FullyVisibleLineCount;
	size_t m_VisibleLineCount;
	size_t m_SelectedLine;
	long m_SplitterPos;
	long m_FirstVisibleX;
	ulong_t m_VisibleWidth;
	ulong_t m_IndentWidth;
	ulong_t m_OptionBoxWidth;
	ulong_t m_IconWidth;
	ulong_t m_LineHeight;
	ulong_t m_MinNamePaneWidth;
	ulong_t m_SinglePaneWidth;
	ulong_t m_ValueWidth;

	bool_t m_HasGridLinesAtRoot;
	bool_t m_DoAlwaysDrawButtons;

	ulong_t m_MouseWheelSpeed;

	axl_rtl_TList m_LineList;
	axl_rtl_TList* m_pLineList;
	axl_rtl_TPtrArray m_VisibleLineArray;
	axl_win_TPropertyInPlace* m_pActiveInPlace;
	ulong_t m_ActiveInPlaceMinWidth;

	COLORREF m_Palette[axl_win_EPropertyColor_Count];

#ifdef __cplusplus
	operator HWND () 
		{ return m_hWnd; }

	static axl_TStatus RegisterWndClass()
		{ return axl_win_TPropertyCtrl_RegisterWndClass(); }

	HWND CreateEx(DWORD ExStyle, DWORD Style, const RECT* pRect, HWND hWndParent, UINT ID, HINSTANCE hInstance)
		{ return axl_win_TPropertyCtrl_CreateEx(this, ExStyle, Style, pRect, hWndParent, ID, hInstance); }

	HWND Create(DWORD Style, const RECT* pRect, HWND hWndParent, UINT ID, HINSTANCE hInstance)
		{ return axl_win_TPropertyCtrl_Create(this, Style, pRect, hWndParent, ID, hInstance); }

	void Destroy()
		{ axl_win_TPropertyCtrl_Destroy(this); }

	static axl_win_TPropertyCtrl* FromHwnd(HWND hWnd)
		{ return axl_win_TPropertyCtrl_FromHwnd(hWnd); }

	void Clear()
		{ axl_win_TPropertyCtrl_Clear(this); }

	int GetFullWidth()
		{ return axl_win_TPropertyCtrl_GetFullWidth(this); }

	int GetFullHeight()
		{ return axl_win_TPropertyCtrl_GetFullHeight(this); }

	void GetSize(SIZE* pSize)
		{ axl_win_TPropertyCtrl_GetSize(this, pSize); }

	SIZE GetSize()
	{ 
		SIZE Size;
		axl_win_TPropertyCtrl_GetSize(this, &Size); 
		return Size;
	}

	void SetImageList(HIMAGELIST hImageList)
		{ axl_win_TPropertyCtrl_SetImageList(this, hImageList); }

	void SetFocus()
		{ axl_win_TPropertyCtrl_SetFocus(this); }

	void Invalidate()
		{ axl_win_TPropertyCtrl_Invalidate(this); }

	void ClientToScreen(POINT* pPoint)
		{ axl_win_TPropertyCtrl_ClientToScreen(this, pPoint); }

	axl_win_TPropertyLine* GetFirstLine()
		{ return axl_win_TPropertyCtrl_GetFirstLine(this); }

	axl_win_TPropertyLine* GetLastLine()
		{ return axl_win_TPropertyCtrl_GetLastLine(this); }

	axl_win_TPropertyLine* GetSelectedLine()
		{ return axl_win_TPropertyCtrl_GetSelectedLine(this); }

	bool SaveActiveInPlace()
		{ return axl_win_TPropertyCtrl_SaveActiveInPlace(this) != 0; }

	axl_win_TPropertyLine* InsertLine(axl_win_TPropertyLine* pParent = NULL, axl_win_TPropertyLine* pBefore = NULL)
		{ return axl_win_TPropertyCtrl_InsertLine(this, pParent, pBefore); }

	void UpdateLineWidth(axl_win_TPropertyLine* pLine)
		{ axl_win_TPropertyCtrl_UpdateLineWidth(this, pLine); }

	void UpdateAllLineWidths()
		{ axl_win_TPropertyCtrl_UpdateAllLineWidths(this); }

	long SetSplitterPos(long SplitterPos)
		{ return axl_win_TPropertyCtrl_SetSplitterPos(this, SplitterPos); }

#endif
} axl_win_TPropertyCtrl;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//.............................................................................

#ifdef __cplusplus
} // extern "C"

namespace axl {
namespace win {

struct EPropertyLine { enum
{
	SinglePane           = axl_win_EPropertyLine_SinglePane,
	ReArrangeable        = axl_win_EPropertyLine_ReArrangeable,
	IsExpanded           = axl_win_EPropertyLine_IsExpanded,
	IsDisabled           = axl_win_EPropertyLine_IsDisabled,
	NotSelectable        = axl_win_EPropertyLine_NotSelectable,
	Button               = axl_win_EPropertyLine_Button,
	NameHyperText        = axl_win_EPropertyLine_NameHyperText,
	ValueHyperText       = axl_win_EPropertyLine_ValueHyperText,
	Menu                 = axl_win_EPropertyLine_Menu,
	NoChildGridLines     = axl_win_EPropertyLine_NoChildGridLines,
	ForceNoGridLines     = axl_win_EPropertyLine_ForceNoGridLines,
	AlwaysDrawButton     = axl_win_EPropertyLine_AlwaysDrawButton,
	NameShadow           = axl_win_EPropertyLine_NameShadow,
	ValueShadow          = axl_win_EPropertyLine_ValueShadow,
	Separator            = axl_win_EPropertyLine_Separator
};};

struct EPropertyLineDelimiter { enum
{
	None        = axl_win_EPropertyLineDelimiter_None,
	NameTop     = axl_win_EPropertyLineDelimiter_NameTop,
	NameBottom  = axl_win_EPropertyLineDelimiter_NameBottom,
	ValueTop    = axl_win_EPropertyLineDelimiter_ValueTop,
	ValueBottom = axl_win_EPropertyLineDelimiter_ValueBottom,
	All         = axl_win_EPropertyLineDelimiter_All,

	Top         = axl_win_EPropertyLineDelimiter_Top,
	Bottom      = axl_win_EPropertyLineDelimiter_Bottom,
	Name        = axl_win_EPropertyLineDelimiter_Name,
	Value       = axl_win_EPropertyLineDelimiter_Value,
};};

struct EPropertyOptionBoxType { enum
{
	None        = axl_win_EPropertyOptionBoxType_None,
	CheckBox    = axl_win_EPropertyOptionBoxType_CheckBox,
	RadioButton = axl_win_EPropertyOptionBoxType_RadioButton,
};};

struct EPropertyCheckState { enum
{
	Unchecked    = axl_win_EPropertyCheckState_Unchecked,
	Checked      = axl_win_EPropertyCheckState_Checked,
	State3       = axl_win_EPropertyCheckState_State3,
};};

struct EPropertyHitTest { enum
{
	None      = axl_win_EPropertyHitTest_None,
	Indent    = axl_win_EPropertyHitTest_Indent,
	PlusMinus = axl_win_EPropertyHitTest_PlusMinus,
	OptionBox = axl_win_EPropertyHitTest_OptionBox,
	Button    = axl_win_EPropertyHitTest_Button,
	Menu      = axl_win_EPropertyHitTest_Menu,
	Icon      = axl_win_EPropertyHitTest_Icon,
	Name      = axl_win_EPropertyHitTest_Name,
	NameTail  = axl_win_EPropertyHitTest_NameTail,
	Splitter  = axl_win_EPropertyHitTest_Splitter,
	Value     = axl_win_EPropertyHitTest_Value,
	ValueTail = axl_win_EPropertyHitTest_ValueTail,
	Hyperlink = axl_win_EPropertyHitTest_Hyperlink,
};};

struct EPropertyNotify { enum
{
	ButtonClick    = axl_win_EPropertyNotify_ButtonClick,
	OptionBoxClick = axl_win_EPropertyNotify_OptionBoxClick,
	HyperlinkClick = axl_win_EPropertyNotify_HyperlinkClick,
	MenuClick      = axl_win_EPropertyNotify_MenuClick,
	ReArrange      = axl_win_EPropertyNotify_ReArrange,
	ExpandCollapse = axl_win_EPropertyNotify_ExpandCollapse,
};};

typedef axl_win_TPropertyCtrl    TPropertyCtrl;
typedef axl_win_TPropertyLine    TPropertyLine;
typedef axl_win_TPropertyInPlace TPropertyInPlace;
typedef axl_win_TPropertyHitTest TPropertyHitTest;
typedef axl_win_TPropertyNotify  TPropertyNotify;

class CPropertyCtrl: public TPropertyCtrl
{
public:
	CPropertyCtrl()
		{ axl_win_TPropertyCtrl_Construct(this); }

	~CPropertyCtrl()
		{ axl_win_TPropertyCtrl_Destruct(this); }
};

class CPropertyInPlace: public TPropertyInPlace
{
public:
	CPropertyInPlace(axl_obj_TAgent* pAgent)
		{ axl_win_TPropertyInPlace_Construct(this, pAgent); }
};

template <class T>
class CPropertyInPlaceT: public CPropertyInPlace
{
public:
	typedef CPropertyInPlaceT CBaseClass;

public:
	CPropertyInPlaceT(axl::obj::TAgent* pAgent):
		CPropertyInPlace(pAgent)
	{ 
		m_pfLoad = _Load;
		m_pfSave = _Save;
		m_pfLayout = _Layout;
		m_pfSetFocus = _SetFocus;
	}

protected:
	static bool_t AXL_API _Load(axl_win_TPropertyInPlace* pInPlace, axl_win_TPropertyLine* pLine, const RECT* pRect, ulong_t* pMinWidth)
		{ return static_cast<T*>(pInPlace)->Load(pLine, pRect, pMinWidth); }

	static bool_t AXL_API _Save(axl_win_TPropertyInPlace* pInPlace, axl_win_TPropertyLine* pLine)
		{ return static_cast<T*>(pInPlace)->Save(pLine); }

	static void AXL_API _Layout(axl_win_TPropertyInPlace* pInPlace, const RECT* pRect)
		{ static_cast<T*>(pInPlace)->Layout(pRect); }

	static void AXL_API _SetFocus(axl_win_TPropertyInPlace* pInPlace)
		{ static_cast<T*>(pInPlace)->SetFocus(); }
};

} // namespace win {
} // namespace axl {

#endif

//.............................................................................

