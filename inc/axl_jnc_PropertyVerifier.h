// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_PropertyType.h"
#include "axl_jnc_Function.h"

namespace axl {
namespace jnc {

//.............................................................................

class CPropertyVerifier
{
protected:
	int m_MethodMemberMask;           // only setter could be overloaded
	rtl::CString m_IndexArgSignature; // all accessors must have matching index arg signature
	
public:
	CPropertyVerifier ()
	{
		m_MethodMemberMask = 0;
	}

	bool
	AddMethodMember (
		EFunction FunctionKind,
		CFunctionType* pFunctionType
		);

protected:
	static
	rtl::CString 
	CreateIndexArgSignature (
		EFunction FunctionKind,
		CFunctionType* pFunctionType
		);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
