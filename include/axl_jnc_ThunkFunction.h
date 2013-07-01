// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Function.h"

namespace axl {
namespace jnc {

//.............................................................................

class CThunkFunction: public CFunction
{
	friend class CFunctionMgr;

protected:
	rtl::CString m_Signature;
	CFunction* m_pTargetFunction;

public:
	CThunkFunction ();

	virtual
	bool 
	Compile ();
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
