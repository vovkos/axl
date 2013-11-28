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
	CNamespace* m_pAnchorNamespace; // for dual pointers

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

	CNamespace* 
	GetAnchorNamespace ()
	{
		return m_pAnchorNamespace;
	}

	bool
	IsConstPtrType ();

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

	CDataPtrType*
	GetUnThinPtrType ()
	{
		return (m_PtrTypeKind == EDataPtrType_Thin) ?  
			m_pTargetType->GetDataPtrType (m_TypeKind, EDataPtrType_Normal, m_Flags) : 
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

	virtual 
	void
	PrepareLlvmDiType ();
};

//.............................................................................

struct TDataPtrTypeTuple: rtl::TListLink
{
	CStructType* m_pPtrStructType;
	CDataPtrType* m_PtrTypeArray [2] [2] [2] [2] [3]; // ref x kind x const x volatile x unsafe / checked
};

//.............................................................................

inline
bool 
IsCharPtrType (CType* pType)
{
	return 
		pType->GetTypeKind () == EType_DataPtr &&
		((CDataPtrType*) pType)->GetTargetType ()->GetTypeKind () == EType_Char;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
bool 
IsSafeThinDataPtrType (CType* pType)
{
	return 
		pType->GetTypeKind () == EType_DataPtr &&
		!(pType->GetFlags () & EPtrTypeFlag_Unsafe) &&
		((CDataPtrType*) pType)->GetPtrTypeKind () == EDataPtrType_Thin;		
}

//.............................................................................

// structure backing up fat data pointer, e.g.:
// int* p;

struct TDataPtr
{
	void* m_p;
	void* m_pRangeBegin;
	void* m_pRangeEnd;
	size_t m_ScopeLevel;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// structure backing up formatting literal

struct TFmtLiteral
{
	char* m_p;
	size_t m_MaxLength;
	size_t m_Length;
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
