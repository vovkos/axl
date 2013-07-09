#include "pch.h"
#include "axl_jnc_ClassType.h"
#include "axl_jnc_Module.h"
#include "axl_jnc_Parser.llk.h"

namespace axl {
namespace jnc {

//.............................................................................

CClassType::CClassType ()
{
	m_TypeKind = EType_Class;
	m_pIfaceStructType = NULL;
	m_pClassStructType = NULL;
	m_pExtensionNamespace = NULL;
	m_pPrimer = NULL;
	m_pDestructor = NULL;
	m_pVTableStructType = NULL;
	m_pClassPtrTypeTuple = NULL;
}

CClassPtrType* 
CClassType::GetClassPtrType (
	EType TypeKind,
	EClassPtrType PtrTypeKind,
	uint_t Flags
	)
{
	return m_pModule->m_TypeMgr.GetClassPtrType (this, TypeKind, PtrTypeKind, Flags);
}

CStructField*
CClassType::GetFieldByIndex (size_t Index)
{
	if (!m_BaseTypeList.IsEmpty ())
	{
		err::SetFormatStringError ("'%s' has base types, cannot use indexed member operator", GetTypeString ().cc ());
		return NULL;
	}

	return m_pIfaceStructType->GetFieldByIndexImpl (Index, true);
}

CStructField*
CClassType::CreateFieldImpl (
	const rtl::CString& Name,
	CType* pType,
	size_t BitCount,
	uint_t PtrTypeFlags,
	rtl::CBoxListT <CToken>* pConstructor,
	rtl::CBoxListT <CToken>* pInitializer
	)
{
	CStructField* pField = m_pIfaceStructType->CreateField (Name, pType, BitCount, PtrTypeFlags, pConstructor, pInitializer);
	if (!pField)
		return NULL;

	// re-parent

	pField->m_pParentNamespace = this;

	if (!Name.IsEmpty ())
	{
		bool Result = AddItem (pField);
		if (!Result)
			return NULL;
	}

	m_MemberFieldArray.Append (pField);
	return pField;
}

bool
CClassType::AddMethod (CFunction* pFunction)
{
	EStorage StorageKind = pFunction->GetStorageKind ();
	EFunction FunctionKind = pFunction->GetFunctionKind ();
	uint_t FunctionKindFlags = GetFunctionKindFlags (FunctionKind);
	uint_t ThisArgTypeFlags = pFunction->m_ThisArgTypeFlags;

	pFunction->m_pParentNamespace = this;

	switch (StorageKind)
	{
	case EStorage_Static:
		if (ThisArgTypeFlags)
		{
			err::SetFormatStringError ("static method cannot be '%s'", GetPtrTypeFlagString (ThisArgTypeFlags));
			return false;
		}

		break;

	case EStorage_Undefined:
		pFunction->m_StorageKind = EStorage_Member;
		// and fall through

	case EStorage_Member:
		pFunction->ConvertToMemberMethod (this);
		break;

	case EStorage_Override:
		m_OverrideMethodArray.Append (pFunction);
		pFunction->ConvertToMemberMethod (this);
		return true; // layout overrides later

	case EStorage_Abstract:
	case EStorage_Virtual:
		m_VirtualMethodArray.Append (pFunction);
		pFunction->ConvertToMemberMethod (this);
		break;

	default:
		err::SetFormatStringError ("invalid storage specifier '%s' for method member", GetStorageKindString (StorageKind));
		return false;
	}

	CFunction** ppTarget = NULL;

	switch (FunctionKind)
	{
	case EFunction_Internal:
		return true;

	case EFunction_PreConstructor:
		ppTarget = &m_pPreConstructor;
		break;

	case EFunction_Constructor:
		ppTarget = &m_pConstructor;
		break;

	case EFunction_Destructor:
		ppTarget = &m_pDestructor;
		break;

	case EFunction_StaticConstructor:
		ppTarget = &m_pStaticConstructor;
		break;

	case EFunction_StaticDestructor:
		ppTarget = &m_pStaticDestructor;
		break;

	case EFunction_Named:
		return AddFunction (pFunction);

	case EFunction_UnaryOperator:
		pFunction->m_Tag.Format ("%s.operator %s", m_Tag.cc (), GetUnOpKindString (pFunction->GetUnOpKind ()));

		if (m_UnaryOperatorTable.IsEmpty ())
			m_UnaryOperatorTable.SetCount (EUnOp__Count);

		ppTarget = &m_UnaryOperatorTable [pFunction->GetUnOpKind ()];
		break;

	case EFunction_BinaryOperator:
		pFunction->m_Tag.Format ("%s.operator %s", m_Tag.cc (), GetBinOpKindString (pFunction->GetBinOpKind ()));

		if (m_BinaryOperatorTable.IsEmpty ())
			m_BinaryOperatorTable.SetCount (EBinOp__Count);

		ppTarget = &m_BinaryOperatorTable [pFunction->GetBinOpKind ()];
		break;

	case EFunction_CallOperator:
		pFunction->m_Tag.Format ("%s.operator ()", m_Tag.cc ());
		ppTarget = &m_pCallOperator;
		break;

	case EFunction_AutoEvHandler:
		if (m_ClassTypeKind == EClassType_AutoEv)
			return true;

		// else fall through and fail

	default:
		err::SetFormatStringError (
			"invalid %s in '%s'", 
			GetFunctionKindString (FunctionKind), 
			GetTypeString ().cc () 
			);
		return false;
	}

	pFunction->m_Tag.Format ("%s.%s", m_Tag.cc (), GetFunctionKindString (FunctionKind));

	if (!*ppTarget)
	{
		*ppTarget = pFunction;
	}
	else if (FunctionKindFlags & EFunctionKindFlag_NoOverloads)
	{
		err::SetFormatStringError (
			"'%s' already has '%s' method", 
			GetTypeString ().cc (), 
			GetFunctionKindString (FunctionKind)
			);
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
CClassType::AddProperty (CProperty* pProperty)
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
		pProperty->m_pParentType = this;
		break;

	case EStorage_Abstract:
	case EStorage_Virtual:
	case EStorage_Override:
		m_VirtualPropertyArray.Append (pProperty);
		pProperty->m_pParentType = this;
		break;
	}

	m_MemberPropertyArray.Append (pProperty);
	return true;
}

bool
CClassType::CalcLayout ()
{
	bool Result;

	if (m_pExtensionNamespace)
		ApplyExtensionNamespace ();

	// resolve imports

	Result = 
		ResolveImportBaseTypes () &&
		ResolveImportFields ();

	if (!Result)
		return false;

	// layout base types

	size_t BaseTypeCount = m_BaseTypeList.GetCount ();

	char Buffer [256];
	rtl::CArrayT <CBaseTypeSlot*> IfaceBaseTypeArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	IfaceBaseTypeArray.SetCount (BaseTypeCount);

	rtl::CIteratorT <CBaseTypeSlot> Slot = m_BaseTypeList.GetHead ();
	for (size_t i = 0; Slot; i++, Slot++)
	{
		CBaseTypeSlot* pSlot = *Slot;

		CDerivableType* pType = pSlot->GetType ();
		Result = pType->EnsureLayout ();
		if (!Result)
			return false;

		if (pSlot->m_pType->GetConstructor ())
			m_BaseTypeConstructArray.Append (pSlot);

		if (pSlot->m_pType->GetTypeKind () != EType_Class)
		{
			IfaceBaseTypeArray [i] = m_pIfaceStructType->AddBaseType (pSlot->m_pType);
			continue;
		}

		CClassType* pBaseClassType = (CClassType*) pSlot->m_pType;
		IfaceBaseTypeArray [i] = m_pIfaceStructType->AddBaseType (pBaseClassType->GetIfaceStructType ());
		pSlot->m_VTableIndex = m_VTable.GetCount ();
		m_VTable.Append (pBaseClassType->m_VTable);
		m_pVTableStructType->Append (pBaseClassType->m_pVTableStructType);

		m_BaseTypePrimeArray.Append (pSlot);

		if (pBaseClassType->m_pDestructor)
			m_BaseTypeDestructArray.Append (pBaseClassType);
	}

	// finalize iface layout

	Result = m_pIfaceStructType->EnsureLayout ();
	if (!Result)
		return false;

	// scan members for constructors & destructors 

	size_t Count = m_MemberFieldArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CStructField* pField = m_MemberFieldArray [i];
		CType* pType = pField->GetType ();
		
		if (pType->GetFlags () & ETypeFlag_GcRoot)
			m_Flags |= ETypeFlag_GcRoot;

		if ((pType->GetTypeKindFlags () & ETypeKindFlag_Derivable) && ((CDerivableType*) pType)->GetConstructor ())
			m_MemberFieldConstructArray.Append (pField);

		if (pType->GetTypeKind () == EType_Class)
		{
			if (pType->GetFlags () & EClassTypeFlag_Abstract)
			{
				err::SetFormatStringError ("cannot instantiate abstract '%s'", pType->GetTypeString ().cc ()); 
				return false;
			}

			m_ClassMemberFieldArray.Append (pField);

			if (((CClassType*) pType)->GetDestructor ())
				m_MemberFieldDestructArray.Append (pField);
		}
	}

