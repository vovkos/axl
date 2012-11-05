#include "stdafx.h"
#include "axl_jnc_StructType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

bool
CStructClassType::AddBaseType (CType* pType)
{
	if (m_BaseTypeArray.Find (pType) != -1)
	{
		err::SetFormatStringError (_T("'%s' is already a base type"), pType->GetTypeString ());
		return false;
	}

	m_BaseTypeArray.Append (pType);
	return true;
}

bool
CStructClassType::AddGenericArgument (CImportType* pType)
{
	if (m_GenericArgumentArray.Find (pType) != -1)
	{
		err::SetFormatStringError (_T("multiple generic argument names '%s'"), pType->GetTypeString ());
		return false;
	}

	m_GenericArgumentArray.Append (pType);
	return true;
}

//.............................................................................

CStructMember::CStructMember ()
{
	m_ItemKind = EModuleItem_StructMember;
	m_pType = NULL;
	m_Offset = 0;
	m_Index = 0;
	m_LlvmIndex = 0;
}

//.............................................................................

CStructMember*
CStructType::FindMember (
	const tchar_t* pName,
	bool Traverse
	)
{
	rtl::CHashTableMapIteratorT <const tchar_t*, CModuleItem*> It = m_ItemMap.Find (pName); 
	if (It)
		return (CStructMember*) It->m_Value;

	if (!Traverse)
		return NULL;

	size_t Count = m_BaseTypeArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CType* pBaseType = m_BaseTypeArray [i];
		EType TypeKind = pBaseType->GetTypeKind ();

		CStructMember* pMember = NULL;

		switch (TypeKind)
		{
		case EType_Struct:
		case EType_Union:
			pMember = ((CStructType*) pBaseType)->FindMember (pName, true);
			break;
		}

		if (pMember)
			return pMember;
	}

	return NULL;
}

CStructMember*
CStructType::CreateMember (
	const rtl::CString& Name,
	CType* pType
	)
{
	size_t Offset = m_ActualSize;
	size_t AlignFactor = pType->GetAlignFactor ();

	if (AlignFactor > m_PackFactor)
		AlignFactor = m_PackFactor;

	size_t Mod = Offset % AlignFactor;
	if (Mod)
		Offset += AlignFactor - Mod;

	CStructMember* pMember = AXL_MEM_NEW (CStructMember);
	pMember->m_Name = Name;
	pMember->m_pType = pType;
	pMember->m_Offset = Offset;
	m_MemberList.InsertTail (pMember);

	bool Result = AddItem (pMember);
	if (!Result)
		return NULL;

	m_ActualSize = Offset + pType->GetSize ();
	m_Size = m_ActualSize;

	if (AlignFactor > m_AlignFactor)
		m_AlignFactor = AlignFactor;

	Mod = m_Size % m_AlignFactor;
	if (Mod)
		m_Size += m_AlignFactor - Mod;

	return pMember;
}

llvm::StructType* 
CStructType::GetLlvmType ()
{
	if (m_Flags & ETypeFlag_IsLlvmReady)
		return (llvm::StructType*) m_pLlvmType;
	
	char Buffer [256];
	rtl::CArrayT <llvm::Type*> LlvmMemberTypeArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	LlvmMemberTypeArray.Reserve (m_MemberList.GetCount ()); // could be more, if padding fields are inserted
	
	size_t Offset = 0;
	size_t LlvmIndex = 0;

	rtl::CIteratorT <CStructMember> Member = m_MemberList.GetHead ();
	for (; Member; Member++)
	{
		CStructMember* pMember = *Member;

		if (pMember->m_Offset > Offset) // insert padding
		{
			CArrayType* pPaddingArrayType = m_pModule->m_TypeMgr.GetArrayType (EType_Int8_u, pMember->m_Offset - Offset);
			LlvmMemberTypeArray.Append (pPaddingArrayType->GetLlvmType ());
			Offset = pMember->m_Offset;
			LlvmIndex++;
		}

		CType* pType = pMember->GetType ();
		LlvmMemberTypeArray.Append (pType->GetLlvmType ());
		Offset = pMember->m_Offset + pType->GetSize ();
		pMember->m_LlvmIndex = LlvmIndex++;
	}

	ASSERT (Offset == m_ActualSize);

	if (m_Size > m_ActualSize) // insert padding
	{
		CArrayType* pPaddingArrayType = m_pModule->m_TypeMgr.GetArrayType (EType_Int8_u, m_Size - m_ActualSize);
		LlvmMemberTypeArray.Append (pPaddingArrayType->GetLlvmType ());
	}

	llvm::StructType* pLlvmType = llvm::StructType::create (
		llvm::getGlobalContext (),
		llvm::ArrayRef<llvm::Type*> (LlvmMemberTypeArray, LlvmMemberTypeArray.GetCount ()),
		(const tchar_t*) GetQualifiedName (), 
		true
		);

	m_pLlvmType = pLlvmType;
	m_Flags |= ETypeFlag_IsLlvmReady;
	return pLlvmType;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {

