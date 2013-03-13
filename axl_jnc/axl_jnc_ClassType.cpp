#include "stdafx.h"
#include "axl_jnc_ClassType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CClassType::CClassType ()
{
	m_TypeKind = EType_Class;
	m_pIfaceStructType = NULL;
	m_pClassStructType = NULL;
	m_pExtensionNamespace = NULL;

	m_pPreConstructor = NULL;
	m_pConstructor = NULL;
	m_pStaticConstructor = NULL;
	m_pDestructor = NULL;
	m_pInitializer = NULL;

	m_PackFactor = 8;
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

CAutoEvType* 
CClassType::GetAutoEvMemberType (CAutoEvType* pShortType)
{
	return m_pModule->m_TypeMgr.GetAutoEvMemberType (this, pShortType);
}

CFunction* 
CClassType::GetDefaultConstructor ()
{
	ASSERT (m_pConstructor);

	CType* pThisArgType = GetClassPtrType ();
	CFunction* pDefaultConstructor = m_pConstructor->ChooseOverload (&pThisArgType, 1);
	if (!pDefaultConstructor)
	{
		err::SetFormatStringError (_T("'%s' does not provide a default constructor"), GetTypeString ());
		return NULL;
	}

	return pDefaultConstructor;
}

CStructField*
CClassType::CreateFieldMember (
	EStorage StorageKind,
	const rtl::CString& Name,
	CType* pType,
	size_t BitCount,
	int PtrTypeFlags
	)
{
	CStructType* pFieldStructType;

	switch (StorageKind)
	{
	case EStorage_Undefined:
	case EStorage_Member:
		pFieldStructType = m_pIfaceStructType;
		break;

	case EStorage_Static:
		if (!m_pStaticFieldStructType)
		{
			m_pStaticFieldStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType (m_PackFactor);
			m_pStaticFieldStructType->m_StorageKind = EStorage_Static;
			m_pStaticFieldStructType->m_pParentNamespace = this;
			m_pStaticFieldStructType->m_Tag.Format (_T("%s.static_field_struct"), m_Tag);
		}

		pFieldStructType = m_pStaticFieldStructType;
		break;

	default:
		err::SetFormatStringError (_T("invalid storage '%s' for field member '%s'"), GetStorageKindString (StorageKind), Name);
		return NULL;
	}

	CStructField* pMember = pFieldStructType->CreateFieldMember (Name, pType, BitCount, PtrTypeFlags);

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

	pFunction->m_pParentNamespace = this;

	switch (StorageKind)
	{
	case EStorage_Static:
		if (ThisArgTypeFlags)
		{
			err::SetFormatStringError (_T("static method cannot be '%s'"), GetPtrTypeFlagString (ThisArgTypeFlags));
			return false;
		}

		break;

	case EStorage_Undefined:
		pFunction->m_StorageKind = EStorage_Member;
		// and fall through

	case EStorage_Member:
		pFunction->ConvertToMethodMember (this);
		break;

	case EStorage_Override:
		m_OverrideMethodArray.Append (pFunction);
		pFunction->ConvertToMethodMember (this);
		return true; // layout overrides later

	case EStorage_Abstract:
	case EStorage_Virtual:
		m_VirtualMethodArray.Append (pFunction);
		pFunction->ConvertToMethodMember (this);
		break;

	default:
		err::SetFormatStringError (_T("invalid storage specifier '%s' for method member"), GetStorageKindString (StorageKind));
		return false;
	}

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
		err::SetFormatStringError (_T("'%s' already has '%s' method"), GetTypeString (), GetFunctionKindString (FunctionKind));
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
	ASSERT (pProperty->IsNamed ());
	bool Result = AddItem (pProperty);
	if (!Result)
		return false;

	pProperty->m_pParentNamespace = this;

	EStorage StorageKind = pProperty->GetStorageKind ();
	switch (StorageKind)
	{
	case EStorage_Static:
		break;

	case EStorage_Undefined:
		pProperty->m_StorageKind = EStorage_Member;
		//and fall through

	case EStorage_Member:
		pProperty->m_pParentClassType = this;
		break;

	case EStorage_Abstract:
	case EStorage_Virtual:
	case EStorage_Override:
		m_VirtualPropertyArray.Append (pProperty);
		pProperty->m_pParentClassType = this;
		break;
	}

	return true;
}

bool
CClassType::AddAutoEvMember (CAutoEv* pAutoEv)
{
	ASSERT (pAutoEv->IsNamed () || (m_Flags & EClassTypeFlag_AutoEv));
	if (pAutoEv->IsNamed ())
	{
		bool Result = AddItem (pAutoEv);
		if (!Result)
			return false;
	}

	pAutoEv->m_pParentNamespace = this;

	EStorage StorageKind = pAutoEv->GetStorageKind ();
	switch (StorageKind)
	{
	case EStorage_Static:
		break;

	case EStorage_Undefined:
		pAutoEv->m_StorageKind = EStorage_Member;
		//and fall through

	case EStorage_Member:
		m_AutoEvArray.Append (pAutoEv);
		pAutoEv->m_pParentClassType = this;
		break;

	default:
		err::SetFormatStringError (_T("invalid storage '%s' for autoev member"), GetStorageKindString (StorageKind));
		return false;
	}

	return true;
}

CFunction*
CClassType::CreateMethodMember (
	EStorage StorageKind,
	const rtl::CString& Name,
	CFunctionType* pShortType
	)
{
	rtl::CString QualifiedName = CreateQualifiedName (Name);

	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateFunction (EFunction_Named, pShortType);
	pFunction->m_StorageKind = StorageKind;
	pFunction->m_Name = Name;
	pFunction->m_QualifiedName = QualifiedName;
	pFunction->m_Tag = QualifiedName;

	bool Result = AddMethodMember (pFunction);
	if (!Result)
		return NULL;

	return pFunction;
}

CProperty*
CClassType::CreatePropertyMember (
	EStorage StorageKind,
	const rtl::CString& Name,
	CPropertyType* pShortType
	)
{
	rtl::CString QualifiedName = CreateQualifiedName (Name);

	CProperty* pProperty = m_pModule->m_FunctionMgr.CreateProperty (Name, QualifiedName);

	bool Result = 
		AddPropertyMember (pProperty) &&
		pProperty->Create (pShortType);

	if (!Result)
		return NULL;
	
	return pProperty;
}

bool
CClassType::CalcLayout ()
{
	if (m_Flags & ETypeFlag_LayoutReady)
		return true;

	bool Result = PreCalcLayout ();
	if (!Result)
		return false;
		
	// layout base types

	bool HasBaseConstructor = false;
	bool HasBaseDestructor = false;

	size_t BaseTypeCount = m_BaseTypeList.GetCount ();

	char Buffer [256];
	rtl::CArrayT <CBaseType*> IfaceBaseTypeArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	IfaceBaseTypeArray.SetCount (BaseTypeCount);

	rtl::CIteratorT <CBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (size_t i = 0; BaseType; i++, BaseType++)
	{
		Result = BaseType->m_pType->CalcLayout ();
		if (!Result)
			return false;

		CClassType* pBaseClassType;
		CBaseType* pIfaceBaseType;

		EType TypeKind = BaseType->m_pType->GetTypeKind ();
		switch (TypeKind)
		{
		case EType_Class:
			pBaseClassType = (CClassType*) BaseType->m_pType;
			pIfaceBaseType = m_pIfaceStructType->AddBaseType (pBaseClassType->GetIfaceStructType ());
			BaseType->m_VTableIndex = m_VTable.GetCount ();
			m_VTable.Append (pBaseClassType->m_VTable);
			m_pVTableStructType->Append (pBaseClassType->m_pVTableStructType);

			if (pBaseClassType->m_pConstructor)
				HasBaseConstructor = true;

			if (pBaseClassType->m_pDestructor)
				HasBaseDestructor = true;

			break;

		case EType_Struct:
		case EType_Union:
			pIfaceBaseType = m_pIfaceStructType->AddBaseType (BaseType->m_pType);
			break;

		default:
			err::SetFormatStringError (_T("invalid base type '%s'"), BaseType->m_pType->GetTypeString ());
			return false;
		}

		IfaceBaseTypeArray [i] = pIfaceBaseType;
	}

	// finalize iface layout

	Result = m_pIfaceStructType->CalcLayout ();
	if (!Result)
		return false;

	// update base type llvm indexes & offsets

	BaseType = m_BaseTypeList.GetHead ();
	for (size_t i = 0; BaseType; i++, BaseType++)
	{
		CBaseType* pIfaceBaseType = IfaceBaseTypeArray [i];

		BaseType->m_LlvmIndex = pIfaceBaseType->m_LlvmIndex;
		BaseType->m_Offset = pIfaceBaseType->m_Offset;
	}

	// static fields

	if (m_pStaticFieldStructType)
	{
		Result = m_pStaticFieldStructType->CalcLayout ();
		if (!Result)
			return false;

		m_pStaticDataVariable = m_pModule->m_VariableMgr.CreateVariable (
			EVariable_Global, 
			_T("static_field"),
			m_Tag + _T(".static_field"), 
			m_pStaticFieldStructType
			);
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
		ASSERT (pFunction->m_StorageKind == EStorage_Abstract || EStorage_Virtual);

		AddVirtualFunction (pFunction);
	}

	Count = m_OverrideMethodArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunction* pFunction = m_OverrideMethodArray [i];
		ASSERT (pFunction->m_StorageKind == EStorage_Override);

		Result = OverrideVirtualFunction (pFunction);
		if (!Result)
			return false;
	}

	Result = m_pVTableStructType->CalcLayout ();
	if (!Result)
		return false;

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

	m_pClassStructType->CalcLayout ();

	CreateVTablePtr ();

	if (m_Flags & EClassTypeFlag_AutoEv)
	{
		Result = CreateAutoEvConstructor () && CreateDefaultDestructor ();
		if (!Result)
			return false;
	}
	else
	{
		if (!m_pConstructor && (m_pPreConstructor || HasBaseConstructor))
		{
			Result = CreateDefaultConstructor ();
			if (!Result)
				return false;
		}

		if (!m_pDestructor && (!m_AutoEvArray.IsEmpty () || HasBaseDestructor))
		{
			Result = CreateDefaultDestructor ();
			if (!Result)
				return false;
		}
	}

	PostCalcLayout ();
	return true;
}