	Count = m_MemberPropertyArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CProperty* pProperty = m_MemberPropertyArray [i];
		Result = pProperty->EnsureLayout ();
		if (!Result)
			return false;

		if (pProperty->GetConstructor ())
			m_MemberPropertyConstructArray.Append (pProperty);

		if (pProperty->GetDestructor ())
			m_MemberPropertyDestructArray.Append (pProperty);
	}

	// update base type llvm indexes & offsets

	Slot = m_BaseTypeList.GetHead ();
	for (size_t i = 0; Slot; i++, Slot++)
	{
		CBaseTypeSlot* pSlot = *Slot;
		CBaseTypeSlot* pIfaceSlot = IfaceBaseTypeArray [i];

		Slot->m_LlvmIndex = pIfaceSlot->m_LlvmIndex;
		Slot->m_Offset = pIfaceSlot->m_Offset;
	}

	// layout virtual properties

	Count = m_VirtualPropertyArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CProperty* pProperty = m_VirtualPropertyArray [i];
		ASSERT (pProperty->m_StorageKind == EStorage_Abstract || pProperty->m_StorageKind == EStorage_Virtual);

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
		ASSERT (pFunction->m_StorageKind == EStorage_Abstract || pFunction->m_StorageKind == EStorage_Virtual);

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

