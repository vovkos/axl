// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_PropertyVerifier.h"

namespace axl {
namespace jnc {

//.............................................................................

// we need to keep namespaces for property templates cause other module items 
// could potentially have pointers to them via m_pParentNamespace

class CPropertyTemplate:	
	public CModuleItem,
	public CNamespace
{
protected:
	CFunctionType* m_pGetterType;
	CFunctionTypeOverload m_SetterType;
	CPropertyVerifier m_Verifier;

public:
	CPropertyTemplate ()
	{
		m_pGetterType = NULL;
	}

	CFunctionType* 
	GetGetterType ()
	{
		return m_pGetterType;
	}

	const CFunctionTypeOverload& 
	GetSetterType ()
	{
		return m_SetterType;
	}

	bool
	AddMethodMember (
		EFunction FunctionKind,
		CFunctionType* pFunctionType
		);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
