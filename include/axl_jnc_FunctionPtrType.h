// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_FunctionType.h"

namespace axl {
namespace jnc {

class CMulticastType;
class CMcSnapshotType;

//.............................................................................

class CFunctionPtrType: public CType
{
protected:
	friend class CTypeMgr;

	EFunctionPtrType m_PtrTypeKind;
	CFunctionType* m_pTargetType;
	CMulticastType* m_pMulticastType;
	CMcSnapshotType* m_pMcSnapshotType;
	rtl::CString m_TypeModifierString;

public:
	CFunctionPtrType ();

	EFunctionPtrType
	GetPtrTypeKind ()
	{
		return m_PtrTypeKind;
	}

	CFunctionType* 
	GetTargetType ()
	{
		return m_pTargetType;
	}

	bool
	HasClosure ()
	{
		return m_PtrTypeKind == EFunctionPtrType_Normal || m_PtrTypeKind == EFunctionPtrType_Weak;
	}

	CMulticastType* 
	GetMulticastType ();

	CMcSnapshotType* 
	GetMcSnapshotType ();

	CStructType* 
	GetFunctionPtrStructType ();

	CStructType* 
	GetFunctionPtrStructType_w ();

	rtl::CString
	GetTypeModifierString ();

	static
	rtl::CString
	CreateSignature (
		CFunctionType* pFunctionType,
		EType TypeKind,
		EFunctionPtrType PtrTypeKind,
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

class CFunctionPtrTypeTuple: public rtl::TListLink
{
protected:
	friend class CTypeMgr;

	CStructType* m_pPtrStructType;
	CStructType* m_pPtrStructType_w;
	CFunctionPtrType* m_PtrTypeArray [2] [EFunctionPtrType__Count] [2]; // ref x kind x nullable

public:
	CFunctionPtrTypeTuple ()
	{
		memset (this, 0, sizeof (CFunctionPtrTypeTuple));
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
