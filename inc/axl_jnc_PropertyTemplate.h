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
	friend class CFunctionMgr;
	friend class CParser;

	CFunctionType* m_pGetterType;
	CFunctionTypeOverload m_SetterType;
	CPropertyVerifier m_Verifier;
	CType* m_pAuPropValueType; // before the type is calculated
	int m_TypeModifiers;

public:
	CPropertyTemplate ();

	CFunctionType* 
	GetGetterType ()
	{
		return m_pGetterType;
	}

	CFunctionTypeOverload*
	GetSetterType ()
	{
		return &m_SetterType;
	}

	bool
	AddMethodMember (
		EFunction FunctionKind,
		CFunctionType* pFunctionType
		);

	CPropertyType*
	CalcType ();

};

//.............................................................................

} // namespace jnc {
} // namespace axl {
