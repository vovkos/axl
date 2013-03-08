// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_AutoEvType.h"
#include "axl_jnc_FunctionPtrType.h"

namespace axl {
namespace jnc {

//.............................................................................

enum EAutoEvPtrType
{
	EAutoEvPtrType_Normal = 0,
	EAutoEvPtrType_Weak,
	EAutoEvPtrType_Thin,
	EAutoEvPtrType_Unsafe,
	EAutoEvPtrType__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const tchar_t*
GetAutoEvPtrTypeKindString (EAutoEvPtrType PtrTypeKind);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
EAutoEvPtrType 
GetAutoEvPtrTypeKindFromModifiers (int Modifiers)
{
	return 
		(Modifiers & ETypeModifier_Unsafe) ? EAutoEvPtrType_Unsafe :
		(Modifiers & ETypeModifier_Weak) ? EAutoEvPtrType_Weak : 
		(Modifiers & ETypeModifier_Thin) ? EAutoEvPtrType_Thin : EAutoEvPtrType_Normal;
}

//.............................................................................

class CAutoEvPtrType: public CType
{
protected:
	friend class CTypeMgr;

	EAutoEvPtrType m_PtrTypeKind;
	CAutoEvType* m_pTargetType;

public:
	CAutoEvPtrType ();

	EAutoEvPtrType
	GetPtrTypeKind ()
	{
		return m_PtrTypeKind;
	}

	CAutoEvType* 
	GetTargetType ()
	{
		return m_pTargetType;
	}

	bool
	HasClosure ()
	{
		return m_PtrTypeKind == EAutoEvPtrType_Normal || m_PtrTypeKind == EAutoEvPtrType_Weak;
	}

	CStructType* 
	GetAutoEvPtrStructType ();

	CStructType* 
	GetAutoEvPtrStructType_w ();

	static
	rtl::CStringA
	CreateSignature (
		CAutoEvType* pAutoEvType,
		EType TypeKind,
		EAutoEvPtrType PtrTypeKind,
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

class CAutoEvPtrTypeTuple: public rtl::TListLink
{
protected:
	friend class CTypeMgr;

	CStructType* m_pPtrStructType;
	CStructType* m_pPtrStructType_w;

	CAutoEvPtrType* m_PtrTypeArray [2] [EAutoEvPtrType__Count] [2]; // ref x kind x nullable

public:
	CAutoEvPtrTypeTuple ()
	{
		memset (this, 0, sizeof (CAutoEvPtrTypeTuple));
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {

