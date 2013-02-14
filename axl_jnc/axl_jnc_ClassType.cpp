#include "stdafx.h"
#include "axl_jnc_ClassType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CClassType::CClassType ()
{
	m_TypeKind = EType_Class;
	m_pInterfaceStructType = NULL;
	m_pClassStructType = NULL;
	m_pExtensionNamespace = NULL;

	m_pPreConstructor = NULL;
	m_pConstructor = NULL;
	m_pStaticConstructor = NULL;
	m_pDestructor = NULL;
	m_pInitializer = NULL;

	m_PackFactor = 8;
	m_pFieldStructType = NULL;
	m_pFieldMember = NULL;
	m_pStaticFieldStructType = NULL;
	m_pStaticDataVariable = NULL;

	m_pVTableStructType = NULL;
	m_pClassPtrTypeTuple = NULL;
}

CClassPtrType* 
CClassType::GetClassPtrType (
	EClassPtrType PtrTypeKind,
	int Flags
	)
{
	return m_pModule->m_TypeMgr.GetClassPtrType (this, PtrTypeKind, Flags);
}

CFunctionType* 
CClassType::GetMethodMemberType (
	CFunctionType* pShortType,
	int ThisArgTypeFlags
	)
{
	return m_pModule->m_TypeMgr.GetMethodMemberType (this, pShortType, ThisArgTypeFlags);
}

CPropertyType* 
CClassType::GetPropertyMemberType (CPropertyType* pShortType)
{
	return m_pModule->m_TypeMgr.GetPropertyMemberType (this, pShortType);
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

CModuleItem*
CClassType::FindMemberImpl (
	bool IncludeThis, 
	bool IncludeExtensionNamespace,
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
			if (pBaseTypeCoord)
				pBaseTypeCoord->m_FieldCoord.m_LlvmIndexArray.SetCount (Level);

			return It->m_Value;
		}
	}
	
	if (IncludeExtensionNamespace && m_pExtensionNamespace)
	{
		CModuleItem* pMember = m_pExtensionNamespace->FindItem (pName);
		if (pMember)
		{
			if (pBaseTypeCoord)
				pBaseTypeCoord->m_FieldCoord.m_LlvmIndexArray.SetCount (Level);

			return pMember;
		}
	}

	rtl::CIteratorT <CClassBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		CClassBaseType* pBaseType = *BaseType;
		CModuleItem* pMember = pBaseType->m_pType->FindMemberImpl (true, IncludeExtensionNamespace, pName, pBaseTypeCoord, Level + 1);
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

CStructMember*
CClassType::CreateFieldMember (
	EStorage StorageKind,
	const rtl::CString& Name,
	CType* pType,
	size_t BitCount
	)
{
	CStructType* pFieldStructType;

	switch (StorageKind)
	{
	case EStorage_Undefined:
	case EStorage_Mutable:
		if (!m_pFieldStructType)
		{
			m_pFieldStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType (m_PackFactor);
			m_pFieldStructType->m_pFieldParent = this;
			m_pFieldStructType->m_Tag.Format (_T("%s.field_struct"), m_Tag);
		}

		pFieldStructType = m_pFieldStructType;
		break;

	case EStorage_Static:
		if (!m_pStaticFieldStructType)
		{
			m_pStaticFieldStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType (m_PackFactor);
			m_pStaticFieldStructType->m_StorageKind = EStorage_Static;
			m_pStaticFieldStructType->m_pFieldParent = this;
			m_pStaticFieldStructType->m_Tag.Format (_T("%s.static_field_struct"), m_Tag);
		}

		pFieldStructType = m_pStaticFieldStructType;
		break;

	default:
		err::SetFormatStringError (_T("invalid storage '%s' for field member '%s'"), GetStorageKindString (StorageKind), Name);
		return NULL;
	}

	CStructMember* pMember = pFieldStructType->CreateMember (Name, pType, BitCount);

	if (!Name.IsEmpty ())
	{
		bool Result = AddItem (pMember);
		if (!Result)
			return NULL;
	}

	return pMember;
}

bool
CClassType::AddMethodMember (CFunction* pFunction)
{
	EStorage StorageKind = pFunction->GetStorageKind ();
	EFunction FunctionKind = pFunction->GetFunctionKind ();
	int FunctionKindFlags = GetFunctionKindFlags (FunctionKind);
	int ThisArgTypeFlags = pFunction->m_ThisArgTypeFlags;

	switch (StorageKind)
	{
	case EStorage_Static:
		if (ThisArgTypeFlags)
		{
			err::SetFormatStringError (_T("static method cannot be '%s'"), GetPtrTypeFlagString (ThisArgTypeFlags));
			return false;
		}

		break;

	case EStorage_Abstract:
	case EStorage_Virtual:
		m_VirtualMethodArray.Append (pFunction);
		// and fall through;

	case EStorage_Undefined:
	case EStorage_NoVirtual:
		pFunction->ConvertToMethodMember (this);
		break;

	default:
		err::SetFormatStringError (_T("invalid storage specifier '%s' for method member"), GetStorageKindString (StorageKind));
		return false;
	}

	pFunction->m_pParentNamespace = this;

	CFunction** ppTarget = NULL;

	switch (FunctionKind)
	{
	case EFunction_Constructor:
		ppTarget = &m_pConstructor;
		break;

	case EFunction_PreConstructor:
		ppTarget = &m_pPreConstructor;
		break;

	case EFunction_StaticConstructor:
		ppTarget = &m_pStaticConstructor;
		break;

	case EFunction_Destructor:
		ppTarget = &m_pDestructor;
		break;

	case EFunction_Named:
		return AddFunction (pFunction);

	case EFunction_AutoEv:
		pFunction->m_Tag.Format (_T("%s.autoev"), m_Tag);
		return true;

	case EFunction_UnaryOperator:
		pFunction->m_Tag.Format (_T("%s.operator %s"), m_Tag, GetUnOpKindString (pFunction->GetUnOpKind ()));

		if (m_UnaryOperatorTable.IsEmpty ())
			m_UnaryOperatorTable.SetCount (EUnOp__Count);

		ppTarget = &m_UnaryOperatorTable [pFunction->GetUnOpKind ()];
		break;

	case EFunction_BinaryOperator:
		pFunction->m_Tag.Format (_T("%s.operator %s"), m_Tag, GetBinOpKindString (pFunction->GetBinOpKind ()));

		if (m_BinaryOperatorTable.IsEmpty ())
			m_BinaryOperatorTable.SetCount (EBinOp__Count);

		ppTarget = &m_BinaryOperatorTable [pFunction->GetBinOpKind ()];
		break;

	default:
		err::SetFormatStringError (_T("invalid %s in '%s'"), GetFunctionKindString (FunctionKind), GetTypeString ());
		return false;
	}

	pFunction->m_Tag.Format (_T("%s.%s"), m_Tag, GetFunctionKindString (FunctionKind));

	if (!*ppTarget)
	{
		*ppTarget = pFunction;
	}
	else if (FunctionKindFlags & EFunctionKindFlag_NoOverloads)
	{
		err::SetFormatStringError (_T("'%s' already has %s"), GetTypeString (), GetFunctionKindString (FunctionKind));
		return false;
	}
	else
	{
		bool Result = (*ppTarget)->AddOverload (pFunction);
		if (!Result)
			return false;
	}

	return true;
}

bool
CClassType::AddPropertyMember (CProperty* pProperty)
{
	EStorage StorageKind = pProperty->GetStorageKind ();

	switch (StorageKind)
	{
	case EStorage_Static:
		break;

	case EStorage_Abstract:
	case EStorage_Virtual:
		m_VirtualPropertyArray.Append (pProperty);
		// and fall through;

	case EStorage_Undefined:
	case EStorage_NoVirtual:
		pProperty->ConvertToPropertyMember (this);
		break;
	}

	pProperty->m_pParentNamespace = this;

	return true;
}

bool
CClassType::GetVTablePtrValue (CValue* pValue)
{
	if (!m_VTablePtrValue.IsEmpty ())
	{
		*pValue = m_VTablePtrValue;
		return true;
	}

	char Buffer [256];
	rtl::CArrayT <llvm::Constant*> LlvmVTable (ref::EBuf_Stack, Buffer, sizeof (Buffer));

	size_t Count = m_VTable.GetCount ();
	LlvmVTable.SetCount (Count);

	for (size_t i = 0; i < Count; i++)
	{
		CFunction* pFunction = m_VTable [i];
		if (!pFunction->IsDefined ())
		{
			err::SetFormatStringError (
				_T("cannot instantiate abstract '%s': '%s' has no body"), 
				GetTypeString (),
				pFunction->m_Tag
				);
			return false;
		}

		LlvmVTable [i] = pFunction->GetLlvmFunction ();
	}

	llvm::Constant* pLlvmVTableConstant = llvm::ConstantStruct::get (
		(llvm::StructType*) m_pVTableStructType->GetLlvmType (),
		llvm::ArrayRef <llvm::Constant*> (LlvmVTable, Count)
		);

	rtl::CString VariableTag;
	VariableTag.Format (_T("%s.vtbl"), GetQualifiedName ());
	llvm::GlobalVariable* pLlvmVTableVariable = new llvm::GlobalVariable (
			*m_pModule->m_pLlvmModule,
			m_pVTableStructType->GetLlvmType (),
			false,
			llvm::GlobalVariable::ExternalLinkage,
			pLlvmVTableConstant,
			(const tchar_t*) VariableTag
			);

	m_VTablePtrValue.SetLlvmValue (
		pLlvmVTableVariable, 
		m_pVTableStructType->GetDataPtrType (EDataPtrType_Unsafe),
		EValue_Const
		);

	*pValue = m_VTablePtrValue;
	return true;
}

