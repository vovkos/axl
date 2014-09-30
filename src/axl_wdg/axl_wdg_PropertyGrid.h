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
	axl_win_TPropertyInPlace* inPlace,
	axl_win_TPropertyLine* line,
	const RECT* rect,
	ulong_t* minWidth
	);

typedef
bool_t
(AXL_API*
 axl_win_FPropertyInPlaceSave)(
	axl_win_TPropertyInPlace* inPlace,
	axl_win_TPropertyLine* line
	);

typedef
void
(AXL_API*
 axl_win_FPropertyInPlaceLayout)(
	axl_win_TPropertyInPlace* inPlace,
	const RECT* rect
	);

typedef
void
(AXL_API*
 axl_win_FPropertyInPlaceSetFocus)(axl_win_TPropertyInPlace* inPlace);

//.............................................................................

void
AXL_API
axl_win_TPropertyInPlace_Construct(
	axl_win_TPropertyInPlace* inPlace,
	axl_obj_TAgent* agent
	);

#define axl_win_TPropertyInPlace_Load(inPlace, line, rect, minWidth) \
	((inPlace)->m_pfLoad(inPlace, line, rect, minWidth))

#define axl_win_TPropertyInPlace_Save(inPlace, line) \
	((inPlace)->m_pfSave(inPlace, line))

#define axl_win_TPropertyInPlace_Layout(inPlace, rect) \
	((inPlace)->m_pfLayout(inPlace, rect))

#define axl_win_TPropertyInPlace_SetFocus(inPlace) \
	((inPlace)->m_pfSetFocus(inPlace))

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
axl_win_TPropertyLine_Construct(axl_win_TPropertyLine* line);

void 
AXL_API
axl_win_TPropertyLine_Destruct(axl_win_TPropertyLine* line);

void 
AXL_API
axl_win_TPropertyLine_ClearLineList(axl_rtl_TList* lineList);

bool_t 
AXL_API
axl_win_TPropertyLine_Clone(
	axl_win_TPropertyLine* line,
	axl_win_TPropertyLine* src
	);

long 
AXL_API
axl_win_TPropertyLine_GetPartX(
	axl_win_TPropertyLine* line,
	int part
	);

bool_t
AXL_API
axl_win_TPropertyLine_IsPartFullyVisible(
	axl_win_TPropertyLine* line,
	int part
	);

bool_t
AXL_API
axl_win_TPropertyLine_CalcWidth(
	axl_win_TPropertyLine* line,
	int flags
	);

void
AXL_API
axl_win_TPropertyLine_ModifyChildrenHeight(
	axl_win_TPropertyLine* line,
	long delta	
	);

void 
AXL_API
axl_win_TPropertyLine_SetOptionBoxVolatileState(
	axl_win_TPropertyLine* line,
	int state
	);

void 
AXL_API
axl_win_TPropertyLine_SetButtonVolatileState(
	axl_win_TPropertyLine* line,
	int state
	);

bool_t
AXL_API
axl_win_TPropertyLine_IsLastChild(axl_win_TPropertyLine* line);
 
bool_t
AXL_API
axl_win_TPropertyLine_HasGridLines(axl_win_TPropertyLine* line);

bool_t
AXL_API
axl_win_TPropertyLine_HasNameDelimiter(axl_win_TPropertyLine* line);

bool_t
AXL_API
axl_win_TPropertyLine_HasValueDelimiter(axl_win_TPropertyLine* line);

long 
AXL_API
axl_win_TPropertyLine_GetPartWidth(
	axl_win_TPropertyLine* line,
	int part
	);

#define axl_win_TPropertyLine_HasPlusMinus(line) \
	(!axl_rtl_TList_IsEmpty(&(line)->m_childrenList) && !((line)->m_flags & axl_win_EPropertyLine_Menu))

#define axl_win_TPropertyLine_IsSinglePane(line) \
	(((line)->m_flags & (axl_win_EPropertyLine_SinglePane | axl_win_EPropertyLine_Menu)) != 0)

#define axl_win_TPropertyLine_GetFirstChild(line) \
	((axl_win_TPropertyLine*) axl_rtl_TList_GetHead(&line->m_childrenList))

#define axl_win_TPropertyLine_GetLastChild(line) \
	((axl_win_TPropertyLine*) axl_rtl_TList_GetTail(&line->m_childrenList))

axl_win_TPropertyLine* 
AXL_API
axl_win_TPropertyLine_GetNextSibling(axl_win_TPropertyLine* line);

axl_win_TPropertyLine* 
AXL_API
axl_win_TPropertyLine_GetPrevSibling(axl_win_TPropertyLine* line);

#define axl_win_TPropertyLine_SetInPlace(line, inPlace) \
	axl_obj_TObject_CopyPtr(&line->m_inPlace, inPlace)

#define axl_win_TPropertyLine_IsReArrangeable(line) \
	((line)->m_parent && ((line)->m_parent->m_flags & axl_win_EPropertyLine_ReArrangeable))

#define axl_win_TPropertyLine_IsButton(line) \
	((line->m_flags & axl_win_EPropertyLine_Button) || \
	((line->m_flags & axl_win_EPropertyLine_Menu) && !line->m_propertyCtrl->m_menuParentPropertyCtrl))

void 
AXL_API
axl_win_TPropertyLine_SetName(
	axl_win_TPropertyLine* line,
	const tchar_t* string
	);

void 
AXL_API
axl_win_TPropertyLine_SetValue(
	axl_win_TPropertyLine* line,
	const tchar_t* string
	);

void 
AXL_API
axl_win_TPropertyLine_SetNameAttr(
	axl_win_TPropertyLine* line,
	axl_gr_TTextAttr* attr
	);

void 
AXL_API
axl_win_TPropertyLine_SetValueAttr(
	axl_win_TPropertyLine* line,
	axl_gr_TTextAttr* attr
	);

void 
AXL_API
axl_win_TPropertyLine_SetIcon(
	axl_win_TPropertyLine* line,
	int icon
	);

void 
AXL_API
axl_win_TPropertyLine_SetIndent(
	axl_win_TPropertyLine* line,
	int indent
	);

void 
AXL_API
axl_win_TPropertyLine_SetOptionBoxType(
	axl_win_TPropertyLine* line,
	int type
	);

void 
AXL_API
axl_win_TPropertyLine_SetOptionBoxCheckState(
	axl_win_TPropertyLine* line,
	int state
	);

void 
AXL_API
axl_win_TPropertyLine_SetButtonCheckState(
	axl_win_TPropertyLine* line,
	int state
	);

void 
AXL_API
axl_win_TPropertyLine_SetFlags(
	axl_win_TPropertyLine* line,
	int flags
	);

void 
AXL_API
axl_win_TPropertyLine_SetDelimiters(
	axl_win_TPropertyLine* line,
	int delimiters
	);

void 
AXL_API
axl_win_TPropertyLine_ModifyFlags(
	axl_win_TPropertyLine* line,
	int modifier,
	bool_t set
	);

void 
AXL_API
axl_win_TPropertyLine_Invalidate(
	axl_win_TPropertyLine* line,
	ulong_t mask
	);

void 
AXL_API
axl_win_TPropertyLine_ExpandCollapse(
	axl_win_TPropertyLine* line,
	bool_t isExpanded
	);