	Result = m_pVTableStructType->EnsureLayout ();
	if (!Result)
		return false;

	m_pClassStructType->EnsureLayout ();

	CreateVTablePtr ();

	if (!m_pStaticConstructor && m_pStaticDestructor)
	{
		Result = CreateDefaultMethod (EFunction_StaticConstructor, EStorage_Static);
		if (!Result)
			return false;
	}

	if (m_pStaticConstructor)
		m_pStaticOnceFlagVariable = m_pModule->m_VariableMgr.CreateOnceFlagVariable ();

	if (m_pStaticDestructor)
		m_pModule->m_VariableMgr.AddToStaticDestructList (m_pStaticOnceFlagVariable, m_pStaticDestructor);

	if (!m_pPreConstructor && 
		(m_pStaticConstructor ||
		!m_pIfaceStructType->GetInitializedFieldArray ().IsEmpty ()))
	{
		Result = CreateDefaultMethod (EFunction_PreConstructor);
		if (!Result)
			return false;
	}

	if (!m_pConstructor && 
		(m_pPreConstructor || 
		!m_BaseTypeConstructArray.IsEmpty () ||
		!m_MemberFieldConstructArray.IsEmpty () ||
		!m_MemberPropertyConstructArray.IsEmpty ()))
	{
		Result = CreateDefaultMethod (EFunction_Constructor);
		if (!Result)
			return false;
	}

	if (!m_pDestructor && 
		(!m_BaseTypeDestructArray.IsEmpty () || 
		!m_MemberFieldDestructArray.IsEmpty () ||
		!m_MemberPropertyDestructArray.IsEmpty ()))
	{
		Result = CreateDefaultMethod (EFunction_Destructor);
		if (!Result)
			return false;
	}

	if (!(m_Flags & EClassTypeFlag_Abstract))
		CreatePrimer ();

