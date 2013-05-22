// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_AttributeBlock.h"

namespace axl {
namespace jnc {

class CModule;

//.............................................................................

class CAttributeMgr: public rtl::TListLink
{
protected:
	friend class CModule;
	CModule* m_pModule;

	rtl::CStdListT <CAttributeBlock> m_AttributeBlockList;

public:
	CAttributeMgr ();

	CModule* 
	GetModule ()
	{
		return m_pModule;
	}

	void
	Clear ()
	{
		m_AttributeBlockList.Clear ();
	}

	CAttributeBlock*
	CreateAttributeBlock ();

	CAttribute*
	CreateAttribute (
		const rtl::CString& Name,
		CValue* pValue = NULL
		);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
