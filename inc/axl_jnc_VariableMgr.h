// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Variable.h"

namespace axl {
namespace jnc {

//.............................................................................

class CVariableMgr
{
protected:
	friend class CModule;
	CModule* m_pModule;

	rtl::CStdListT <CVariable> m_VariableList;

public:
	CVariableMgr (CModule* pModule);

	CModule* 
	GetModule ()
	{
		return m_pModule;
	}

	void
	Clear ();

	CVariable*
	CreateVariable (
		const rtl::CString& Name,
		CType* pType
		);
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