	m_Size = m_pClassStructType->GetSize ();
	m_AlignFactor = m_pClassStructType->GetAlignFactor ();
	return true;
}

void
CClassType::AddVirtualFunction (CFunction* pFunction)
{
	ASSERT (pFunction->m_StorageKind == EStorage_Abstract || pFunction->m_StorageKind == EStorage_Virtual);
	ASSERT (pFunction->m_pVirtualOriginClassType == NULL); // not layed out yet

	pFunction->m_pVirtualOriginClassType = this;
	pFunction->m_ClassVTableIndex = m_VTable.GetCount ();

	CFunctionPtrType* pPointerType = pFunction->GetType ()->GetFunctionPtrType (EFunctionPtrType_Thin, EPtrTypeFlag_Unsafe);
	m_pVTableStructType->CreateField (pPointerType);
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
		err::SetFormatStringError ("cannot override '%s': method not found", pFunction->m_Tag.cc ());
		return false;
	}

	CFunction* pOverridenFunction;

	EModuleItem ItemKind = pMember->GetItemKind ();
	switch (ItemKind)
	{
	case EModuleItem_Function:
		if (FunctionKind != EFunction_Named)
		{
			err::SetFormatStringError (
				"cannot override '%s': function kind mismatch", 
				pFunction->m_Tag.cc () 
				);
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
				err::SetFormatStringError ("cannot override '%s': property has no setter", pFunction->m_Tag.cc ());
				return false;
			}

			break;

		default:
			err::SetFormatStringError ("cannot override '%s': function kind mismatch", pFunction->m_Tag.cc ());
			return false;
		}

		break;

	default:
		err::SetFormatStringError ("cannot override '%s': not a method or property", pFunction->m_Tag.cc ());
		return false;
	}
	
	pOverridenFunction = pOverridenFunction->FindShortOverload (pFunction->GetType ()->GetShortType ());
	if (!pOverridenFunction)
	{
		err::SetFormatStringError ("cannot override '%s': method signature mismatch", pFunction->m_Tag.cc ());
		return false;
	}

	if (!pOverridenFunction->IsVirtual ())
	{
		err::SetFormatStringError ("cannot override '%s': method is not virtual", pFunction->m_Tag.cc ());
		return false;
	}

	#pragma AXL_TODO ("virtual multipliers")

	pFunction->m_pType = pOverridenFunction->m_pType;
	pFunction->m_pThisArgType = pOverridenFunction->m_pThisArgType;
	pFunction->m_ThisArgDelta = -BaseTypeCoord.m_Offset;
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
		m_VTablePtrValue = m_pVTableStructType->GetDataPtrType (EDataPtrType_Thin, EPtrTypeFlag_Unsafe)->GetZeroValue ();
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
	VariableTag.Format ("%s.vtbl", GetQualifiedName ().cc ()); 
	llvm::GlobalVariable* pLlvmVTableVariable = new llvm::GlobalVariable (
			*m_pModule->m_pLlvmModule,
			m_pVTableStructType->GetLlvmType (),
			false,
			llvm::GlobalVariable::ExternalLinkage,
			pLlvmVTableConstant,
			(const char*) VariableTag
			);

	m_VTablePtrValue.SetLlvmValue (
		pLlvmVTableVariable, 
		m_pVTableStructType->GetDataPtrType (EDataPtrType_Thin, EPtrTypeFlag_Unsafe),
		EValue_Const
		);
}

bool
CClassType::Compile ()
{
	bool Result;

	if (m_pStaticConstructor && !(m_pStaticConstructor->GetFlags () & EModuleItemFlag_User))
	{
		Result = CompileDefaultStaticConstructor ();
		if (!Result)
			return false;
	}

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

	if (m_pDestructor && !(m_pDestructor->GetFlags () & EModuleItemFlag_User))
	{
		Result = CompileDefaultDestructor ();
		if (!Result)
			return false;
	}

	if (m_pPrimer)
	{
		Result = CompilePrimer ();
		if (!Result)
			return false;
	}

	return true;
}

