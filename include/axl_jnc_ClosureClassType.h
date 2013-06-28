// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_ClassType.h"

namespace axl {
namespace jnc {

//.............................................................................

class CClosureClassType: public CClassType
{
	friend class CTypeMgr;

protected:
	CFunction* m_pThunkMethod;

public:
	CClosureClassType ();

	virtual 
	bool
	Compile ()
	{
		return 
			CClassType::Compile () &&
			CompileThunkMethod ();
	}

protected:
	bool
	CompileThunkMethod ();
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
