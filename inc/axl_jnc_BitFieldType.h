// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"

namespace axl {
namespace jnc {

//.............................................................................

class CBitFieldType: public CDerivedType
{
protected:
	friend class CTypeMgr;

	size_t m_BitOffset;
	size_t m_BitCount;

public:
	CBitFieldType ()
	{
		m_TypeKind = EType_BitField;
		m_BitOffset = 0;
		m_BitCount = 0;
	}

	size_t
	GetBitOffset ()
	{
		return m_BitOffset;
	}

	size_t
	GetBitCount ()
	{
		return m_BitCount;
	}

	static
	rtl::CStringA
	CreateSignature (
		CType* pBaseType,
		size_t BitOffset,
		size_t BitCount
		)
	{ 
		return rtl::CStringA::Format_s (
			"B%s:%d:%d", 
			pBaseType->GetSignature (),
			BitOffset,
			BitOffset + BitCount
			); 
	}

	static
	rtl::CString
	CreateTypeString (
		CType* pBaseType,
		size_t BitOffset,
		size_t BitCount
		)
	{ 
		return rtl::CStringA::Format_s (
			"%s:%d:%d",
			pBaseType->GetTypeString (),
			BitOffset,
			BitOffset + BitCount
			); 
	}

	rtl::CString
	CreateTypeString ()
	{ 
		return CreateTypeString (m_pBaseType, m_BitOffset, m_BitCount);
	}
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
