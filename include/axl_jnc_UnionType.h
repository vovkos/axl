// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_StructType.h"

namespace axl {
namespace jnc {

class CUnionType;

//.............................................................................

// union cannot be a child, but it can be a parent

class CUnionType: public CDerivableType 
{
protected:
	friend class CTypeMgr;
	friend class CParser;

	rtl::CStdListT <CStructField> m_FieldList;
	rtl::CArrayT <CStructField*> m_FieldArray;
	CStructField* m_pInitializedField;
	CStructType* m_pStructType;
	
public:
	CUnionType ();

	CStructType*
	GetStructType ()
	{
		ASSERT (m_pStructType);
		return m_pStructType;
	}

	rtl::CConstListT <CStructField>
	GetFieldList ()
	{
		return m_FieldList;
	}

	CStructField*
	CreateField (
		const rtl::CString& Name,
		CType* pType,
		size_t BitCount,
		int PtrTypeFlags,
		rtl::CBoxListT <CToken>* pInitializer = NULL
		);

	CStructField*
	CreateField (
		CType* pType,
		size_t BitCount = 0,
		int PtrTypeFlags = 0
		)
	{
		return CreateField (rtl::CString (), pType, BitCount, PtrTypeFlags);
	}

	CStructField*
	GetFieldByIndex (size_t Index);

	bool
	InitializeField ();

	virtual
	bool
	CalcLayout ();

protected:
	virtual 
	void
	PrepareTypeString ()
	{
		m_TypeString.Format ("union %s", m_Tag.cc ()); // thanks a lot gcc
	}

	virtual 
	void
	PrepareLlvmType ()
	{
		m_pLlvmType = GetStructType ()->GetLlvmType ();
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
