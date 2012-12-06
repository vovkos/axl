#include "stdafx.h"
#include "axl_jnc_ClassType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetClassMemberKindString (EClassMember MemberKind)
{
	switch (MemberKind)
	{
	case EClassMember_Field:
		return _T("class-field");

	case EClassMember_Method:
		return _T("class-method");

	case EClassMember_Property:
		return _T("class-property");

	case EClassMember_Constructor:
		return _T("class-constructor");

	case EClassMember_Destructor:
		return _T("class-destructor");

	default:
		return _T("undefined-class-member");
	}
}

//.............................................................................

CClassType::CClassType ()
{
	m_TypeKind = EType_Class;
	m_NamespaceKind = ENamespace_Class;
	m_PackFactor = 8;
	m_pInitializer = NULL;
	m_pFinalizer = NULL;
	m_pPointerStructType = NULL;
	m_pInterfaceStructType = NULL;
	m_pClassStructType = NULL;
	m_pVTableStructType = NULL;
}

bool
CClassType::FindBaseType (
	CClassType* pType,
	size_t* pOffset,
	rtl::CArrayT <size_t>* pLlvmIndexArray,
	size_t* pVTableIndex
	)
{
	rtl::CStringHashTableMapIteratorAT <CClassBaseType*> It = m_BaseTypeMap.Find (pType->GetSignature ());
	if (It)
	{
		CClassBaseType* pBaseType = It->m_Value;

		if (pOffset)
			*pOffset = pBaseType->GetOffset ();

		if (pLlvmIndexArray)
			pLlvmIndexArray->Copy (pBaseType->GetLlvmIndex ());

		if (pVTableIndex)
			*pVTableIndex = pBaseType->m_VTableIndex;

		return true;
	}

	char Buffer [256];
	rtl::CArrayT <size_t> LlvmIndexArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));

	rtl::CIteratorT <CClassBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		size_t Offset;
		size_t VTableIndex;

		bool Result = BaseType->m_pType->FindBaseType (pType, &Offset, &LlvmIndexArray, &VTableIndex);
		if (Result)
		{
			if (pOffset)
				*pOffset = BaseType->GetOffset () + Offset;

			if (pVTableIndex)
				*pVTableIndex = BaseType->m_VTableIndex + VTableIndex;

			if (pLlvmIndexArray)
			{
				pLlvmIndexArray->Clear ();
				pLlvmIndexArray->Append (BaseType->GetLlvmIndex ());
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

CModuleItem*
CClassType::FindItemWithBaseTypeList (const tchar_t* pName)
{
	rtl::CStringHashTableMapIteratorT <CModuleItem*> It = m_ItemMap.Find (pName);
	if (It)
		return It->m_Value;

	if (m_Name == pName)
		return this;

	rtl::CIteratorT <CClassBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		CClassType* pClassType = BaseType->m_pType;
		if (pClassType->m_Name == pName)
			return pClassType;

		CModuleItem* pItem = pClassType->FindItemWithBaseTypeList (pName);
		if (pItem)
			return pItem;
	}

	return NULL;
}

CClassMember*
CClassType::FindMemberImpl (
	bool ExcludeThis,
	const tchar_t* pName,
	size_t* pBaseTypeOffset,
	rtl::CArrayT <size_t>* pLlvmBaseTypeIndexArray,
	size_t* pBaseTypeVTableIndex
	)
{
	if (!ExcludeThis)
	{
		rtl::CStringHashTableMapIteratorT <CModuleItem*> It = m_ItemMap.Find (pName);
		if (It)
		{
			CModuleItem* pItem = It->m_Value;
			if (pItem->GetItemKind () != EModuleItem_ClassMember)
				return NULL;

			if (pBaseTypeOffset)
				*pBaseTypeOffset = 0;

			if (pLlvmBaseTypeIndexArray)
				pLlvmBaseTypeIndexArray->Clear ();

			if (pBaseTypeVTableIndex)
				*pBaseTypeVTableIndex = 0;

			return (CClassMember*) pItem;
		}
	}

	char Buffer [256];
	rtl::CArrayT <size_t> LlvmBaseTypeIndexArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));

	rtl::CIteratorT <CClassBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		size_t BaseTypeOffset;
		size_t BaseTypeVTableIndex;

		CClassMember* pMember = BaseType->m_pType->FindMember (
			pName, 
			&BaseTypeOffset, 
			&LlvmBaseTypeIndexArray,
			&BaseTypeVTableIndex
			);

		if (pMember)
		{
			if (pBaseTypeOffset)
				*pBaseTypeOffset = BaseType->GetOffset () + BaseTypeOffset;

			if (pBaseTypeVTableIndex)
				*pBaseTypeVTableIndex = BaseType->m_VTableIndex + BaseTypeVTableIndex;

			if (pLlvmBaseTypeIndexArray)
			{
				pLlvmBaseTypeIndexArray->Clear ();
				pLlvmBaseTypeIndexArray->Append (BaseType->GetLlvmIndex ());
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

CClassMethodMember*
CClassType::CreateMethodMember (
	const rtl::CString& Name,
	CFunction* pFunction
	)
{
	AddMethodFunction (pFunction);

	CClassMethodMember* pMethodMember = NULL;

	rtl::CStringHashTableMapIteratorT <CModuleItem*> It = m_ItemMap.Goto (Name);
	if (!It->m_Value)
	{
		pMethodMember = AXL_MEM_NEW (CClassMethodMember);
		pMethodMember->m_Name = Name;
		pMethodMember->m_pParentNamespace = this;
		m_MethodMemberList.InsertTail (pMethodMember);
		It->m_Value = pMethodMember;
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

	bool Result = pMethodMember->AddOverload (pFunction);
	if (!Result)
		return NULL;

	return pMethodMember;
}

void
CClassType::AddMethodFunction (CFunction* pFunction)
{
	CFunctionType* pType = pFunction->GetType ();
	
	rtl::CArrayT <CType*> ArgTypeArray = pType->GetArgTypeArray ();
	ArgTypeArray.Insert (0, this);
	CFunctionType* pFullType = m_pModule->m_TypeMgr.GetFunctionType (pType->GetReturnType (), ArgTypeArray, pType->GetFlags ());

	pFunction->m_FunctionKind = EFunction_Method;
	pFunction->m_pClassType = this;
	pFunction->m_pClosureType = pType;
	pFunction->m_pType = pFullType;

	m_MethodFunctionArray.Append (pFunction);
}

CClassPropertyMember*
CClassType::CreatePropertyMember (
	const rtl::CString& Name,
	CPropertyType* pType
	)
{
	CClassPropertyMember* pMember = AXL_MEM_NEW (CClassPropertyMember);
	pMember->m_Name = Name;
	pMember->m_pType = pType;
	m_PropertyMemberList.InsertTail (pMember);

	bool Result = AddItem (pMember);
	if (!Result)
		return NULL;

	pType->m_pParentNamespace = this;
	pType->m_Name = Name;
	pType->TagAccessors ();
	return pMember;
}

bool
CClassType::CalcLayout ()
{
	if (m_Flags & ETypeFlag_IsLayoutReady)
		return true;

	bool Result = PreCalcLayout ();
	if (!Result)
		return false;

	m_pVTableStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	m_pVTableStructType->m_Tag.Format (_T("%s.vtbl"), m_Tag);

	m_pInterfaceHdrStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType (m_PackFactor);
	m_pInterfaceHdrStructType->m_Tag.Format (_T("%s.iface.hdr"), m_Tag);
	m_pInterfaceHdrStructType->CreateMember (m_pVTableStructType->GetPointerType (EType_Pointer_u));
	m_pInterfaceHdrStructType->CreateMember (m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectHdr)->GetPointerType (EType_Pointer_u));

	m_pInterfaceStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType (m_PackFactor);
	m_pInterfaceStructType->m_Tag.Format (_T("%s.iface"), m_Tag);
	m_pInterfaceStructType->AddBaseType (m_pInterfaceHdrStructType);

	m_pPointerStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	m_pPointerStructType->m_Tag.Format (_T("%s.ptr"), m_Tag);
	m_pPointerStructType->CreateMember (GetInterfaceStructType ()->GetPointerType (EType_Pointer_u));
	m_pPointerStructType->CreateMember (m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT));
	m_pPointerStructType->CalcLayout ();

	// lay out base types

	rtl::CIteratorT <CClassBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		CClassType* pBaseClassType = BaseType->m_pType;

		Result = pBaseClassType->CalcLayout ();
		if (!Result)
			return false;
				
		BaseType->m_pStructBaseType = m_pInterfaceStructType->AddBaseType (pBaseClassType->GetInterfaceStructType ());
		BaseType->m_VTableIndex = m_VTable.GetCount ();

		m_VTable.Append (pBaseClassType->m_VTable);

		rtl::CIteratorT <CStructMember> VTableMember = pBaseClassType->GetVTableStructType ()->GetFirstMember ();
		for (; VTableMember; VTableMember++)		
			m_pVTableStructType->CreateMember (VTableMember->GetType ());
	}

	// lay out properties

	rtl::CIteratorT <CClassPropertyMember> PropertyMember = m_PropertyMemberList.GetHead ();
	for (; PropertyMember; PropertyMember++)
	{
		CPropertyType* pPropertyType = PropertyMember->m_pType;

		Result = pPropertyType->CalcLayout ();
		if (!Result)
			return false;

		pPropertyType->m_ParentVTableIndex = m_VTable.GetCount ();

		m_VTable.Append (pPropertyType->m_VTable);

		rtl::CIteratorT <CStructMember> VTableMember = pPropertyType->GetVTableStructType ()->GetFirstMember ();
		for (; VTableMember; VTableMember++)		
			m_pVTableStructType->CreateMember (VTableMember->GetType ());
	}

	// lay out methods

	size_t Count = m_MethodFunctionArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunction* pFunction = m_MethodFunctionArray [i];
		Result = LayoutFunction (pFunction);
		if (!Result)
			return false;
	}

	m_pVTableStructType->CalcLayout ();

	// lay out fields

	rtl::CIteratorT <CClassFieldMember> FieldMember = m_FieldMemberList.GetHead ();
	for (; FieldMember; FieldMember++)
		FieldMember->m_pStructMember = m_pInterfaceStructType->CreateMember (FieldMember->m_pType, FieldMember->m_BitCount);

	m_pInterfaceStructType->CalcLayout ();

	if (m_TypeKind == EType_Class)
	{
		m_pClassStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType (m_PackFactor);
		m_pClassStructType->m_Tag.Format (_T("%s.class"), m_Tag);
		m_pClassStructType->CreateMember (m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectHdr));
		m_pClassStructType->CreateMember (m_pInterfaceStructType);
		m_pClassStructType->CalcLayout ();
	}

	PostCalcLayout ();

	return true;
}

bool
CClassType::LayoutFunction (CFunction* pFunction)
{
	#pragma AXL_TODO ("deal with virtual overloads and multipliers")

	if (pFunction->m_PropertyAccessorKind && pFunction->m_Name.IsEmpty ()) // property declared within this class
	{
		AddFunctionToVTable (pFunction);
		return true;
	}

	CFunction* pOverridenFunction = NULL;
	size_t BaseTypeVTableIndex = -1;

	CClassMember* pMember = FindMemberImpl (
		!pFunction->m_PropertyAccessorKind, // exclude 'this' if its not an accessor
		pFunction->m_Name.GetShortName (),
		NULL,
		NULL,
		&BaseTypeVTableIndex
		);

	CFunctionType* pClosureType = pFunction->GetClosureType ();

	if (pFunction->m_PropertyAccessorKind)
	{
		if (pMember && pMember->GetMemberKind () == EClassMember_Property)
		{
			CClassPropertyMember* pPropertyMember = (CClassPropertyMember*) pMember;
			CPropertyType* pPropertyType = pPropertyMember->GetType ();

			pOverridenFunction = pFunction->m_PropertyAccessorKind == EPropertyAccessor_Set ? 
				pPropertyType->GetSetter ()->FindOverload (pClosureType, true) : 
				pPropertyType->GetGetter ()->GetClosureType ()->Cmp (pClosureType) == 0 ? pPropertyType->GetGetter () : 
				NULL;
		}

		if (!pOverridenFunction)
		{
			err::SetFormatStringError (_T("orphaned property accessor '%s'"), pFunction->m_Tag);
			return false;
		}
	}
	else
	{
		if (pMember && pMember->GetMemberKind () == EClassMember_Method)
		{
			CClassMethodMember* pMethodMember = (CClassMethodMember*) pMember;
			pOverridenFunction = pMethodMember->FindOverload (pFunction->GetClosureType (), true);
		}

		if (!pOverridenFunction)
		{
			AddFunctionToVTable (pFunction);
			return true;
		}
	}

	size_t VTableIndex = BaseTypeVTableIndex + pOverridenFunction->GetVTableIndex ();
	pFunction->m_pType = pOverridenFunction->m_pType;
	pFunction->m_VTableIndex = VTableIndex;
	pFunction->m_pVTableType = pOverridenFunction->m_pVTableType;
	m_VTable [pFunction->m_VTableIndex] = pFunction;
	return true;
}

CFunction* 
CClassType::GetInitializer ()
{
	if (m_pInitializer)
		return m_pInitializer;

	m_pInitializer = m_pModule->m_FunctionMgr.CreateClassInitializer (this);
	return m_pInitializer;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
