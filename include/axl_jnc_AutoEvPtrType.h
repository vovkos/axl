// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_AutoEvType.h"
#include "axl_jnc_FunctionPtrType.h"

namespace axl {
namespace jnc {

//.............................................................................

class CAutoEvPtrType: public CType
{
	friend class CTypeMgr;

protected:
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
	rtl::CString
	CreateSignature (
		CAutoEvType* pAutoEvType,
		EType TypeKind,
		EAutoEvPtrType PtrTypeKind,
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

class CAutoEvPtrTypeTuple: public rtl::TListLink
{
	friend class CTypeMgr;

protected:
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

