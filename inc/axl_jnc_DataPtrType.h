// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"
#include "axl_jnc_FunctionType.h"

namespace axl {
namespace jnc {

//.............................................................................

enum EDataPtrType
{
	EDataPtrType_Normal = 0,
	EDataPtrType_Thin,
	EDataPtrType_Unsafe,
	EDataPtrType__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const tchar_t*
GetDataPtrTypeKindString (EDataPtrType PtrTypeKind);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
EDataPtrType
GetDataPtrTypeKindFromModifiers (int Modifiers)
{
	return (Modifiers & ETypeModifier_Unsafe) ? EDataPtrType_Unsafe : EDataPtrType_Normal;
}

//.............................................................................

class CDataPtrType: public CType
{
protected:
	friend class CTypeMgr;
	
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

	CStructType* 
	GetDataPtrStructType ();

	static
	rtl::CStringA
	CreateSignature (
		CType* pBaseType,
		EType TypeKind,
		EDataPtrType PtrTypeKind,
		int Flags
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

class CDataPtrTypeTuple: public rtl::TListLink
{
protected:
	friend class CTypeMgr;

	CStructType* m_pDataPtrStructType;
	CDataPtrType* m_DataPtrArray [2] [EDataPtrType__Count] [2] [2] [2]; // ref x ptrkind x const x volatile x nullable

public:
	CDataPtrTypeTuple ()
	{
		memset (this, 0, sizeof (CDataPtrTypeTuple));
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
