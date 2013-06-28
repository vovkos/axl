#include "pch.h"
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
}

CStructField*
CUnionType::CreateFieldImpl (
	const rtl::CString& Name,
	CType* pType,
	size_t BitCount,
	uint_t PtrTypeFlags,
	rtl::CBoxListT <CToken>* pConstructor,
	rtl::CBoxListT <CToken>* pInitializer
	)
{
	if (!(pType->GetFlags () & ETypeFlag_Pod))
	{
		err::SetFormatStringError ("non-POD '%s' cannot be union member", pType->GetTypeString ().cc ());
		return NULL;
	}

	CStructField* pField = AXL_MEM_NEW (CStructField);
	pField->m_Name = Name;
	pField->m_pParentNamespace = this;
	pField->m_pType = pType;
	pField->m_PtrTypeFlags = PtrTypeFlags;
	pField->m_pBitFieldBaseType = BitCount ? pType : NULL;
	pField->m_BitCount = BitCount;

	if (pConstructor)
		pField->m_Constructor.TakeOver (pConstructor);

	if (pInitializer)
		pField->m_Initializer.TakeOver (pInitializer);

	if (!pField->m_Constructor.IsEmpty () || 
		!pField->m_Initializer.IsEmpty ())
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
	bool Result;

	if (m_pExtensionNamespace)
		ApplyExtensionNamespace ();

	Result = ResolveImportFields ();
	if (!Result)
		return false;

	CType* pLargestMemberType = NULL;

	rtl::CIteratorT <CStructField> Member = m_FieldList.GetHead ();
	for (; Member; Member++)
	{
		CStructField* pMember = *Member;

		Result = pMember->m_pType->EnsureLayout ();
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

	m_pStructType->CreateField (pLargestMemberType);
	Result = m_pStructType->EnsureLayout ();
	if (!Result)
		return false;

	if (!m_pPreConstructor && m_pInitializedField)
	{
		Result = CreateDefaultMemberMethod (EFunction_PreConstructor);
		if (!Result)
			return false;
	}

	if (!m_pConstructor && m_pPreConstructor)
	{
		Result = CreateDefaultMemberMethod (EFunction_Constructor);
		if (!Result)
			return false;
	}

	m_Size = m_pStructType->GetSize ();
	m_AlignFactor = m_pStructType->GetAlignFactor ();
	return true;
}

bool
CUnionType::Compile ()
{
	bool Result;

	if (m_pPreConstructor && !(m_pPreConstructor->GetFlags () & EModuleItemFlag_User))
	{
		Result = CompileDefaultPreConstructor ();
		if (!Result)
			return false;
	}

	if (m_pConstructor && !(m_pConstructor->GetFlags () & EModuleItemFlag_User))
	{
		Result = CompileDefaultConstructor ();
		if (!Result)
			return false;
	}

	return true;
}

bool
CUnionType::CompileDefaultPreConstructor ()
{
	ASSERT (m_pPreConstructor);

	bool Result;

	CValue ThisValue;
	m_pModule->m_FunctionMgr.InternalPrologue (m_pPreConstructor, &ThisValue, 1);

	Result = InitializeField (ThisValue);
	if (!Result)
		return false;

	m_pModule->m_FunctionMgr.InternalEpilogue ();
	return true;
}


bool
CUnionType::InitializeField (const CValue& ThisValue)
{
	ASSERT (m_pInitializedField);

	CValue FieldValue;
	return 
		m_pModule->m_OperatorMgr.GetField (ThisValue, m_pInitializedField, NULL, &FieldValue) &&
		m_pModule->m_OperatorMgr.ParseInitializer (
			FieldValue, 
			m_pInitializedField->m_Constructor,
			m_pInitializedField->m_Initializer
			);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
