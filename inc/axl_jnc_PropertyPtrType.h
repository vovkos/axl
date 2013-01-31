// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_PropertyType.h"
#include "axl_jnc_FunctionPtrType.h"

namespace axl {
namespace jnc {

//.............................................................................

enum EPropertyPtrType
{
	EPropertyPtrType_Normal = 0,
	EPropertyPtrType_Thin,
	EPropertyPtrType_Weak,
	EPropertyPtrType_Unsafe,
	EPropertyPtrType__Count,
};

const tchar_t*
GetPropertyPtrTypeKindString (EPropertyPtrType PtrTypeKind);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
EPropertyPtrType 
GetPropertyPtrTypeKindFromModifiers (int Modifiers)
{
	return 
		(Modifiers & ETypeModifier_Unsafe) ? EPropertyPtrType_Unsafe :
		(Modifiers & ETypeModifier_Weak) ? EPropertyPtrType_Weak : 
		(Modifiers & ETypeModifier_Thin) ? EPropertyPtrType_Thin : EPropertyPtrType_Normal;
}

//.............................................................................

class CPropertyPtrType: public CType
{
protected:
	friend class CTypeMgr;

	EPropertyPtrType m_PtrTypeKind;
	CPropertyType* m_pPropertyType;

public:
	CPropertyPtrType ();

	EPropertyPtrType
	GetPtrTypeKind ()
	{
		return m_PtrTypeKind;
	}

	CPropertyType* 
	GetPropertyType ()
	{
		return m_pPropertyType;
	}

	CStructType* 
	GetPropertyPtrStructType ();

	CStructType* 
	GetPropertyWeakPtrStructType ();

	CStructType* 
	GetBindablePropertyThinPtrStructType ();

	CStructType* 
	GetBindablePropertyUnsafePtrStructType ();

	static
	rtl::CStringA
	CreateSignature (
		CPropertyType* pPropertyType,
		EType TypeKind,
		EPropertyPtrType PtrTypeKind,
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

class CPropertyPtrTypeTuple: public rtl::TListLink
{
protected:
	friend class CTypeMgr;

	CStructType* m_pPropertyPtrStructType;
	CStructType* m_pPropertyWeakPtrStructType;
	CStructType* m_pBindablePropertyThinPtrStructType;
	CStructType* m_pBindablePropertyUnsafePtrStructType;

	CPropertyPtrType* m_PropertyPtrArray [2] [EPropertyPtrType__Count] [2]; // ref x kind x nonull

public:
	CPropertyPtrTypeTuple ()
	{
		memset (this, 0, sizeof (CPropertyPtrTypeTuple));
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {

