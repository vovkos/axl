// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"

namespace axl {
namespace jnc {

//.............................................................................
	
class CDataPtrType: public CType
{
	friend class CTypeMgr;
	
protected:
	EDataPtrType m_PtrTypeKind;
	CType* m_pTargetType;

public:
	CDataPtrType ();

	EDataPtrType
	GetPtrTypeKind ()
	{
		return m_PtrTypeKind;
	}

	CType*
	GetTargetType ()
	{
		return m_pTargetType;
	}

	CDataPtrType*
	GetCheckedPtrType ()
	{
		return !(m_Flags & (EPtrTypeFlag_Checked | EPtrTypeFlag_Unsafe)) ?  
			m_pTargetType->GetDataPtrType (m_TypeKind, m_PtrTypeKind, m_Flags | EPtrTypeFlag_Checked) : 
			this;			
	}

	CDataPtrType*
	GetUnCheckedPtrType ()
	{
		return (m_Flags & EPtrTypeFlag_Checked) ?  
			m_pTargetType->GetDataPtrType (m_TypeKind, m_PtrTypeKind, m_Flags & ~EPtrTypeFlag_Checked) : 
			this;			
	}

	CDataPtrType*
	GetUnConstPtrType ()
	{
		return (m_Flags & EPtrTypeFlag_Const) ?  
			m_pTargetType->GetDataPtrType (m_TypeKind, m_PtrTypeKind, m_Flags & ~EPtrTypeFlag_Const) : 
			this;			
	}

	CStructType* 
	GetDataPtrStructType ();

	static
	rtl::CString
	CreateSignature (
		CType* pBaseType,
		EType TypeKind,
		EDataPtrType PtrTypeKind,
		uint_t Flags
		);

	virtual 
	void
	EnumGcRoots (
		CRuntime* pRuntime,
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

struct TDataPtrTypeTuple: rtl::TListLink
{
	CStructType* m_pPtrStructType;
	CDataPtrType* m_PtrTypeArray [2] [2] [2] [2] [3]; // ref x kind x const x volatile x unsafe / checked
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
