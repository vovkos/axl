// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_ClassType.h"

namespace axl {
namespace jnc {

//.............................................................................

enum EClassPtrType
{
	EClassPtrType_Normal = 0,
	EClassPtrType_Weak,
	EClassPtrType_Unsafe,
	EClassPtrType__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const tchar_t*
GetClassPtrTypeKindString (EClassPtrType PtrTypeKind);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
EClassPtrType 
GetClassPtrTypeKindFromModifiers (int Modifiers)
{
	return 
		(Modifiers & (ETypeModifier_Unsafe | ETypeModifier_Unsafe_p)) ? EClassPtrType_Unsafe :
		(Modifiers & (ETypeModifier_Weak | ETypeModifier_Weak_p)) ? EClassPtrType_Weak : EClassPtrType_Normal;
}

//.............................................................................

class CClassPtrType: public CType
{
protected:
	friend class CTypeMgr;

	EClassPtrType m_PtrTypeKind;
	CClassType* m_pTargetType;

public:
	CClassPtrType ();

	EClassPtrType 
	GetPtrTypeKind ()
	{
		return m_PtrTypeKind;
	}

	CClassType* 
	GetTargetType ()
	{
		return m_pTargetType;
	}

	static
	rtl::CStringA
	CreateSignature (
		CClassType* pClassType,
		EClassPtrType PtrTypeKind,
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

class CClassPtrTypeTuple: public rtl::TListLink
{
protected:
	friend class CTypeMgr;

	CClassPtrType* m_ClassPtrArray [EClassPtrType__Count] [2] [2]; // ptrkind x const x nonull

public:
	CClassPtrTypeTuple ()
	{
		memset (this, 0, sizeof (CClassPtrTypeTuple));
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
