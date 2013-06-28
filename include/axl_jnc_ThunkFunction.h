// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Function.h"

namespace axl {
namespace jnc {

//.............................................................................

enum EThunk
{
	EThunk_Undefined = 0,
	EThunk_Direct,
	EThunk_DirectUnusedClosure,
	EThunk_Closure,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CThunkFunction: public CFunction
{
	friend class CFunctionMgr;

protected:
	EThunk m_ThunkKind;
	rtl::CString m_Signature;
	CFunction* m_pTargetFunction; 
	CFunctionType* m_pTargetFunctionType;
	CClassType* m_pClosureType;
	rtl::CArrayT <size_t> m_ClosureMap;

public:
	CThunkFunction ();

	virtual
	bool 
	Compile ()
	{
		return m_pClosureType ?
			CompileClosureThunk () :
			CompileDirectThunk ();
	}

protected:
	bool
	CompileDirectThunk ();

	bool
	CompileClosureThunk ();
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
