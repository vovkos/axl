// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"

namespace axl {
namespace jnc {

//.............................................................................

class CArrayType: public CDerivedType
{
protected:
	friend class CTypeMgr;

	size_t m_ElementCount;

public:
	CArrayType ()
	{
		m_TypeKind = EType_Array;
		m_ElementCount = 0;
	}

	size_t
	GetElementCount ()
	{
		return m_ElementCount;
	}

	CType* 
	GetRootType ()
	{
		return GetRootType (m_pBaseType);
	}

	static
	CType* 
	GetRootType (CType* pBaseType);

	static
	rtl::CStringA
	CreateSignature (
		CType* pBaseType,
		size_t ElementCount
		)
	{ 
		return rtl::CStringA::Format_s ("A%d%s", ElementCount, pBaseType->GetSignature ()); 
	}

	static
	rtl::CString
	CreateTypeString (
		CType* pBaseType,
		size_t ElementCount
		);

	rtl::CString
	CreateTypeString ()
	{
		return CreateTypeString (m_pBaseType, m_ElementCount);
	}
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
