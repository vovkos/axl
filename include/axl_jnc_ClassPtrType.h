// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_ClassType.h"

namespace axl {
namespace jnc {

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
	rtl::CString
	CreateSignature (
		CClassType* pClassType,
		EClassPtrType PtrTypeKind,
		uint_t Flags
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

	CClassPtrType* m_PtrTypeArray [EClassPtrType__Count] [2] [2]; // ptrkind x const x nullable

public:
	CClassPtrTypeTuple ()
	{
		memset (this, 0, sizeof (CClassPtrTypeTuple));
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