#ifdef __cplusplus
typedef struct _axl_win_TPropertyLine: axl_rtl_TListEntry {
#else
typedef struct _axl_win_TPropertyLine { axl_rtl_TListEntry;
#endif	
	int m_flags;
	int m_delimiters;
	int m_optionBoxType;
	int m_optionBoxCheckState;
	int m_optionBoxVolatileState;
	int m_buttonVolatileState;
	int m_buttonCheckState;
	int m_icon;
	int m_toolTipIcon;
	int m_level;
	int m_indent;
	size_t m_visibleIndex;
	size_t m_childrenHeight;
	ulong_t m_menuWidth;
	ulong_t m_singlePaneWidth;
	ulong_t m_nameWidth;
	ulong_t m_valueWidth;
	ulong_t m_childrenSinglePaneWidth;
	ulong_t m_childrenValueWidth;
	axl_win_TPropertyCtrl* m_propertyCtrl;
	axl_win_TPropertyLine* m_parent;
	axl_rtl_TList m_childrenList;
	axl_rtl_TString m_name;
	axl_rtl_TString m_value;
	axl_rtl_TString m_toolTipTitle;
	axl_rtl_TString m_toolTipText;
	axl_gr_TTextAttr m_nameAttr;
	axl_gr_TTextAttr m_valueAttr;
	axl_gr_THyperText m_nameHyperText;
	axl_gr_THyperText m_valueHyperText;
	axl_win_TPropertyInPlace* m_inPlace;
	axl_obj_TData m_userData;

#ifdef __cplusplus
	axl_win_TPropertyLine* getFirstChild()
		{ return axl_win_TPropertyLine_GetFirstChild(this); }

	axl_win_TPropertyLine* getLastChild()
		{ return axl_win_TPropertyLine_GetLastChild(this); }

	axl_win_TPropertyLine* getNextSibling()
		{ return axl_win_TPropertyLine_GetNextSibling(this); }

	axl_win_TPropertyLine* getPrevSibling()
		{ return axl_win_TPropertyLine_GetPrevSibling(this); }

	void setInPlace(axl_win_TPropertyInPlace* inPlace)
		{ axl_win_TPropertyLine_SetInPlace(this, inPlace); }

	void setName(const tchar_t* string)
		{ axl_win_TPropertyLine_SetName(this, string); }

	void setValue(const tchar_t* string)
		{ axl_win_TPropertyLine_SetValue(this, string); }

	void setNameAttr(axl_gr_TTextAttr* attr)
		{ axl_win_TPropertyLine_SetNameAttr(this, attr); }

	void setValueAttr(axl_gr_TTextAttr* attr)
		{ axl_win_TPropertyLine_SetValueAttr(this, attr); }

	void setIcon(int icon)
		{ axl_win_TPropertyLine_SetIcon(this, icon); }

	void setIndent(int indent)
		{ axl_win_TPropertyLine_SetIndent(this, indent); }

	void setOptionBoxType(int type)
		{ axl_win_TPropertyLine_SetOptionBoxType(this, type); }

	void setOptionBoxCheckState(int state)
		{ axl_win_TPropertyLine_SetOptionBoxCheckState(this, state); }

	void setButtonCheckState(int state)
		{ axl_win_TPropertyLine_SetButtonCheckState(this, state); }

	void modifyFlags(int modifier, bool_t set)
		{ axl_win_TPropertyLine_ModifyFlags(this, modifier, set); }

	void setFlags(int flags)
		{ axl_win_TPropertyLine_SetFlags(this, flags); }

	void setDelimiters(int delimiters)
		{ axl_win_TPropertyLine_SetDelimiters(this, delimiters); }

	void invalidate(int mask = axl_win_EPropertyLineInvalidate_All)
		{ axl_win_TPropertyLine_Invalidate(this, mask); }

	void expandCollapse(bool_t isExpanded)
		{ axl_win_TPropertyLine_ExpandCollapse(this, isExpanded); }
#endif
} axl_win_TPropertyLine;

//.............................................................................

typedef struct _axl_win_TPropertyHitTest
{
	int m_hitTest;
	axl_win_TPropertyLine* m_line;
	axl_gr_THyperlinkAnchor* m_hyperlink;
} axl_win_TPropertyHitTest;

typedef struct _axl_win_TPropertyNotify
{
	NMHDR m_hdr;
	axl_win_TPropertyHitTest m_hitTest;
} axl_win_TPropertyNotify;

//.............................................................................

// property ctrl

void
AXL_API
axl_win_TPropertyCtrl_Construct(axl_win_TPropertyCtrl* propertyCtrl);

void
AXL_API
axl_win_TPropertyCtrl_Destruct(axl_win_TPropertyCtrl* propertyCtrl);

bool_t
AXL_API
axl_win_TPropertyCtrl_RegisterWndClass();

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
	);

#define axl_win_TPropertyCtrl_Create(propertyCtrl, style, rect, hWndParent, ID, hInstance) \
	axl_win_TPropertyCtrl_CreateEx(propertyCtrl, 0, style, rect, hWndParent, ID, hInstance)

#define axl_win_TPropertyCtrl_FromHwnd(hWnd) \
	((axl_win_TPropertyCtrl*) (LONG_PTR) getWindowLongPtr(hWnd, 0))

void
AXL_API
axl_win_TPropertyCtrl_Destroy(axl_win_TPropertyCtrl* propertyCtrl);

void
AXL_API
axl_win_TPropertyCtrl_SetSelection(
	axl_win_TPropertyCtrl* propertyCtrl,
	size_t line,
	bool_t setInPlaceFocus
	);

void 
AXL_API
axl_win_TPropertyCtrl_AdvanceSelection(
	axl_win_TPropertyCtrl* propertyCtrl,
	int delta,
	bool_t setInPlaceFocus
	);

void
AXL_API
axl_win_TPropertyCtrl_KillSelection(axl_win_TPropertyCtrl* propertyCtrl);

void 
AXL_API
axl_win_TPropertyCtrl_EnsureVisible(
	axl_win_TPropertyCtrl* propertyCtrl,
	size_t line
	);

#define axl_win_TPropertyCtrl_IsMenuActive(propertyCtrl) \
	(propertyCtrl->m_menuAnchorLine != NULL)

#define axl_win_TPropertyCtrl_IsMenu(propertyCtrl) \
	(propertyCtrl->m_menuRootPropertyCtrl != NULL)

#define axl_win_TPropertyCtrl_GetMenuRootPropertyCtrl(propertyCtrl) \
	((propertyCtrl)->m_menuRootPropertyCtrl ? (propertyCtrl)->m_menuRootPropertyCtrl : (propertyCtrl))

#define axl_win_TPropertyCtrl_GetFirstLine(propertyCtrl) \
	((axl_win_TPropertyLine*) axl_rtl_TList_GetHead(&propertyCtrl->m_lineList))

#define axl_win_TPropertyCtrl_GetLastLine(propertyCtrl) \
	((axl_win_TPropertyLine*) axl_rtl_TList_GetTail(&propertyCtrl->m_lineList))

axl_win_TPropertyLine*
AXL_API
axl_win_TPropertyCtrl_GetSelectedLine(axl_win_TPropertyCtrl* propertyCtrl);

bool_t
AXL_API
axl_win_TPropertyCtrl_SaveActiveInPlace(axl_win_TPropertyCtrl* propertyCtrl);

axl_win_TPropertyLine*
AXL_API
axl_win_TPropertyCtrl_InsertLineEx(
	axl_win_TPropertyCtrl* propertyCtrl,
	axl_win_TPropertyLine* parent,
	axl_win_TPropertyLine* before,
	axl_win_TPropertyLine* source
	);

#define axl_win_TPropertyCtrl_InsertLine(propertyCtrl, parent, before) \
	axl_win_TPropertyCtrl_InsertLineEx(propertyCtrl, parent, before, NULL)

void
AXL_API
axl_win_TPropertyCtrl_Clear(axl_win_TPropertyCtrl* propertyCtrl);

void
AXL_API
axl_win_TPropertyCtrl_GetSize(
	axl_win_TPropertyCtrl* propertyCtrl,
	SIZE* size
	);

void 
AXL_API
axl_win_TPropertyCtrl_UpdateLineWidth(
	axl_win_TPropertyCtrl* propertyCtrl,
	axl_win_TPropertyLine* line
	);

void 
AXL_API
axl_win_TPropertyCtrl_UpdateAllLineWidths(axl_win_TPropertyCtrl* propertyCtrl);

bool_t
AXL_API
axl_win_TPropertyCtrl_Expand(
	axl_win_TPropertyCtrl* propertyCtrl,
	axl_win_TPropertyLine* line
	);

bool_t
AXL_API
axl_win_TPropertyCtrl_Collapse(
	axl_win_TPropertyCtrl* propertyCtrl,
	axl_win_TPropertyLine* line
	);

