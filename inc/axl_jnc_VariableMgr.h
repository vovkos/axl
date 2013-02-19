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

	rtl::CStdListT <CVariable> m_GlobalVariableList;
	rtl::CStdListT <CVariable> m_LocalVariableList;

	CVariable* m_pScopeLevelVariable;

public:
	CVariableMgr ();

	CModule* 
	GetModule ()
	{
		return m_pModule;
	}

	void
	Clear ();

	CVariable*
	CreateVariable (
		EVariable VariableKind,
		const rtl::CString& Name,
		const rtl::CString& QualifiedName,
		CType* pType,
		int PtrTypeFlags = 0
		);

	bool
	AllocateGlobalVariables ();

	CVariable*
	GetScopeLevelVariable ();
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
