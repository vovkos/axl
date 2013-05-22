// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_FunctionType.h"

namespace axl {
namespace jnc {

class CAutoEvPtrType;
class CAutoEvPtrTypeTuple;

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

const char*
GetAutoEvPtrTypeKindString (EAutoEvPtrType PtrTypeKind);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
EAutoEvPtrType 
GetAutoEvPtrTypeKindFromModifiers (uint_t Modifiers)
{
	return 
		(Modifiers & ETypeModifier_Unsafe) ? EAutoEvPtrType_Unsafe :
		(Modifiers & ETypeModifier_Weak) ? EAutoEvPtrType_Weak : 
		(Modifiers & ETypeModifier_Thin) ? EAutoEvPtrType_Thin : EAutoEvPtrType_Normal;
}

//.............................................................................

class CAutoEvType: public CType
{
protected:
	friend class CTypeMgr;

	CFunctionType* m_pStarterType;
	CFunctionType* m_pStopperType;
	CAutoEvType* m_pStdObjectMemberAutoEvType;
	CAutoEvType* m_pShortType;
	CStructType* m_pVTableStructType;

	CAutoEvPtrTypeTuple* m_pAutoEvPtrTypeTuple;

public:
	CAutoEvType ();

	CFunctionType*
	GetStarterType ()
	{
		return m_pStarterType;
	}

	CFunctionType*
	GetStopperType ()
	{
		return m_pStopperType;
	}

	bool
	IsMemberAutoEvType ()
	{
		return m_pStarterType->IsMemberMethodType ();
	}

	CType*
	GetThisArgType ()
	{
		return m_pStarterType->GetThisArgType (); 
	}

	CAutoEvType*
	GetMemberAutoEvType (CClassType* pType);

	CAutoEvType*
	GetStdObjectMemberAutoEvType ();

	CAutoEvType*
	GetShortType  ();

	CAutoEvPtrType* 
	GetAutoEvPtrType (
		EType TypeKind,
		EAutoEvPtrType PtrTypeKind = EAutoEvPtrType_Normal,
		uint_t Flags = 0
		);

	CAutoEvPtrType* 
	GetAutoEvPtrType (
		EAutoEvPtrType PtrTypeKind = EAutoEvPtrType_Normal,
		uint_t Flags = 0
		)
	{
		return GetAutoEvPtrType (EType_AutoEvPtr, PtrTypeKind, Flags);
	}

	CStructType*
	GetVTableStructType ();

	static
	rtl::CString
	CreateSignature (
		CFunctionType* pStarterType,
		CFunctionType* pStopperType
		)
	{ 
		return rtl::CString::Format_s (
			"V%s%s", 
			pStarterType->GetSignature ().cc (), // thanks a lot gcc
			pStopperType->GetSignature ().cc ()
			); 
	}

protected:
	virtual 
	void
	PrepareTypeString ()
	{
		m_TypeString.Format ("autoev %s", m_pStarterType->GetArgString ().cc ());
	}

	virtual 
	void
	PrepareLlvmType ()
	{
		ASSERT (false);
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