void
CClassType::AddVirtualFunction (CFunction* pFunction)
{
	ASSERT (pFunction->m_StorageKind == EStorage_Abstract || pFunction->m_StorageKind == EStorage_Virtual);
	ASSERT (pFunction->m_pVirtualOriginClassType == NULL); // not layed out yet

	pFunction->m_pVirtualOriginClassType = this;
	pFunction->m_ClassVTableIndex = m_VTable.GetCount ();

	CFunctionPtrType* pPointerType = pFunction->GetType ()->GetFunctionPtrType (EFunctionPtrType_Unsafe);
	m_pVTableStructType->CreateFieldMember (pPointerType);
	m_VTable.Append (pFunction);
}

bool
CClassType::OverrideVirtualFunction (CFunction* pFunction)
{
	ASSERT (pFunction->m_StorageKind == EStorage_Override);
	ASSERT (pFunction->m_pVirtualOriginClassType == NULL); // not layed out yet

	EFunction FunctionKind = pFunction->GetFunctionKind ();

	CBaseTypeCoord BaseTypeCoord;
	CModuleItem* pMember = FindItemTraverse (
		pFunction->m_DeclaratorName, 
		&BaseTypeCoord, 
		ETraverse_NoExtensionNamespace | ETraverse_NoParentNamespace
		);

	if (!pMember)
	{
		err::SetFormatStringError (_T("cannot override '%s': method not found"), pFunction->m_Tag);
		return false;
	}

	CFunction* pOverridenFunction;

	EModuleItem ItemKind = pMember->GetItemKind ();
	switch (ItemKind)
	{
	case EModuleItem_Function:
		if (FunctionKind != EFunction_Named)
		{
			err::SetFormatStringError (_T("cannot override '%s': function kind mismatch"), pFunction->m_Tag);
			return false;
		}

		pOverridenFunction = (CFunction*) pMember;
		break;

	case EModuleItem_Property:
		switch (FunctionKind)
		{
		case EFunction_Getter:
			pOverridenFunction = ((CProperty*) pMember)->GetGetter ();
			break;

		case EFunction_Setter:
			pOverridenFunction = ((CProperty*) pMember)->GetSetter ();
			if (!pOverridenFunction)
			{
				err::SetFormatStringError (_T("cannot override '%s': property has no setter"), pFunction->m_Tag);
				return false;
			}

			break;

		default:
			err::SetFormatStringError (_T("cannot override '%s': function kind mismatch"), pFunction->m_Tag);
			return false;
		}

		break;

	default:
		err::SetFormatStringError (_T("cannot override '%s': not a method or property"), pFunction->m_Tag);
		return false;
	}
	
	pOverridenFunction = pOverridenFunction->FindShortOverload (pFunction->GetType ()->GetShortType ());
	if (!pOverridenFunction)
	{
		err::SetFormatStringError (_T("cannot override '%s': method signature mismatch"), pFunction->m_Tag);
		return false;
	}

	if (!pOverridenFunction->IsVirtual ())
	{
		err::SetFormatStringError (_T("cannot override '%s': method is not virtual"), pFunction->m_Tag);
		return false;
	}

	#pragma AXL_TODO ("virtual multipliers")

	pFunction->m_pType = pOverridenFunction->m_pType;
	pFunction->m_pThisArgType = pOverridenFunction->m_pThisArgType;
	pFunction->m_ThisArgDelta = -(intptr_t) BaseTypeCoord.m_Offset;
	pFunction->m_pVirtualOriginClassType = pOverridenFunction->m_pVirtualOriginClassType;
	pFunction->m_ClassVTableIndex = pOverridenFunction->m_ClassVTableIndex;

	size_t VTableIndex = BaseTypeCoord.m_VTableIndex + pOverridenFunction->m_ClassVTableIndex;
	ASSERT (VTableIndex < m_VTable.GetCount ());
	m_VTable [VTableIndex] = pFunction;
	return true;
}

