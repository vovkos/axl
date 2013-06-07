// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"

namespace axl {
namespace jnc {

//.............................................................................

class CArrayType: public CType
{
	friend class CTypeMgr;

protected:
	CType* m_pElementType;
	CType* m_pRootType;
	size_t m_ElementCount;

public:
	CArrayType ();

	CType*
	GetElementType ()
	{
		return m_pElementType;
	}

	CType* 
	GetRootType ()
	{
		return m_pRootType;
	}

	size_t
	GetElementCount ()
	{
		return m_ElementCount;
	}

	virtual
	size_t
	GetAlignFactor ()
	{
		return m_pRootType->GetAlignFactor ();
	}

	static
	rtl::CString
	CreateSignature (
		CType* pElementType,
		size_t ElementCount
		)
	{ 
		return rtl::CString::Format_s (
			"A%d%s", 
			ElementCount, 
			pElementType->GetSignature ().cc () // thanks a lot gcc
			);  
	}

protected:
	virtual 
	void
	PrepareTypeString ();

	virtual 
	void
	PrepareLlvmType ()
	{
		m_pLlvmType = llvm::ArrayType::get (m_pElementType->GetLlvmType (), m_ElementCount);
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {