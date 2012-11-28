#include "stdafx.h"
#include "axl_jnc_ClassType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

bool
CClassType::FindBaseType (
	CClassType* pType,
	size_t* pOffset,
	rtl::CArrayT <size_t>* pLlvmIndexArray,
	size_t* pMethodTableIndex
	)
{
	rtl::CStringHashTableMapIteratorAT <CClassBaseType*> It = m_BaseTypeMap.Find (pType->GetSignature ());
	if (It)
	{
		CClassBaseType* pBaseType = It->m_Value;

		if (pOffset)
			*pOffset = pBaseType->m_Offset;

		if (pLlvmIndexArray)
			pLlvmIndexArray->Copy (&pBaseType->m_LlvmIndex, 1);

		if (pMethodTableIndex)
			*pMethodTableIndex = pBaseType->m_MethodTableIndex;

		return true;
	}

	char Buffer [256];
	rtl::CArrayT <size_t> LlvmIndexArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));

	rtl::CIteratorT <CClassBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		size_t Offset;
		size_t MethodTableIndex;

		bool Result = BaseType->m_pType->FindBaseType (pType, &Offset, &LlvmIndexArray, &MethodTableIndex);
		if (Result)
		{
			if (pOffset)
				*pOffset = BaseType->m_Offset + Offset;

			if (pMethodTableIndex)
				*pMethodTableIndex = BaseType->m_MethodTableIndex + MethodTableIndex;

			if (pLlvmIndexArray)
			{
				pLlvmIndexArray->Clear ();
				pLlvmIndexArray->Append (BaseType->m_LlvmIndex);
				pLlvmIndexArray->Append (LlvmIndexArray);
			}

			return true;
		}
	}

	return false;
}

CClassBaseType*
CClassType::AddBaseType (CClassType* pType)
{
	rtl::CStringHashTableMapIteratorAT <CClassBaseType*> It = m_BaseTypeMap.Goto (pType->GetSignature ());
	if (It->m_Value)
	{
		err::SetFormatStringError (_T("'%s' is already a base type"), pType->GetTypeString ());
		return NULL;
	}

	CClassBaseType* pBaseType = AXL_MEM_NEW (CClassBaseType);
	pBaseType->m_pType = pType;
	m_BaseTypeList.InsertTail (pBaseType);
	It->m_Value = pBaseType;
	return pBaseType;
}

CClassMember*
CClassType::FindMember (
	const tchar_t* pName,
	size_t* pBaseTypeOffset,
	rtl::CArrayT <size_t>* pLlvmBaseTypeIndexArray,
	size_t* pBaseTypeMethodTableIndex
	)
{
	rtl::CStringHashTableMapIteratorT <CModuleItem*> It = m_ItemMap.Find (pName);
	if (It)
	{
		CModuleItem* pItem = It->m_Value;
		return pItem->GetItemKind () == EModuleItem_ClassMember ? (CClassMember*) pItem : NULL;
	}

	char Buffer [256];
	rtl::CArrayT <size_t> LlvmBaseTypeIndexArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));

	rtl::CIteratorT <CClassBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		size_t BaseTypeOffset;
		size_t BaseTypeMethodTableIndex;

		CClassMember* pMember = BaseType->m_pType->FindMember (
			pName, 
			&BaseTypeOffset, 
			&LlvmBaseTypeIndexArray,
			&BaseTypeMethodTableIndex
			);

		if (pMember)
		{
			if (pBaseTypeOffset)
				*pBaseTypeOffset = BaseType->m_Offset + BaseTypeOffset;

			if (pBaseTypeMethodTableIndex)
				*pBaseTypeMethodTableIndex = BaseType->m_MethodTableIndex + BaseTypeMethodTableIndex;

			if (pLlvmBaseTypeIndexArray)
			{
				pLlvmBaseTypeIndexArray->Clear ();
				pLlvmBaseTypeIndexArray->Append (BaseType->m_LlvmIndex);
				pLlvmBaseTypeIndexArray->Append (LlvmBaseTypeIndexArray);	
			}

			return pMember;
		}
	}

	return NULL;
}

CClassFieldMember*
CClassType::CreateFieldMember (
	const rtl::CString& Name,
	CType* pType,
	size_t BitCount
	)
{
	CClassFieldMember* pMember = AXL_MEM_NEW (CClassFieldMember);
	pMember->m_Name = Name;
	pMember->m_pType = pType;
	pMember->m_pBitFieldBaseType = BitCount ? pType : NULL;
	pMember->m_BitCount = BitCount;
	m_FieldMemberList.InsertTail (pMember);

	bool Result = AddItem (pMember);
	if (!Result)
		return NULL;

	return pMember;
}

CClassMethod*
CClassType::CreateMethod (
	const rtl::CString& Name,
	CFunction* pFunction
	)
{
	CClassMethodMember* pMethodMember = NULL;

	rtl::CStringHashTableMapIteratorT <CModuleItem*> It = m_ItemMap.Goto (Name);
	if (!It->m_Value)
	{
		pMethodMember = AXL_MEM_NEW (CClassMethodMember);
		pMethodMember->m_Name = Name;
		m_MethodMemberList.InsertTail (pMethodMember);
	}
	else
	{
		CModuleItem* pItem = It->m_Value;
		if (pItem->GetItemKind () == EModuleItem_ClassMember)
		{
			CClassMember* pMember = (CClassMember*) pItem;
			if (pMember->GetMemberKind () == EClassMember_Method)
				pMethodMember = (CClassMethodMember*) pMember;
		}

		if (!pMethodMember)
		{
			err::SetFormatStringError (_T("redefinition of '%s'"), Name);
			return NULL;
		}
	}

	ASSERT (pMethodMember);

	size_t MethodTableIndex = m_MethodTable.GetCount ();
	m_MethodTable.SetCount (MethodTableIndex + 1);

	CClassMethod* pMethod = AXL_MEM_NEW (CClassMethod);
	pMethod->m_pMethodMember = pMethodMember;
	pMethod->m_pFunction = pFunction;
	pMethod->m_MethodTableIndex = MethodTableIndex;
	
	pMethodMember->m_OverloadList.InsertTail (pMethod);

	return pMethod;
}

CClassPropertyMember*
CClassType::CreatePropertyMember (
	const rtl::CString& Name,
	CProperty* pProperty
	)
{
	size_t SetterCount = pProperty->GetSetter ()->GetOverloadCount ();
	size_t MethodTableIndex = m_MethodTable.GetCount ();
	m_MethodTable.SetCount (MethodTableIndex + 1 + SetterCount);

	CClassPropertyMember* pMember = AXL_MEM_NEW (CClassPropertyMember);
	pMember->m_Name = Name;
	pMember->m_pProperty = pProperty;
	pMember->m_MethodTableIndex = MethodTableIndex;
	m_PropertyMemberList.InsertTail (pMember);

	bool Result = AddItem (pMember);
	if (!Result)
		return NULL;

	return pMember;
}

