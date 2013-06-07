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
	m_pPreConstructor = NULL;
	m_pDefaultConstructor = NULL;
	m_pDestructor = NULL;
	m_pInitializer = NULL;
	m_pVTableStructType = NULL;
	m_pClassPtrTypeTuple = NULL;
}

CClassPtrType* 
CClassType::GetClassPtrType (
	EClassPtrType PtrTypeKind,
	uint_t Flags
	)
{
	return m_pModule->m_TypeMgr.GetClassPtrType (this, PtrTypeKind, Flags);
}

CAutoEvType* 
CClassType::GetMemberAutoEvType (CAutoEvType* pShortType)
{
	return m_pModule->m_TypeMgr.GetMemberAutoEvType (this, pShortType);
}

CFunction* 
CClassType::GetDefaultConstructor ()
{
	ASSERT (m_pConstructor);
	if (m_pDefaultConstructor)
		return m_pDefaultConstructor;

	rtl::CBoxListEntryT <CValue> ThisArgValue;
	ThisArgValue.m_Value.SetType (GetThisArgType ());
	
	rtl::CAuxListT <rtl::CBoxListEntryT <CValue> > ArgList;
	ArgList.InsertTail (&ThisArgValue);

	CType* pThisArgType = GetThisArgType ();
	m_pDefaultConstructor = m_pConstructor->ChooseOverload (ArgList);
	if (!m_pDefaultConstructor)
	{
		err::SetFormatStringError (
			"'%s' does not provide a default constructor", 
			GetTypeString ().cc () // thanks a lot gcc
			);
		return NULL;
	}

	return m_pDefaultConstructor;
}

CStructField*
CClassType::GetStdField (EStdField Field)
{
	if (Field != EStdField_Value || !(m_Flags & EClassTypeFlag_Box))
	{
		err::SetFormatStringError (
			"'%s' has no field '%s'", 
			GetTypeString ().cc (), 
			GetStdFieldString (Field)
			);
		return NULL;
	}

	ASSERT (m_pIfaceStructType->GetFieldList ().GetCount () == 1);
	return *m_pIfaceStructType->GetFieldList ().GetHead ();	
}

CStructField*
CClassType::CreateField (
	const rtl::CString& Name,
	CType* pType,
	size_t BitCount,
	int PtrTypeFlags,
	rtl::CBoxListT <CToken>* pInitializer
	)
{
	CStructField* pField = m_pIfaceStructType->CreateField (Name, pType, BitCount, PtrTypeFlags, pInitializer);
	if (!pField)
		return NULL;

	if (!Name.IsEmpty ())
	{
		bool Result = AddItem (pField);
		if (!Result)
			return NULL;
	}

	return pField;
}

bool
CClassType::AddMemberNewType (CType* pType)
{
	bool Result = pType->CalcLayout ();
	if (!Result)
		return false;

	CStructField* pField;

	if (pType->GetTypeKind () != EType_Class)
	{
		pField = CreateField (pType);
	}
	else
	{
		CClassType* pClassType = (CClassType*) pType;

		pField = CreateField (pClassType->GetClassStructType ());

		if (pClassType->GetDestructor ())
			m_MemberDestructArray.Append (pField);
	}

	if (!m_FirstMemberNewField)
		m_FirstMemberNewField = pField;

	return true;
}

bool
CClassType::AddMethod (CFunction* pFunction)
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

	return true;
}

bool
CClassType::AddAutoEv (CAutoEv* pAutoEv)
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
		err::SetFormatStringError ("invalid storage '%s' for autoev member", GetStorageKindString (StorageKind));
		return false;
	}

	return true;
}

CFunction*
CClassType::CreateMethod (
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

	bool Result = AddMethod (pFunction);
	if (!Result)
		return NULL;

	return pFunction;
}

CFunction*
CClassType::CreateUnnamedMethod (
	EStorage StorageKind,
	EFunction FunctionKind,
	CFunctionType* pShortType
	)
{
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateFunction (FunctionKind, pShortType);
	pFunction->m_StorageKind = StorageKind;
	
	bool Result = AddMethod (pFunction);
	if (!Result)
		return NULL;

	return pFunction;
}

CProperty*
CClassType::CreateProperty (
	EStorage StorageKind,
	const rtl::CString& Name,
	CPropertyType* pShortType
	)
{
	rtl::CString QualifiedName = CreateQualifiedName (Name);

	CProperty* pProperty = m_pModule->m_FunctionMgr.CreateProperty (Name, QualifiedName);

	bool Result = 
		AddProperty (pProperty) &&
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
	
	if (m_pExtensionNamespace)
		ApplyExtensionNamespace ();

	// layout base types

	bool HasBaseConstructor = false;
	bool HasBaseDestructor = false;
	bool HasMemberDestructor = false;

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
			err::SetFormatStringError ("invalid base type '%s'", BaseType->m_pType->GetTypeString ().cc ());
			return false;
		}

		IfaceBaseTypeArray [i] = pIfaceBaseType;
	}

	// scan for member new operators 

	if (!m_pIfaceStructType->GetInitializedFieldArray ().IsEmpty ())
	{
		m_pModule->m_NamespaceMgr.OpenNamespace (this);

		Result = ScanInitializersForMemberNewOperators ();
		if (!Result)
			return false;

		HasMemberDestructor = !m_MemberDestructArray.IsEmpty ();

		m_pModule->m_NamespaceMgr.CloseNamespace ();
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
		if (!m_pPreConstructor && !m_pIfaceStructType->GetInitializedFieldArray ().IsEmpty ())
		{
			Result = CreateDefaultPreConstructor ();
			if (!Result)
				return false;
		}

		if (!m_pConstructor && (m_pPreConstructor || HasBaseConstructor))
		{
			Result = CreateDefaultConstructor ();
			if (!Result)
				return false;
		}

		if (!m_pDestructor && (!m_AutoEvArray.IsEmpty () || HasBaseDestructor || HasMemberDestructor))
		{
			Result = CreateDefaultDestructor ();
			if (!Result)
				return false;
		}
	}

	PostCalcLayout ();
	return true;
}

