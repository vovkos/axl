// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_FunctionType.h"
#include "axl_jnc_Namespace.h"

namespace axl {
namespace jnc {

//.............................................................................

class CPropertyType: 
	public CType,
	public rtl::TListLink
{
protected:
	friend class CTypeMgr;

	CFunctionType* m_pGetterType;
	CFunctionTypeOverload m_SetterType;

public:
	CPropertyType ();

	bool
	IsReadOnly ()
	{
		return m_SetterType.IsEmpty ();
	}

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

	static
	rtl::CStringA
	CreateSignature (
		CFunctionType* pGetterType,
		const CFunctionTypeOverload& SetterType
		);

	static
	rtl::CString
	CreateTypeString (
		CFunctionType* pGetterType,
		const CFunctionTypeOverload& SetterType
		);

	rtl::CString
	CreateTypeString ()
	{
		return CreateTypeString (m_pGetterType, m_SetterType);
	}
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