void
CClassType::CreateVTablePtr ()
{
	if (m_VTable.IsEmpty ())
	{
		m_VTablePtrValue = m_pVTableStructType->GetDataPtrType (EDataPtrType_Unsafe)->GetZeroValue ();
		return;
	}

	char Buffer [256];
	rtl::CArrayT <llvm::Constant*> LlvmVTable (ref::EBuf_Stack, Buffer, sizeof (Buffer));

	size_t Count = m_VTable.GetCount ();
	LlvmVTable.SetCount (Count);

	for (size_t i = 0; i < Count; i++)
	{
		CFunction* pFunction = m_VTable [i];
		if (pFunction->GetStorageKind () == EStorage_Abstract)
		{
			pFunction = pFunction->GetType ()->GetAbstractFunction ();
			m_Flags |= EClassTypeFlag_Abstract;
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
}

bool
CClassType::CreateAutoEvConstructor ()
{
	ASSERT (!m_pConstructor && m_AutoEvArray.GetCount () == 1);

	CAutoEv* pAutoEv = m_AutoEvArray [0];
	CFunction* pStarter = pAutoEv->GetStarter ();
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateFunction (EFunction_Constructor, pStarter->GetType ());
	pFunction->m_Tag = m_Tag + _T(".this");

	size_t ArgCount = pStarter->GetType ()->GetArgTypeArray ().GetCount ();
	
	char Buffer [256];
	rtl::CArrayT <CValue> ArgValueArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	ArgValueArray.SetCount (ArgCount);

	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, ArgValueArray, ArgCount);

	CValue ReturnValue;
	m_pModule->m_LlvmBuilder.CreateCall (
		pStarter, 
		pStarter->GetType (), 
		ArgValueArray, 
		ArgCount,
		&ReturnValue
		);

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	m_pConstructor = pFunction;
	return true;
}

bool
CClassType::CreateDefaultConstructor ()
{
	ASSERT (!m_pConstructor);

	CType* ArgTypeArray [] =
	{
		GetClassPtrType ()
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateFunction (EFunction_Constructor, pType);
	pFunction->m_Tag = m_Tag + _T(".this");

	CValue ArgValue;
	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, &ArgValue, 1);

	rtl::CIteratorT <CBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		if (BaseType->m_pType->GetTypeKind () != EType_Class)
			continue;

		CClassType* pBaseClassType = (CClassType*) BaseType->m_pType;
		if (!pBaseClassType->GetConstructor ())
			continue;

		CFunction* pConstructor = pBaseClassType->GetDefaultConstructor ();
		if (!pConstructor)
			return false;

		bool Result = m_pModule->m_OperatorMgr.CallOperator (pConstructor, ArgValue);
		if (!Result)
			return false;
	}

	if (m_pPreConstructor)
	{
		CValue ReturnValue;
		m_pModule->m_LlvmBuilder.CreateCall (
			m_pPreConstructor, 
			m_pPreConstructor->GetType (), 
			ArgValue, 
			&ReturnValue
			);
	}

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	m_pConstructor = pFunction;
	return true;
}

