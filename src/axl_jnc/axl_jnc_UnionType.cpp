#include "pch.h"
#include "axl_jnc_UnionType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CUnionType::CUnionType ()
{
	m_TypeKind = EType_Union;
	m_Flags = ETypeFlag_Pod | ETypeFlag_Moveable;
	m_pStructType = NULL;
	m_pInitializedField = NULL;
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
	pType = m_pModule->m_TypeMgr.PrepareDataType (pType);

	if (!(pType->GetFlags () & ETypeFlag_Pod))
	{
		err::SetFormatStringError ("non-POD '%s' cannot be union member", pType->GetTypeString ().cc ());
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
			err::SetFormatStringError (
				"'%s' already has initialized field '%s'", 
				pType->GetTypeString ().cc (), 
				m_pInitializedField->GetName ().cc ()
				);
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

CStructField*
CUnionType::GetFieldByIndex (size_t Index)
{
	size_t Count = m_FieldList.GetCount ();
	if (Index >= Count)
	{
		err::SetFormatStringError ("index '%d' is out of bounds", Index);
		return NULL;
	}

	if (m_FieldArray.GetCount () != Count)
	{
		m_FieldArray.SetCount (Count);
		rtl::CIteratorT <CStructField> Field = m_FieldList.GetHead ();
		for (size_t i = 0; i < Count; i++, Field++)
			m_FieldArray [i] = *Field;	
	}

	return m_FieldArray [Index];
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
	m_pStructType->m_Tag.Format ("%s.struct", m_Tag.cc ());
	m_pStructType->CreateField (pLargestMemberType);
	Result = m_pStructType->CalcLayout ();
	if (!Result)
		return false;

	if (!m_pConstructor && m_pInitializedField)
	{
		Result = CreateDefaultConstructor ();
		if (!Result)
			return false;
	}

	PostCalcLayout ();
	return true;
}

bool
CUnionType::InitializeField ()
{
	ASSERT (m_pInitializedField);

	CValue FieldValue;
	return 
		m_pModule->m_OperatorMgr.GetField (m_pInitializedField, NULL, &FieldValue) &&
		m_pModule->m_OperatorMgr.ParseInitializer (FieldValue, m_pInitializedField->GetInitializer ());
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
