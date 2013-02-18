// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
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

	rtl::CStdListT <CStructMember> m_MemberList;

	CStructType* m_pStructType;

public:
	CUnionType ()
	{
		m_TypeKind = EType_Union;
		m_Flags = ETypeFlag_Pod;
		m_pStructType = NULL;
	}

	CStructType*
	GetStructType ()
	{
		ASSERT (m_pStructType);
		return m_pStructType;
	}

	rtl::CConstListT <CStructMember>
	GetMemberList ()
	{
		return m_MemberList;
	}

	CStructMember*
	FindMember (const tchar_t* pName)
	{
		rtl::CStringHashTableMapIteratorT <CModuleItem*> It = m_ItemMap.Find (pName);
		return It && It->m_Value->GetItemKind () == EModuleItem_StructMember ? (CStructMember*) It->m_Value : NULL;
	}

	CStructMember*
	CreateMember (
		const rtl::CString& Name,
		CType* pType,
		size_t BitCount = 0
		);

	virtual
	bool
	CalcLayout ();

protected:
	virtual 
	void
	PrepareTypeString ()
	{
		m_TypeString.Format (_T("union %s"), m_Tag);
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
