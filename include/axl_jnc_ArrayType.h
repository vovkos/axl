// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_ImportType.h"

namespace axl {
namespace jnc {

//.............................................................................

class CArrayType: public CType
{
	friend class CTypeMgr;

protected:
	CType* m_pElementType;
	CImportType* m_pElementType_i;
	CType* m_pRootType;
	size_t m_ElementCount;

public:
	CArrayType ();

	CType*
	GetElementType ()
	{
		return m_pElementType;
	}

	CImportType*
	GetElementType_i ()
	{
		return m_pElementType_i;
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

	virtual 
	void
	EnumGcRoots (
		CGcHeap* pGcHeap,
		void* p
		);	

protected:
	virtual 
	bool
	CalcLayout ();

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

inline
bool 
IsAutoSizeArrayType (CType* pType)
{
	return 
		pType->GetTypeKind () == EType_Array &&
		((CArrayType*) pType)->GetElementCount () == 0;
}

inline
bool 
IsCharArrayType (CType* pType)
{
	return 
		pType->GetTypeKind () == EType_Array &&
		((CArrayType*) pType)->GetElementType ()->GetTypeKind () == EType_Char;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
