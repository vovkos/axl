#pragma once

#include "..\axl_jnc\axl_jnc_Parser.h"

//.............................................................................

class CModulePane : public CDockablePane
{
protected:
	CTreeCtrl m_TreeCtrl;

public:
	bool
	Build (jnc::CModule* pModule);

	void
	Clear ();

protected:
	bool
	AddItemAttributes (
		HTREEITEM hParent,
		jnc::CModuleItem* pItem
		);

	void
	AddNamespace (
		HTREEITEM hParent,
		jnc::CGlobalNamespace* pNamespace
		);

	void
	AddItem (
		HTREEITEM hParent,
		jnc::CModuleItem* pItem
		);

	void
	AddType (
		HTREEITEM hParent,
		jnc::CType* pType
		);

	void
	AddAlias (
		HTREEITEM hParent,
		jnc::CAlias* pAlias
		);

	void
	AddVariable (
		HTREEITEM hParent,
		jnc::CVariable* pVariable
		);

	void
	AddEnumMember (
		HTREEITEM hParent,
		jnc::CEnumMember* pMember
		);

	void
	AddStructMember (
		HTREEITEM hParent,
		jnc::CStructMember* pMember
		);

	void
	AddClassMember (
		HTREEITEM hParent,
		jnc::CClassMember* pMember
		);

	void
	AddFunction (
		HTREEITEM hParent,
		const rtl::CString& Name,
		jnc::CFunction* pFunction
		);

	void
	CModulePane::AddFunction (
		HTREEITEM hParent,
		const rtl::CString& Name,
		jnc::CFunctionOverload* pFunction
		);

	void
	AddProperty (
		HTREEITEM hParent,
		const rtl::CString& Name,
		jnc::CProperty* pProperty
		);

	void
	AddGlobalFunction (
		HTREEITEM hParent,
		jnc::CGlobalFunction* pFunction
		);

	void
	AddGlobalProperty (
		HTREEITEM hParent,
		jnc::CGlobalProperty* pProperty
		);

	void
	AddEnumTypeMembers (
		HTREEITEM hParent,
		jnc::CEnumType* pType
		);

	void
	CModulePane::AddStructTypeMembers (
		HTREEITEM hParent,
		jnc::CStructType* pType
		);

	void
	CModulePane::AddClassTypeMembers (
		HTREEITEM hParent,
		jnc::CClassType* pType
		);

	rtl::CString
	GetItemTip (jnc::CModuleItem* pItem);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblClk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetInfoTip(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

//.............................................................................
