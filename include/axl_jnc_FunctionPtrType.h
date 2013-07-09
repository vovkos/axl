// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_FunctionType.h"

namespace axl {
namespace jnc {

//.............................................................................

class CFunctionPtrType: public CType
{
	friend class CTypeMgr;

protected:
	EFunctionPtrType m_PtrTypeKind;
	CFunctionType* m_pTargetType;
	CClassType* m_pMulticastType;
	rtl::CString m_TypeModifierString;

public:
	CFunctionPtrType ();

	EFunctionPtrType
	GetPtrTypeKind ()
	{
		return m_PtrTypeKind;
	}

	CFunctionType* 
	GetTargetType ()
	{
		return m_pTargetType;
	}

	bool
	HasClosure ()
	{
		return m_PtrTypeKind == EFunctionPtrType_Normal || m_PtrTypeKind == EFunctionPtrType_Weak;
	}

	CFunctionPtrType*
	GetCheckedPtrType ()
	{
		return !(m_Flags & (EPtrTypeFlag_Checked | EPtrTypeFlag_Unsafe)) ?  
			m_pTargetType->GetFunctionPtrType (m_TypeKind, m_PtrTypeKind, m_Flags | EPtrTypeFlag_Checked) : 
			this;			
	}

	CFunctionPtrType*
	GetUnCheckedPtrType ()
	{
		return (m_Flags & EPtrTypeFlag_Checked) ?  
			m_pTargetType->GetFunctionPtrType (m_TypeKind, m_PtrTypeKind, m_Flags & ~EPtrTypeFlag_Checked) : 
			this;			
	}

	CClassType* 
	GetMulticastType ();

	CStructType* 
	GetFunctionPtrStructType ();

	rtl::CString
	GetTypeModifierString ();

	static
	rtl::CString
	CreateSignature (
		CFunctionType* pFunctionType,
		EType TypeKind,
		EFunctionPtrType PtrTypeKind,
		uint_t Flags
		);

	virtual 
	void
	EnumGcRoots (
		CGcHeap* pGcHeap,
		void* p
		);	

protected:
	virtual 
	void
	PrepareTypeString ();

	virtual 
	void
	PrepareLlvmType ();
};

//.............................................................................

struct TFunctionPtrTypeTuple: rtl::TListLink
{
	CStructType* m_pPtrStructType;
	CFunctionPtrType* m_PtrTypeArray [2] [3] [3]; // ref x kind x unsafe/checked
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
