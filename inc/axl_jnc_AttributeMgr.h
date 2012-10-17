// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Attribute.h"

namespace axl {
namespace jnc {

//.............................................................................

class CAttributeMgr: public rtl::TListLink
{
protected:
	friend class CModule;
	CModule* m_pModule;

	rtl::CStdListT <CAttributeSet> m_AttributeSetList;
	CAttributeSet* m_pCurrentAttributeSet;

public:
	CAttributeMgr ()
	{
		m_pModule = NULL;
		m_pCurrentAttributeSet = NULL;
	}

	CAttributeSet*
	GetCurrentAttributeSet ()
	{
		return m_pCurrentAttributeSet;
	}

	void
	DropCurrentAttributeSet ()
	{
		m_pCurrentAttributeSet = NULL;
	}

	void
	AssignAttributeSet (CModuleItem* pItem);

	void
	Clear ()
	{
		m_AttributeSetList.Clear ();
	}

	CAttributeSet*
	CreateAttributeSet ();

	CAttribute*
	CreateAttribute (
		const rtl::CString& Name,
		CValue* pValue = NULL
		);
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