bool
CClassType::ScanInitializersForMemberNewOperators ()
{
	bool Result;

	size_t Count = m_pIfaceStructType->m_InitializedFieldArray.GetCount ();	
	for (size_t i = 0; i < Count; i++)
	{
		CStructField* pField = m_pIfaceStructType->m_InitializedFieldArray [i];

		CParser Parser;
		Parser.m_pModule = m_pModule;
		Parser.m_Stage = CParser::EStage_Pass2;
		Parser.m_pMemberNewTargetType = this;
		
		Result = Parser.ParseTokenList (ESymbol_expression_s, pField->GetInitializer());
		if (!Result)
			return false;
	}

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
	pFunction->m_Tag = m_Tag + ".this";

	size_t ArgCount = pStarter->GetType ()->GetArgArray ().GetCount ();
	
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
CClassType::CreateDefaultPreConstructor ()
{
	CFunctionType* pType = (CFunctionType*) m_pModule->m_TypeMgr.GetStdType (EStdType_SimpleFunction);

	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateFunction (EFunction_PreConstructor, pType);
	pFunction->m_StorageKind = EStorage_Member;
	
	bool Result = AddMethod (pFunction);
	if (!Result)
		return false;

	m_pModule->m_FunctionMgr.m_DefaultPreConstructorTypeArray.Append (this);
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
	pFunction->m_Tag = m_Tag + ".this";

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

	bool Result;

	CType* ArgTypeArray [] =
	{
		GetClassPtrType ()
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateFunction (EFunction_Destructor, pType);
	pFunction->m_Tag = m_Tag + ".~this";
		
	CValue ArgValue;
	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, &ArgValue, 1);

	Result = 
		StopAutoEvs (ArgValue) &&
		CallMemberNewDestructors (ArgValue) &&
		CallBaseDestructors (ArgValue);

	if (!Result)
		return false;

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	m_pDestructor = pFunction;
	return true;
}

bool
CClassType::CallMemberNewDestructors (const CValue& ThisValue)
{
	bool Result;

	if (m_MemberDestructArray.IsEmpty ())
		return true;

	CValue FlagsValue;

	size_t Count = m_MemberDestructArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CStructField* pField = m_MemberDestructArray [i];
		ASSERT (pField->GetType ()->GetTypeKind () == EType_Struct && ((CStructType*) pField->GetType ())->IsClassStructType ());

		CClassType* pClassType = (CClassType*) ((CStructType*) pField->GetType ())->GetParentNamespace ();				
		CFunction* pDestructor = pClassType->GetDestructor ();
		ASSERT (pDestructor);

		CValue FieldValue;
		Result = m_pModule->m_OperatorMgr.GetClassField (ThisValue, pField, NULL, &FieldValue);
		if (!Result)
			return false;

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

		CValue IfaceValue;
		m_pModule->m_LlvmBuilder.CreateGep2 (FieldValue, 1, pDestructor->GetThisArgType (), &IfaceValue);

		Result = m_pModule->m_OperatorMgr.CallOperator (pDestructor, IfaceValue);
		if (!Result)
			return false;				
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

CFunction* 
CClassType::GetInitializer ()
{
	if (m_pInitializer)
		return m_pInitializer;

	CType* ArgTypeArray [] =
	{
		GetClassStructType ()->GetDataPtrType (EDataPtrType_Unsafe),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p)
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction (m_Tag + ".init", pType);

	CValue ArgValueArray [countof (ArgTypeArray)];
	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, ArgValueArray, countof (ArgValueArray));

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

	// store CClassType*

	m_pModule->m_LlvmBuilder.CreateGep2 (ObjectPtrValue, 0, NULL, &PtrValue);
	m_pModule->m_LlvmBuilder.CreateStore (TypeValue, PtrValue);

	// store ScopeLevel

	m_pModule->m_LlvmBuilder.CreateGep2 (ObjectPtrValue, 1, NULL, &PtrValue);
	m_pModule->m_LlvmBuilder.CreateStore (ArgValue2, PtrValue);

	// store Flags

	m_pModule->m_LlvmBuilder.CreateGep2 (ObjectPtrValue, 2, NULL, &PtrValue);
	m_pModule->m_LlvmBuilder.CreateStore (ArgValue3, PtrValue);

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

//.............................................................................

} // namespace jnc {
} // namespace axl {