bool
CClassType::CompileDefaultPreConstructor ()
{
	ASSERT (m_pPreConstructor);

	bool Result;

	CValue ThisValue;
	m_pModule->m_FunctionMgr.InternalPrologue (m_pPreConstructor, &ThisValue, 1);

	Result = m_pIfaceStructType->InitializeFields (ThisValue);
	if (!Result)
		return false;

	m_pModule->m_FunctionMgr.InternalEpilogue ();
	return true;
}

bool
CClassType::CompileDefaultDestructor ()
{
	ASSERT (m_pDestructor);

	bool Result;
	
	CValue ArgValue;
	m_pModule->m_FunctionMgr.InternalPrologue (m_pDestructor, &ArgValue, 1);

	Result = 
		CallMemberPropertyDestructors (ArgValue) &&
		CallMemberFieldDestructors (ArgValue) &&
		CallBaseTypeDestructors (ArgValue);

	if (!Result)
		return false;

	m_pModule->m_FunctionMgr.InternalEpilogue ();
	return true;
}

bool
CClassType::CallMemberFieldDestructors (const CValue& ThisValue)
{
	bool Result;

	// TODO: only call member field destructors if class is allocated on stack

/*
		static int32_t LlvmIndexArray [] = 
		{
			0, // TObjectHdr**
			0, // TObjectHdr*
			2, // intptr_t m_Flags
		};

		CValue FlagsValue;
		m_pModule->m_LlvmBuilder.CreateGep (
			FieldValue, 
			LlvmIndexArray, 
			countof (LlvmIndexArray), 
			NULL, 
			&FlagsValue
			);
*/

	size_t Count = m_MemberFieldDestructArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CStructField* pField = m_MemberFieldDestructArray [i];
		CType* pType = pField->GetType ();

		ASSERT (pType->GetTypeKind () == EType_Class);

		CFunction* pDestructor = ((CClassType*) pType)->GetDestructor ();
		ASSERT (pDestructor);

		CValue FieldValue;
		Result = 
			m_pModule->m_OperatorMgr.GetClassField (ThisValue, pField, NULL, &FieldValue) &&
			m_pModule->m_OperatorMgr.CallOperator (pDestructor, FieldValue);

		if (!Result)
			return false;
	}

	return true;
}

bool
CClassType::CallMemberPropertyDestructors (const CValue& ThisValue)
{
	bool Result;

	size_t Count = m_MemberPropertyDestructArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CProperty* pProperty = m_MemberPropertyDestructArray [i];

		CFunction* pDestructor = pProperty->GetDestructor ();
		ASSERT (pDestructor);

		Result = m_pModule->m_OperatorMgr.CallOperator (pDestructor, ThisValue);
		if (!Result)
			return false;				
	}

	return true;
}

bool
CClassType::CallBaseTypeDestructors (const CValue& ThisValue)
{
	bool Result;

	size_t Count = m_BaseTypeDestructArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CClassType* pBaseClassType = m_BaseTypeDestructArray [i];
		CFunction* pDestructor = pBaseClassType->GetDestructor ();		
		ASSERT (pDestructor);

		Result = m_pModule->m_OperatorMgr.CallOperator (pDestructor, ThisValue);
		if (!Result)
			return false;
	}
		
	return true;
}

void
CClassType::CreatePrimer ()
{
	CType* ArgTypeArray [] =
	{
		GetClassStructType ()->GetDataPtrType (EDataPtrType_Thin, EPtrTypeFlag_Unsafe),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p)
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray, countof (ArgTypeArray));
	m_pPrimer = m_pModule->m_FunctionMgr.CreateFunction (EFunction_Primer, pType);
	m_pPrimer->m_Tag = m_Tag + ".prime";

	m_pModule->MarkForCompile (this);
}

