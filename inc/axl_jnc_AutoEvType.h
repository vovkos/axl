// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_FunctionType.h"

namespace axl {
namespace jnc {

class CAutoEvPtrType;
class CAutoEvPtrTypeTuple;

enum EAutoEvPtrType;

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
		EAutoEvPtrType PtrTypeKind = (EAutoEvPtrType) 0,
		int Flags = 0
		);

	CAutoEvPtrType* 
	GetAutoEvPtrType (
		EAutoEvPtrType PtrTypeKind = (EAutoEvPtrType) 0,
		int Flags = 0
		)
	{
		return GetAutoEvPtrType (EType_AutoEvPtr, PtrTypeKind, Flags);
	}

	CStructType*
	GetVTableStructType ();

	static
	rtl::CStringA
	CreateSignature (
		CFunctionType* pStarterType,
		CFunctionType* pStopperType
		)
	{ 
		return rtl::CStringA::Format_s ("V%s%s", pStarterType->GetSignature (), pStopperType->GetSignature ()); 
	}

protected:
	virtual 
	void
	PrepareTypeString ()
	{
		m_TypeString.Format (_T("autoev %s"), m_pStarterType->GetArgString ());
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