#define axl_win_TPropertyCtrl_ExpandCollapse(propertyCtrl, line, doExpand) \
	((doExpand) ? axl_win_TPropertyCtrl_Expand(propertyCtrl, line) : axl_win_TPropertyCtrl_Collapse(propertyCtrl, line))

axl_win_TPropertyLine*
AXL_API
axl_win_TPropertyCtrl_GetSelectedLine(axl_win_TPropertyCtrl* propertyCtrl);

long
AXL_API
axl_win_TPropertyCtrl_SetSplitterPos(
	axl_win_TPropertyCtrl* propertyCtrl,
	long splitterPos
	);

void 
AXL_API
axl_win_TPropertyCtrl_SetImageList(
	axl_win_TPropertyCtrl* propertyCtrl,
	HIMAGELIST hImageList
	);

axl_win_TPropertyLine* 
AXL_API
axl_win_TPropertyCtrl_HitTest(
	axl_win_TPropertyCtrl* propertyCtrl,
	long x,
	long y,	
	axl_win_TPropertyHitTest* hitTest
	);

#ifdef __cplusplus

#define axl_win_TPropertyCtrl_SetFocus(propertyCtrl) \
	(::SetFocus((propertyCtrl)->m_hWnd))

#define axl_win_TPropertyCtrl_Invalidate(propertyCtrl) \
	(::InvalidateRect((propertyCtrl)->m_hWnd, NULL, FALSE))

#define axl_win_TPropertyCtrl_ClientToScreen(propertyCtrl, point) \
	(::ClientToScreen((propertyCtrl)->m_hWnd, point))

#else

#define axl_win_TPropertyCtrl_SetFocus(propertyCtrl) \
	setFocus((propertyCtrl)->m_hWnd)

#define axl_win_TPropertyCtrl_Invalidate(propertyCtrl) \
	invalidateRect((propertyCtrl)->m_hWnd, NULL, FALSE)

#define axl_win_TPropertyCtrl_ClientToScreen(propertyCtrl, point) \
	clientToScreen((propertyCtrl)->m_hWnd, point)

#endif

HIMAGELIST 
AXL_API
axl_win_TPropertyCtrl_CreatePlusMinusImageList(
	ulong_t cx, 
	ulong_t cy,
	COLORREF* palette
	);

HIMAGELIST 
AXL_API
axl_win_TPropertyCtrl_CreateGridImageList(
	ulong_t cx, 
	ulong_t cy,
	COLORREF* palette
	);

HIMAGELIST 
AXL_API
axl_win_TPropertyCtrl_CreateMenuArrowImageList();

void 
AXL_API
axl_win_TPropertyCtrl_PaintRect(
	axl_win_TPropertyCtrl* propertyCtrl,
	HDC hdc,
	const RECT* rect
	);

void 
AXL_API
axl_win_TPropertyCtrl_InvalidateLineRange(
	axl_win_TPropertyCtrl* propertyCtrl,
	size_t fromLine,
	size_t toLine
	);

long
AXL_API
axl_win_TPropertyCtrl_GetFullWidth(axl_win_TPropertyCtrl* propertyCtrl);

long
AXL_API
axl_win_TPropertyCtrl_GetFullHeight(axl_win_TPropertyCtrl* propertyCtrl);

void 
AXL_API
axl_win_TPropertyCtrl_UpdateAllLineWidth(axl_win_TPropertyCtrl* propertyCtrl);

#define axl_win_TPropertyCtrl_SetNotify(propertyCtrl, hWndNotify, idNotify) \
	((propertyCtrl)->m_hWndNotify = (hWndNotify), (propertyCtrl)->m_idNotify = (idNotify))

LRESULT
AXL_API
axl_win_TPropertyCtrl_Notify(
	axl_win_TPropertyCtrl* propertyCtrl,
	int code,
	axl_win_TPropertyHitTest* hitTest
	);

void
AXL_API
axl_win_TPropertyCtrl_OnScroll(
	axl_win_TPropertyCtrl* propertyCtrl, 
	int bar,
	int code
	);

void
AXL_API
axl_win_TPropertyCtrl_OnKeyDown(
	axl_win_TPropertyCtrl* propertyCtrl,
	int vkCode
	);