bool
CClassType::CompilePrimer ()
{
	ASSERT (m_pPrimer);

	CValue ArgValueArray [3];
	m_pModule->m_FunctionMgr.InternalPrologue (m_pPrimer, ArgValueArray, countof (ArgValueArray));

	CValue ArgValue1 = ArgValueArray [0];
	CValue ArgValue2 = ArgValueArray [1];
	CValue ArgValue3 = ArgValueArray [2];

	CClassType* pClassType = this;
	CValue TypeValue (&pClassType, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr));
	CValue ObjectPtrValue;
	CValue IfacePtrValue;
	CValue PtrValue;

	m_pModule->m_LlvmBuilder.CreateGep2 (ArgValue1, 0, NULL, &ObjectPtrValue);
	m_pModule->m_LlvmBuilder.CreateGep2 (ArgValue1, 1, NULL, &IfacePtrValue);

	// zero memory

	m_pModule->m_LlvmBuilder.CreateStore (GetZeroValue (), ArgValue1);

	// store CClassType*

	m_pModule->m_LlvmBuilder.CreateGep2 (ObjectPtrValue, 0, NULL, &PtrValue);
	m_pModule->m_LlvmBuilder.CreateStore (TypeValue, PtrValue);

	// store ScopeLevel

	m_pModule->m_LlvmBuilder.CreateGep2 (ObjectPtrValue, 1, NULL, &PtrValue);
	m_pModule->m_LlvmBuilder.CreateStore (ArgValue2, PtrValue);

	// store Flags

	m_pModule->m_LlvmBuilder.CreateGep2 (ObjectPtrValue, 2, NULL, &PtrValue);
	m_pModule->m_LlvmBuilder.CreateStore (ArgValue3, PtrValue);

	PrimeInterface (this, ObjectPtrValue, IfacePtrValue, m_VTablePtrValue);

	// prime class members fields

	size_t Count = pClassType->m_ClassMemberFieldArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CStructField* pField = pClassType->m_ClassMemberFieldArray [i];
		
		ASSERT (pField->m_pType->GetTypeKind () == EType_Class);
		CClassType* pClassType = (CClassType*) pField->m_pType;

		CValue FieldValue;
		m_pModule->m_LlvmBuilder.CreateGep2 (
			IfacePtrValue, 
			pField->GetLlvmIndex (), 
			pClassType->GetClassStructType ()->GetDataPtrType (EDataPtrType_Thin, EPtrTypeFlag_Unsafe), 
			&FieldValue
			);		

		CFunction* pPrimer = pClassType->GetPrimer ();
		ASSERT (pPrimer); // should have been checked during CalcLayout

		rtl::CBoxListT <CValue> ArgList;
		ArgList.InsertTail (FieldValue);
		ArgList.InsertTail (ArgValue2);
		ArgList.InsertTail (ArgValue3);

		bool Result = m_pModule->m_OperatorMgr.CallOperator (pPrimer, &ArgList);
		ASSERT (Result);
	}

	m_pModule->m_FunctionMgr.InternalEpilogue ();
	return true;
}

bool
CClassType::PrimeInterface (
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

	// prime base types

	size_t Count = pClassType->m_BaseTypePrimeArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CBaseTypeSlot* pSlot = pClassType->m_BaseTypePrimeArray [i];
		ASSERT (pSlot->m_pType->GetTypeKind () == EType_Class);

		CClassType* pBaseClassType = (CClassType*) pSlot->m_pType;

		CValue BaseClassPtrValue;
		m_pModule->m_LlvmBuilder.CreateGep2 (
			IfacePtrValue, 
			pSlot->GetLlvmIndex (), 
			NULL, 
			&BaseClassPtrValue
			);

		CValue BaseClassVTablePtrValue;

		if (!pBaseClassType->HasVTable ())
		{
			BaseClassVTablePtrValue = pBaseClassType->GetVTableStructType ()->GetDataPtrType (EDataPtrType_Thin, EPtrTypeFlag_Unsafe)->GetZeroValue ();
		}
		else
		{
			m_pModule->m_LlvmBuilder.CreateGep2 (
				VTablePtrValue, 
				pSlot->GetVTableIndex (), 
				NULL, 
				&BaseClassVTablePtrValue
				);

			m_pModule->m_LlvmBuilder.CreateBitCast (
				BaseClassVTablePtrValue, 
				pBaseClassType->GetVTableStructType ()->GetDataPtrType (EDataPtrType_Thin, EPtrTypeFlag_Unsafe),
				&BaseClassVTablePtrValue
				);
		}		

		PrimeInterface (pBaseClassType, ObjectPtrValue, BaseClassPtrValue, BaseClassVTablePtrValue);
	}

	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