bool
CClassType::CreateDefaultDestructor ()
{
	ASSERT (!m_pDestructor);

	CType* ArgTypeArray [] =
	{
		GetClassPtrType ()
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateFunction (EFunction_Constructor, pType);
	pFunction->m_Tag = m_Tag + _T(".~this");
		
	CValue ArgValue;
	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, &ArgValue, 1);

	bool Result = 
		StopAutoEvs (ArgValue) &&
		CallBaseDestructors (ArgValue);

	if (!Result)
		return false;

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	m_pDestructor = pFunction;
	return true;
}

CFunction* 
CClassType::GetInitializer ()
{
	if (m_pInitializer)
		return m_pInitializer;

	CType* ArgTypeArray [] =
	{
		GetClassStructType ()->GetDataPtrType (EDataPtrType_Unsafe),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT)
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction (m_Tag + _T(".init"), pType);

	CValue ArgValueArray [countof (ArgTypeArray)];
	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, ArgValueArray, countof (ArgValueArray));

	CValue ArgValue1 = ArgValueArray [0];
	CValue ArgValue2 = ArgValueArray [1];

	CClassType* pClassType = this;
	CValue TypeValue (&pClassType, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr));
	CValue ObjectPtrValue;
	CValue IfacePtrValue;
	CValue PtrValue;

	m_pModule->m_LlvmBuilder.CreateGep2 (ArgValue1, 0, NULL, &ObjectPtrValue);
	m_pModule->m_LlvmBuilder.CreateGep2 (ArgValue1, 1, NULL, &IfacePtrValue);

	// store CClassType*

	m_pModule->m_LlvmBuilder.CreateGep2 (ObjectPtrValue, 0, NULL, &PtrValue);
	m_pModule->m_LlvmBuilder.CreateStore (TypeValue, PtrValue);

	// store ScopeLevel

	m_pModule->m_LlvmBuilder.CreateGep2 (ObjectPtrValue, 1, NULL, &PtrValue);
	m_pModule->m_LlvmBuilder.CreateStore (ArgValue2, PtrValue);

	InitializeInterface (this, ObjectPtrValue, IfacePtrValue, m_VTablePtrValue);

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	m_pInitializer = pFunction;
	return m_pInitializer;
}

bool
CClassType::InitializeInterface (
	CClassType* pClassType,
	const CValue& ObjectPtrValue,
	const CValue& IfacePtrValue,
	const CValue& VTablePtrValue
	)
{
	CValue IfaceHdrPtrValue;
	CValue VTablePtrPtrValue;
	CValue ObjectPtrPtrValue;

	m_pModule->m_LlvmBuilder.CreateGep2 (IfacePtrValue, 0, NULL, &IfaceHdrPtrValue);
	m_pModule->m_LlvmBuilder.CreateGep2 (IfaceHdrPtrValue, 0, NULL, &VTablePtrPtrValue);
	m_pModule->m_LlvmBuilder.CreateGep2 (IfaceHdrPtrValue, 1, NULL, &ObjectPtrPtrValue);
	m_pModule->m_LlvmBuilder.CreateStore (VTablePtrValue, VTablePtrPtrValue);
	m_pModule->m_LlvmBuilder.CreateStore (ObjectPtrValue, ObjectPtrPtrValue);

	rtl::CIteratorT <CBaseType> BaseType = pClassType->GetBaseTypeList ().GetHead ();
	for (; BaseType; BaseType++)
	{
		if (BaseType->m_pType->GetTypeKind () != EType_Class)
			continue;

		CClassType* pBaseClassType = (CClassType*) BaseType->m_pType;

		CValue BaseClassPtrValue;
		m_pModule->m_LlvmBuilder.CreateGep2 (
			IfacePtrValue, 
			BaseType->GetLlvmIndex (), 
			NULL, 
			&BaseClassPtrValue
			);

		CValue BaseClassVTablePtrValue;

		if (!pBaseClassType->HasVTable ())
		{
			BaseClassVTablePtrValue = pBaseClassType->GetVTableStructType ()->GetDataPtrType (EDataPtrType_Unsafe)->GetZeroValue ();
		}
		else
		{
			m_pModule->m_LlvmBuilder.CreateGep2 (
				VTablePtrValue, 
				BaseType->GetVTableIndex (), 
				NULL, 
				&BaseClassVTablePtrValue
				);

			m_pModule->m_LlvmBuilder.CreateBitCast (
				BaseClassVTablePtrValue, 
				pBaseClassType->GetVTableStructType ()->GetDataPtrType (EDataPtrType_Unsafe),
				&BaseClassVTablePtrValue
				);
		}		

		InitializeInterface (pBaseClassType, ObjectPtrValue, BaseClassPtrValue, BaseClassVTablePtrValue);
	}

	return true;
}

bool
CClassType::CallBaseDestructors (const CValue& ThisValue)
{
	bool Result;

	rtl::CIteratorT <CBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		if (BaseType->m_pType->GetTypeKind () != EType_Class)
			continue;

		CClassType* pBaseClassType = (CClassType*) BaseType->m_pType;
		CFunction* pDestructor = pBaseClassType->GetDestructor ();		
		if (!pDestructor)
			continue;

		Result = m_pModule->m_OperatorMgr.CallOperator (pDestructor, ThisValue);
		if (!Result)
			return false;
	}
		
	return true;
}

bool 
CClassType::StopAutoEvs (const CValue& ThisValue)
{
	bool Result;

	size_t AutoEvCount = m_AutoEvArray.GetCount ();
	for (size_t i = 0; i < AutoEvCount; i++)
	{
		CAutoEv* pAutoEv = m_AutoEvArray [i];

		CFunction* pStopper = pAutoEv->GetStopper ();		
		if (!pStopper)
			continue;

		Result = m_pModule->m_OperatorMgr.CallOperator (pStopper, ThisValue);
		if (!Result)
			return false;			
	}

	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
