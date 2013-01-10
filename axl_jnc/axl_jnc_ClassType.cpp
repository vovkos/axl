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
	m_pPreConstructor = NULL;
	m_pDestructor = NULL;
	m_pInitializer = NULL;
	m_pInterfaceStructType = NULL;
	m_pClassStructType = NULL;
	m_pVTableStructType = NULL;
	m_pSimpleMethodType = NULL;
}

CFunctionType* 
CClassType::GetSimpleMethodType ()
{
	if (m_pSimpleMethodType)
		return m_pSimpleMethodType;

	CType* ArgTypeArray [] =
	{
		this
	};

	m_pSimpleMethodType = m_pModule->m_TypeMgr.GetFunctionType (
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Void),
		ArgTypeArray,
		countof (ArgTypeArray)
		);

	return m_pSimpleMethodType;
}

void
CClassType::SetAutoEvBody (rtl::CBoxListT <CToken>* pTokenList)
{
	m_AutoEvBody.TakeOver (pTokenList);
	m_pModule->m_FunctionMgr.m_GlobalAutoEvTypeArray.Append (this);
}

bool
CClassType::FindBaseTypeImpl (
	CClassType* pType,
	CClassBaseTypeCoord* pCoord,
	size_t Level
	)
{
	rtl::CStringHashTableMapIteratorAT <CClassBaseType*> It = m_BaseTypeMap.Find (pType->GetSignature ());
	if (It)
	{
		if (pCoord)
		{
			CClassBaseType* pBaseType = It->m_Value;

			pCoord->m_FieldCoord.m_LlvmIndexArray.SetCount (Level + 1);
			pCoord->m_FieldCoord.m_Offset = pBaseType->m_pFieldBaseType->GetOffset ();
			pCoord->m_FieldCoord.m_LlvmIndexArray [Level] = pBaseType->m_pFieldBaseType->GetLlvmIndex ();
			pCoord->m_VTableIndex = pBaseType->m_VTableIndex;
		}

		return true;
	}

	rtl::CIteratorT <CClassBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		CClassBaseType* pBaseType = *BaseType;

		bool Result = pBaseType->m_pType->FindBaseType (pType, pCoord);
		if (Result)
		{
			if (pCoord)
			{
				pCoord->m_FieldCoord.m_Offset += pBaseType->m_pFieldBaseType->GetOffset ();
				pCoord->m_FieldCoord.m_LlvmIndexArray [Level] = pBaseType->m_pFieldBaseType->GetLlvmIndex ();
				pCoord->m_VTableIndex += pBaseType->m_VTableIndex;
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
	bool IncludeThis,
	const tchar_t* pName,
	CClassBaseTypeCoord* pBaseTypeCoord,
	size_t Level
	)
{
	if (IncludeThis)
	{
		rtl::CStringHashTableMapIteratorT <CModuleItem*> It = m_ItemMap.Find (pName);
		if (It)
		{
			CModuleItem* pItem = It->m_Value;
			if (pItem->GetItemKind () != EModuleItem_ClassMember)
				return NULL;
			
			if (pBaseTypeCoord && Level)
				pBaseTypeCoord->m_FieldCoord.m_LlvmIndexArray.SetCount (Level);

			return (CClassMember*) pItem;
		}
	}
	
	rtl::CIteratorT <CClassBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		CClassBaseType* pBaseType = *BaseType;
		CClassMember* pMember = pBaseType->m_pType->FindMemberImpl (true, pName, pBaseTypeCoord, Level + 1);
		if (pMember)
		{
			if (pBaseTypeCoord)
			{
				pBaseTypeCoord->m_FieldCoord.m_Offset += pBaseType->m_pFieldBaseType->GetOffset ();
				pBaseTypeCoord->m_FieldCoord.m_LlvmIndexArray [Level] = pBaseType->m_pFieldBaseType->GetLlvmIndex ();
				pBaseTypeCoord->m_VTableIndex += pBaseType->m_VTableIndex;
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
	pFunction->m_pType = pFullType;
	pFunction->m_pShortType = pType;

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

	pType->m_PropertyKind = EProperty_Member;
	pType->m_pParentClassType = this;
	pType->m_pParentNamespace = this;
	pType->m_Name = Name;
	pType->m_Tag = pType->GetQualifiedName ();
	pType->TagAccessors ();

	AddMethodFunction (pType->GetGetter ());

	CFunctionOverload* pSetter = pType->GetSetter ();
	size_t Count = pSetter->GetOverloadCount ();
	for (size_t i = 0; i < Count; i++)
		AddMethodFunction (pSetter->GetFunction (i));

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

	CreateVTableStructType ();

	m_pInterfaceHdrStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType (m_PackFactor);
	m_pInterfaceHdrStructType->m_Tag.Format (_T("%s.iface.hdr"), m_Tag);
	m_pInterfaceHdrStructType->CreateMember (m_pVTableStructType->GetPointerType (EType_Pointer_u));
	m_pInterfaceHdrStructType->CreateMember (m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectHdr)->GetPointerType (EType_Pointer_u));

	m_pInterfaceStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType (m_PackFactor);
	m_pInterfaceStructType->m_Tag.Format (_T("%s.iface"), m_Tag);
	m_pInterfaceStructType->AddBaseType (m_pInterfaceHdrStructType);

	// lay out base types

	rtl::CIteratorT <CClassBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		CClassType* pBaseClassType = BaseType->m_pType;

		Result = pBaseClassType->CalcLayout ();
		if (!Result)
			return false;
				
		BaseType->m_pFieldBaseType = m_pInterfaceStructType->AddBaseType (pBaseClassType->GetInterfaceStructType ());
		BaseType->m_VTableIndex = m_VTable.GetCount ();

		AppendVTableType (pBaseClassType);
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

		AppendVTableType (pPropertyType);
	}

	// lay out methods

	size_t Count = m_MethodFunctionArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunction* pFunction = m_MethodFunctionArray [i];
		if (pFunction->m_VTableIndex != -1) // already layed out
		{
			ASSERT (pFunction->m_PropertyAccessorKind && pFunction->m_Name.IsEmpty ());
			continue;
		}
		
		Result = LayoutFunction (pFunction);
		if (!Result)
			return false;
	}

	Result = m_pVTableStructType->CalcLayout ();
	if (!Result)
		return false;

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

	CClassBaseTypeCoord BaseTypeCoord;
	CClassMember* pMember = FindMemberImpl (
		pFunction->m_PropertyAccessorKind != 0, // include 'this' only for property accessors
		pFunction->m_Name.GetShortName (),
		&BaseTypeCoord,
		0
		);

	CFunction* pOverridenFunction = NULL;

	CFunctionType* pShortType = pFunction->GetShortType ();

	if (pFunction->m_PropertyAccessorKind)
	{
		if (pMember && pMember->GetMemberKind () == EClassMember_Property)
		{
			CClassPropertyMember* pPropertyMember = (CClassPropertyMember*) pMember;
			CPropertyType* pPropertyType = pPropertyMember->GetType ();

			pOverridenFunction = pFunction->m_PropertyAccessorKind == EPropertyAccessor_Set ? 
				pPropertyType->GetSetter ()->FindShortOverload (pShortType) : 
				pPropertyType->GetGetter ()->GetShortType ()->Cmp (pShortType) == 0 ? pPropertyType->GetGetter () : 
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
			pOverridenFunction = pMethodMember->FindShortOverload (pFunction->GetShortType ());
		}

		if (!pOverridenFunction)
		{
			AddFunctionToVTable (pFunction);
			return true;
		}
	}

	pFunction->m_pType = pOverridenFunction->m_pType;
	pFunction->m_pVTableType = pOverridenFunction->m_pVTableType;
	pFunction->m_VTableIndex = pOverridenFunction->m_VTableIndex;

	size_t VTableIndex = BaseTypeCoord.m_VTableIndex + pOverridenFunction->m_VTableIndex;
	ASSERT (VTableIndex < m_VTable.GetCount ());
	m_VTable [VTableIndex] = pFunction;
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