bool
CClassType::CalcLayout ()
{
	bool Result;

	size_t Offset;
	size_t LlvmIndex;

	ResetLayout ();

	// layout as EType_Interface 

	CType* pObjectHdrType = m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectHdr);
	LayoutField (pObjectHdrType->GetPointerType (EType_Pointer_u), &Offset, &LlvmIndex); // TObject* m_pObject;
	LayoutField (m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &Offset, &LlvmIndex); // void** m_pMethodTable;

	size_t MethodCount = 0;

	rtl::CIteratorT <CClassBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		CClassBaseType* pBaseType = *BaseType;

		Result = LayoutField (
			pBaseType->m_pType->GetLlvmInterfaceType (),
			pBaseType->m_pType->GetFieldActualSize (),
			pBaseType->m_pType->GetAlignFactor (),
			&pBaseType->m_Offset,
			&pBaseType->m_LlvmIndex
			);

		if (!Result)
			return false;

		pBaseType->m_MethodTableIndex = MethodCount;
		MethodCount += pBaseType->m_pType->m_MethodTable.GetCount ();
	}

	rtl::CIteratorT <CClassFieldMember> Member = m_FieldMemberList.GetHead ();
	for (; Member; Member++)
	{
		CClassFieldMember* pMember = *Member;

		Result = pMember->m_BitCount ? 
			LayoutBitField (
				pMember->m_pBitFieldBaseType,
				pMember->m_BitCount,
				&pMember->m_pType,
				&pMember->m_Offset,
				&pMember->m_LlvmIndex
				) :
			LayoutField (
				pMember->m_pType,
				&pMember->m_Offset,
				&pMember->m_LlvmIndex
				);

			if (!Result)
				return false;
	}

	if (m_FieldAlignedSize > m_FieldActualSize)
		InsertPadding (m_FieldAlignedSize - m_FieldActualSize);

	return true;
}

llvm::Type* 
CClassType::GetLlvmType ()
{
	if (m_pLlvmType)
		return m_pLlvmType;

	llvm::StructType* pLlvmInterfaceType = GetLlvmInterfaceType ();

	llvm::Type* LlvmTypeArray [] =
	{
		llvm::PointerType::get (pLlvmInterfaceType, 0),                   // TInterfaceHdr*
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT)->GetLlvmType (),  // size_t m_Scope;
	};

	m_pLlvmType = llvm::StructType::get (
		llvm::getGlobalContext (), 
		llvm::ArrayRef <llvm::Type*> (LlvmTypeArray, countof (LlvmTypeArray)),
		true
		);

	return m_pLlvmType;
}

llvm::StructType*
CClassType::GetLlvmInterfaceType ()
{
	if (m_pLlvmInterfaceType)
		return m_pLlvmInterfaceType;
	
	m_pLlvmInterfaceType = GetLlvmStructType (GetQualifiedName ());
	return m_pLlvmInterfaceType;
}

llvm::StructType*
CClassType::GetLlvmClassType ()
{
	if (m_pLlvmClassType)
		return m_pLlvmClassType;

	llvm::StructType* pLlvmInterfaceType = GetLlvmInterfaceType ();

	llvm::Type* LlvmTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p)->GetLlvmType (),    // intptr_t m_Flags;
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr)->GetLlvmType (), // CClassType* m_pClass;
		pLlvmInterfaceType,
	};

	m_pLlvmClassType = llvm::StructType::get (
		llvm::getGlobalContext (), 
		llvm::ArrayRef <llvm::Type*> (LlvmTypeArray, countof (LlvmTypeArray)),
		true
		);

	return m_pLlvmClassType;
}

bool
CClassType::InitializeObject (
	TObjectHdr* pObject,
	int Flags
	)
{
	ASSERT (m_TypeKind == EType_Class);

	memset (pObject, 0, GetClassSize ());

	pObject->m_pType = this;
	pObject->m_Flags = Flags;
	InitializeInterface ((TInterfaceHdr*) (pObject + 1), pObject, m_MethodTable);
	return true;
}

bool
CClassType::InitializeInterface (
	TInterfaceHdr* pInterface,
	TObjectHdr* pObject,
	void** pMethodTable
	)
{
	pInterface->m_pObject = pObject;
	pInterface->m_pMethodTable = pMethodTable;

	rtl::CIteratorT <jnc::CClassBaseType> BaseType = GetFirstBaseType ();
	for (; BaseType; BaseType++)
	{		
		BaseType->m_pType->InitializeInterface (
			(TInterfaceHdr*) ((uchar_t*) pInterface + BaseType->m_Offset),
			pObject,
			pMethodTable + BaseType->m_MethodTableIndex
			);
	}

	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