void
AXL_API
axl_win_TPropertyCtrl_OnKeyUp(
	axl_win_TPropertyCtrl* propertyCtrl,
	int vkCode
	);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

typedef struct _axl_win_TPropertyCtrl 
{
	HWND m_hWnd;
	HWND m_hWndToolTip;
	HWND m_hWndNotify;
	UINT m_idNotify;

	axl_win_TPropertyCtrl* m_menuPropertyCtrl;
	axl_win_TPropertyCtrl* m_menuParentPropertyCtrl;
	axl_win_TPropertyCtrl* m_menuRootPropertyCtrl;
	axl_win_TPropertyLine* m_menuAnchorLine;
	DWORD m_menuDelay;

	axl_gr_TTextPaint m_textPaint;
	axl_gr_TStockCtrlPaint m_stockCtrlPaint;
	axl_gr_TOffscreenBuffer m_offscreenBuffer;

//	HRGN m_hUpdateRgn;

	HIMAGELIST m_hImageList;
	HIMAGELIST m_hPlusMinusImageList;
	HIMAGELIST m_hGridImageList;
	HIMAGELIST m_hMenuArrowImageList;
	axl_gr_TFont m_defaultFont;
	HCURSOR m_hArrowCursor;
	HCURSOR m_hSplitterCursor;
	HCURSOR m_hHyperlinkCursor;
	HCURSOR m_hReArrangeCursor;
	RECT m_splitterProximityRect;
	RECT m_margins;
	SIZE m_charSize;
	SIZE m_plusMinusSize;
	SIZE m_optionBoxSize;
	SIZE m_iconSize;
	RECT m_plusMinusRect;
	RECT m_optionBoxRect;
	RECT m_iconRect;
	POINT m_textOrigin;

	DWORD m_toolTipShowDelay;
	DWORD m_toolTipReShowDelay;

	int m_drag;
	int m_dragAnchorHitTest;
	axl_win_TPropertyLine* m_dragAnchorLine;
	axl_gr_THyperlinkAnchor* m_dragAnchorHyperlink;
	POINT m_dragDelta;

	int m_hover;
	axl_win_TPropertyLine* m_hoverAnchorLine;
	axl_win_TPropertyLine* m_lastTooltipLine;

	size_t m_firstVisibleLine;
	size_t m_fullyVisibleLineCount;
	size_t m_visibleLineCount;
	size_t m_selectedLine;
	long m_splitterPos;
	long m_firstVisibleX;
	ulong_t m_visibleWidth;
	ulong_t m_indentWidth;
	ulong_t m_optionBoxWidth;
	ulong_t m_iconWidth;
	ulong_t m_lineHeight;
	ulong_t m_minNamePaneWidth;
	ulong_t m_singlePaneWidth;
	ulong_t m_valueWidth;

	bool_t m_hasGridLinesAtRoot;
	bool_t m_doAlwaysDrawButtons;

	ulong_t m_mouseWheelSpeed;

	axl_rtl_TList m_lineList;
	axl_rtl_TList* m_lineList;
	axl_rtl_TPtrArray m_visibleLineArray;
	axl_win_TPropertyInPlace* m_activeInPlace;
	ulong_t m_activeInPlaceMinWidth;

	COLORREF m_palette[axl_win_EPropertyColor_Count];

#ifdef __cplusplus
	operator HWND () 
		{ return m_hWnd; }

	static axl_TStatus registerWndClass()
		{ return axl_win_TPropertyCtrl_RegisterWndClass(); }

	HWND createEx(DWORD exStyle, DWORD style, const RECT* rect, HWND hWndParent, UINT ID, HINSTANCE hInstance)
		{ return axl_win_TPropertyCtrl_CreateEx(this, exStyle, style, rect, hWndParent, ID, hInstance); }

	HWND create(DWORD style, const RECT* rect, HWND hWndParent, UINT ID, HINSTANCE hInstance)
		{ return axl_win_TPropertyCtrl_Create(this, style, rect, hWndParent, ID, hInstance); }

	void destroy()
		{ axl_win_TPropertyCtrl_Destroy(this); }

	static axl_win_TPropertyCtrl* fromHwnd(HWND hWnd)
		{ return axl_win_TPropertyCtrl_FromHwnd(hWnd); }

	void clear()
		{ axl_win_TPropertyCtrl_Clear(this); }

	int getFullWidth()
		{ return axl_win_TPropertyCtrl_GetFullWidth(this); }

	int getFullHeight()
		{ return axl_win_TPropertyCtrl_GetFullHeight(this); }

	void getSize(SIZE* size)
		{ axl_win_TPropertyCtrl_GetSize(this, size); }

	SIZE getSize()
	{ 
		SIZE size;
		axl_win_TPropertyCtrl_GetSize(this, &size); 
		return size;
	}

	void setImageList(HIMAGELIST hImageList)
		{ axl_win_TPropertyCtrl_SetImageList(this, hImageList); }

	void setFocus()
		{ axl_win_TPropertyCtrl_SetFocus(this); }

	void invalidate()
		{ axl_win_TPropertyCtrl_Invalidate(this); }

	void clientToScreen(POINT* point)
		{ axl_win_TPropertyCtrl_ClientToScreen(this, point); }

	axl_win_TPropertyLine* getFirstLine()
		{ return axl_win_TPropertyCtrl_GetFirstLine(this); }

	axl_win_TPropertyLine* getLastLine()
		{ return axl_win_TPropertyCtrl_GetLastLine(this); }

	axl_win_TPropertyLine* getSelectedLine()
		{ return axl_win_TPropertyCtrl_GetSelectedLine(this); }

	bool saveActiveInPlace()
		{ return axl_win_TPropertyCtrl_SaveActiveInPlace(this) != 0; }

	axl_win_TPropertyLine* insertLine(axl_win_TPropertyLine* parent = NULL, axl_win_TPropertyLine* before = NULL)
		{ return axl_win_TPropertyCtrl_InsertLine(this, parent, before); }

	void updateLineWidth(axl_win_TPropertyLine* line)
		{ axl_win_TPropertyCtrl_UpdateLineWidth(this, line); }

	void updateAllLineWidths()
		{ axl_win_TPropertyCtrl_UpdateAllLineWidths(this); }

	long setSplitterPos(long splitterPos)
		{ return axl_win_TPropertyCtrl_SetSplitterPos(this, splitterPos); }

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

struct PropertyLineKind { enum
{
	singlePane           = axl_win_EPropertyLine_SinglePane,
	reArrangeable        = axl_win_EPropertyLine_ReArrangeable,
	isExpanded           = axl_win_EPropertyLine_IsExpanded,
	isDisabled           = axl_win_EPropertyLine_IsDisabled,
	notSelectable        = axl_win_EPropertyLine_NotSelectable,
	button               = axl_win_EPropertyLine_Button,
	nameHyperText        = axl_win_EPropertyLine_NameHyperText,
	valueHyperText       = axl_win_EPropertyLine_ValueHyperText,
	menu                 = axl_win_EPropertyLine_Menu,
	noChildGridLines     = axl_win_EPropertyLine_NoChildGridLines,
	forceNoGridLines     = axl_win_EPropertyLine_ForceNoGridLines,
	alwaysDrawButton     = axl_win_EPropertyLine_AlwaysDrawButton,
	nameShadow           = axl_win_EPropertyLine_NameShadow,
	valueShadow          = axl_win_EPropertyLine_ValueShadow,
	separator            = axl_win_EPropertyLine_Separator
};};

struct PropertyLineDelimiterKind { enum
{
	none        = axl_win_EPropertyLineDelimiter_None,
	nameTop     = axl_win_EPropertyLineDelimiter_NameTop,
	nameBottom  = axl_win_EPropertyLineDelimiter_NameBottom,
	valueTop    = axl_win_EPropertyLineDelimiter_ValueTop,
	valueBottom = axl_win_EPropertyLineDelimiter_ValueBottom,
	all         = axl_win_EPropertyLineDelimiter_All,

	top         = axl_win_EPropertyLineDelimiter_Top,
	bottom      = axl_win_EPropertyLineDelimiter_Bottom,
	name        = axl_win_EPropertyLineDelimiter_Name,
	value       = axl_win_EPropertyLineDelimiter_Value,
};};

struct PropertyOptionBoxTypeKind { enum
{
	none        = axl_win_EPropertyOptionBoxType_None,
	checkBox    = axl_win_EPropertyOptionBoxType_CheckBox,
	radioButton = axl_win_EPropertyOptionBoxType_RadioButton,
};};

struct PropertyCheckStateKind { enum
{
	unchecked    = axl_win_EPropertyCheckState_Unchecked,
	checked      = axl_win_EPropertyCheckState_Checked,
	state3       = axl_win_EPropertyCheckState_State3,
};};

struct PropertyHitTestKind { enum
{
	none      = axl_win_EPropertyHitTest_None,
	indent    = axl_win_EPropertyHitTest_Indent,
	plusMinus = axl_win_EPropertyHitTest_PlusMinus,
	optionBox = axl_win_EPropertyHitTest_OptionBox,
	button    = axl_win_EPropertyHitTest_Button,
	menu      = axl_win_EPropertyHitTest_Menu,
	icon      = axl_win_EPropertyHitTest_Icon,
	name      = axl_win_EPropertyHitTest_Name,
	nameTail  = axl_win_EPropertyHitTest_NameTail,
	splitter  = axl_win_EPropertyHitTest_Splitter,
	value     = axl_win_EPropertyHitTest_Value,
	valueTail = axl_win_EPropertyHitTest_ValueTail,
	hyperlink = axl_win_EPropertyHitTest_Hyperlink,
};};

struct PropertyNotifyKind { enum
{
	buttonClick    = axl_win_EPropertyNotify_ButtonClick,
	optionBoxClick = axl_win_EPropertyNotify_OptionBoxClick,
	hyperlinkClick = axl_win_EPropertyNotify_HyperlinkClick,
	menuClick      = axl_win_EPropertyNotify_MenuClick,
	reArrange      = axl_win_EPropertyNotify_ReArrange,
	expandCollapse = axl_win_EPropertyNotify_ExpandCollapse,
};};

typedef axl_win_TPropertyCtrl    PropertyCtrl;
typedef axl_win_TPropertyLine    PropertyLine;
typedef axl_win_TPropertyInPlace PropertyInPlace;
typedef axl_win_TPropertyHitTest PropertyHitest;
typedef axl_win_TPropertyNotify  PropertyNotify;

class PropertyCtrl: public PropertyCtrl
{
public:
	PropertyCtrl()
		{ axl_win_TPropertyCtrl_Construct(this); }

	~PropertyCtrl()
		{ axl_win_TPropertyCtrl_Destruct(this); }
};

class PropertyInPlace: public PropertyInPlace
{
public:
	PropertyInPlace(axl_obj_TAgent* agent)
		{ axl_win_TPropertyInPlace_Construct(this, agent); }
};

template <class T>
class PropertyInPlace: public PropertyInPlace
{
public:
	typedef PropertyInPlace BaseClass;

public:
	PropertyInPlace(axl::obj::Agent* agent):
		PropertyInPlace(agent)
	{ 
		m_pfLoad = _Load;
		m_pfSave = _Save;
		m_pfLayout = _Layout;
		m_pfSetFocus = _SetFocus;
	}

protected:
	static bool_t AXL_API _Load(axl_win_TPropertyInPlace* inPlace, axl_win_TPropertyLine* line, const RECT* rect, ulong_t* minWidth)
		{ return static_cast<T*>(inPlace)->load(line, rect, minWidth); }

	static bool_t AXL_API _Save(axl_win_TPropertyInPlace* inPlace, axl_win_TPropertyLine* line)
		{ return static_cast<T*>(inPlace)->save(line); }

	static void AXL_API _Layout(axl_win_TPropertyInPlace* inPlace, const RECT* rect)
		{ static_cast<T*>(inPlace)->layout(rect); }

	static void AXL_API _SetFocus(axl_win_TPropertyInPlace* inPlace)
		{ static_cast<T*>(inPlace)->setFocus(); }
};

} // namespace win {
} // namespace axl {

#endif

//.............................................................................

