#include "stdafx.h"
#include "axl_jnc_UnionType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CUnionType::CUnionType ()
{
	m_TypeKind = EType_Union;
	m_Flags = ETypeFlag_Pod;
	m_pStructType = NULL;
	m_pInitializedField = NULL;
	m_pPreConstructor = NULL;
	m_pConstructor = NULL;
	m_pStaticConstructor = NULL;
}

CStructField*
CUnionType::CreateField (
	const rtl::CString& Name,
	CType* pType,
	size_t BitCount,
	int PtrTypeFlags,
	rtl::CBoxListT <CToken>* pInitializer
	)
{
	if (!(pType->GetFlags () & ETypeFlag_Pod))
	{
		err::SetFormatStringError (_T("non-POD '%s' cannot be union member"), pType->GetTypeString ());
		return NULL;
	}

	CStructField* pField = AXL_MEM_NEW (CStructField);
	pField->m_Name = Name;
	pField->m_pParentType = this;
	pField->m_pType = pType;
	pField->m_PtrTypeFlags = PtrTypeFlags;
	pField->m_pBitFieldBaseType = BitCount ? pType : NULL;
	pField->m_BitCount = BitCount;

	if (pInitializer)
	{
		if (m_pInitializedField)
		{
			err::SetFormatStringError (_T("'%s' already has initialized field '%s'"), pType->GetTypeString (), m_pInitializedField->GetName ());
			return NULL;
		}

		pField->m_Initializer.TakeOver (pInitializer);		
		m_pInitializedField = pField;
	}

	m_FieldList.InsertTail (pField);

	if (!Name.IsEmpty ())
	{
		bool Result = AddItem (pField);
		if (!Result)
			return NULL;
	}

	return pField;
}

bool
CUnionType::CalcLayout ()
{
	if (m_Flags & ETypeFlag_LayoutReady)
		return true;

	bool Result = PreCalcLayout ();
	if (!Result)
		return false;

	if (m_pExtensionNamespace)
		ApplyExtensionNamespace ();

	CType* pLargestMemberType = NULL;

	rtl::CIteratorT <CStructField> Member = m_FieldList.GetHead ();
	for (; Member; Member++)
	{
		CStructField* pMember = *Member;

		Result = pMember->m_pType->CalcLayout ();
		if (!Result)
			return false;

		if (pMember->m_BitCount)
		{
			pMember->m_pType = m_pModule->m_TypeMgr.GetBitFieldType (pMember->m_pBitFieldBaseType, 0, pMember->m_BitCount);
			if (!pMember->m_pType)
				return false;
		}

		if (!pLargestMemberType || pMember->m_pType->GetSize () > pLargestMemberType->GetSize ())
			pLargestMemberType = pMember->m_pType;
	}

	ASSERT (pLargestMemberType);

	m_pStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	m_pStructType->m_Tag.Format (_T("%s.struct"), m_Tag);
	m_pStructType->CreateField (pLargestMemberType);
	Result = m_pStructType->CalcLayout ();
	if (!Result)
		return false;

	PostCalcLayout ();
	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {

