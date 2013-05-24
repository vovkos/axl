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
	friend class CTypeMgr;

protected:
	CStructType* m_pPtrStructType;
	CDataPtrType* m_PtrTypeArray [2] [EDataPtrType__Count] [2] [2] [2]; // ref x ptrkind x const x volatile x nullable

public:
	CDataPtrTypeTuple ()
	{
		memset (this, 0, sizeof (CDataPtrTypeTuple));
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