bool
CClassType::CalcLayout ()
{
	if (m_Flags & ETypeFlag_LayoutReady)
		return true;

	bool Result = PreCalcLayout ();
	if (!Result)
		return false;

	m_pVTableStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	m_pVTableStructType->m_Tag.Format (_T("%s.vtbl"), m_Tag);

	CStructType* pIfaceHdrStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType (m_PackFactor);
	pIfaceHdrStructType->m_Tag.Format (_T("%s.ifacehdr"), m_Tag);
	pIfaceHdrStructType->CreateMember (m_pVTableStructType->GetDataPtrType (EDataPtrType_Unsafe));
	pIfaceHdrStructType->CreateMember (m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectHdr)->GetDataPtrType (EDataPtrType_Unsafe));

	m_pInterfaceStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType (m_PackFactor);
	m_pInterfaceStructType->m_Tag.Format (_T("%s.iface"), m_Tag);
	m_pInterfaceStructType->AddBaseType (pIfaceHdrStructType);

	// layout base types

	rtl::CIteratorT <CClassBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		CClassType* pBaseClassType = BaseType->m_pType;

		Result = pBaseClassType->CalcLayout ();
		if (!Result)
			return false;
				
		BaseType->m_pFieldBaseType = m_pInterfaceStructType->AddBaseType (pBaseClassType->GetInterfaceStructType ());
		BaseType->m_VTableIndex = m_VTable.GetCount ();

		m_VTable.Append (pBaseClassType->m_VTable);
		m_pVTableStructType->Append (pBaseClassType->m_pVTableStructType);
	}

	// layout virtual properties

	size_t Count = m_VirtualPropertyArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CProperty* pProperty = m_VirtualPropertyArray [i];
		Result = pProperty->CalcLayout ();
		if (!Result)
			return false;

		size_t VTableIndex = m_VTable.GetCount ();

		pProperty->m_ParentClassVTableIndex = VTableIndex;
		m_VTable.Append (pProperty->m_VTable);
		m_pVTableStructType->Append (pProperty->m_pType->GetVTableStructType ());

		size_t AccessorCount = pProperty->m_VTable.GetCount ();
		for (size_t j = 0; j < AccessorCount; j++)
		{
			CFunction* pAccessor = pProperty->m_VTable [j];
			pAccessor->m_pVirtualOriginClassType = this;
			pAccessor->m_ClassVTableIndex = VTableIndex + j;
		}
	}

	// layout virtual methods

	Count = m_VirtualMethodArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunction* pFunction = m_VirtualMethodArray [i];

		Result = LayoutNamedVirtualFunction (pFunction);
		if (!Result)
			return false;
	}

	Result = m_pVTableStructType->CalcLayout ();
	if (!Result)
		return false;

	// layout fields

	if (m_pFieldStructType)
	{
		Result = m_pFieldStructType->CalcLayout ();
		if (!Result)
			return false;
	}

	if (m_pStaticFieldStructType)
	{
		Result = m_pStaticFieldStructType->CalcLayout ();
		if (!Result)
			return false;

		m_pStaticDataVariable = m_pModule->m_VariableMgr.CreateVariable (
			EVariable_Global, 
			m_Tag, 
			m_pStaticFieldStructType, 
			false
			);
	}

	if (m_pFieldStructType)
		m_pFieldMember = m_pInterfaceStructType->CreateMember (m_pFieldStructType);

	// extension namespace

	if (m_pExtensionNamespace)
	{
		size_t Count = m_pExtensionNamespace->GetItemCount ();
		for (size_t i = 0; i < Count; i++)
		{
			CModuleItem* pItem = m_pExtensionNamespace->GetItem (i);
			EModuleItem ItemKind = pItem->GetItemKind ();

			switch (ItemKind)
			{
			case EModuleItem_Function:
				((CFunction*) pItem)->ConvertToMethodMember (this);
				break;

			case EModuleItem_Property:
				((CProperty*) pItem)->ConvertToPropertyMember (this);
				break;
			}
		}
	}

	// finalize

	Result = m_pInterfaceStructType->CalcLayout ();
	if (!Result)
		return false;

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
CClassType::LayoutNamedVirtualFunction (CFunction* pFunction)
{
	ASSERT (
		pFunction->m_FunctionKind == EFunction_Named &&
		pFunction->m_StorageKind == EStorage_Virtual && 
		pFunction->m_pClassType == this &&
		pFunction->m_pVirtualOriginClassType == NULL &&
		pFunction->m_ClassVTableIndex == -1
		);

	CFunction* pOverridenFunction = NULL;
	CClassBaseTypeCoord BaseTypeCoord;
	CModuleItem* pMember = FindMemberImpl (false, false, pFunction->m_Name, &BaseTypeCoord, 0);
	if (pMember && pMember->GetItemKind () == EModuleItem_Function)
	{
		pOverridenFunction = (CFunction*) pMember;
		pOverridenFunction = pOverridenFunction->FindShortOverload (pFunction->GetType ()->GetShortType ());
		
		if (pOverridenFunction->m_StorageKind != EStorage_Virtual)
			pOverridenFunction = NULL;
	}

	if (!pOverridenFunction)
	{
		pFunction->m_pVirtualOriginClassType = this;
		pFunction->m_ClassVTableIndex = m_VTable.GetCount ();

		CFunctionPtrType* pPointerType = pFunction->GetType ()->GetFunctionPtrType (EFunctionPtrType_Unsafe);
		m_pVTableStructType->CreateMember (pPointerType);
		m_VTable.Append (pFunction);
		return true;
	}

	#pragma AXL_TODO ("virtual multipliers")

	pFunction->m_pType = pOverridenFunction->m_pType;
	pFunction->m_pVirtualOriginClassType = pOverridenFunction->m_pVirtualOriginClassType;
	pFunction->m_ClassVTableIndex = pOverridenFunction->m_ClassVTableIndex;

	size_t VTableIndex = BaseTypeCoord.m_VTableIndex + pOverridenFunction->m_ClassVTableIndex;
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

} // namespace jnc {
} // namespace axl {

