#include "pch.h"
#include "axl_jnc_Property.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CProperty::CProperty ()
{
	m_ItemKind = EModuleItem_Property;
	m_NamespaceKind = ENamespace_Property;
	m_pItemDecl = this;
	m_pType = NULL;

	m_pPreConstructor = NULL;
	m_pConstructor = NULL;
	m_pDefaultConstructor = NULL;
	m_pStaticConstructor = NULL;
	m_pDestructor = NULL;
	m_pStaticDestructor = NULL;
	m_pGetter = NULL;
	m_pSetter = NULL;
	m_pBinder = NULL;

	m_pPropValue = NULL;
	m_pOnChange = NULL;

	m_pParentType = NULL;
	m_ParentClassVTableIndex = -1;
}

CFunction* 
CProperty::GetDefaultConstructor ()
{
	ASSERT (m_pConstructor);
	if (m_pDefaultConstructor)
		return m_pDefaultConstructor;

	// avoid allocations

	rtl::CBoxListEntryT <CValue> ThisArgValue;
	rtl::CAuxListT <rtl::CBoxListEntryT <CValue> > ArgList;

	if (m_pParentType)
	{
		CType* pThisArgType = m_pParentType->GetThisArgType (EPtrTypeFlag_Checked);
		ThisArgValue.m_Value.SetType (pThisArgType);
		ArgList.InsertTail (&ThisArgValue);
	}

	m_pDefaultConstructor = m_pConstructor->ChooseOverload (ArgList);
	if (!m_pDefaultConstructor)
	{
		err::SetFormatStringError ("'%s' has no default constructor", m_Tag.cc ()); // thanks a lot gcc
		return NULL;
	}

	return m_pDefaultConstructor;
}

bool
CProperty::Compile ()
{
	bool Result;

	if (m_Flags & EPropertyFlag_AutoGet)
	{
		Result = CompileAutoGetter ();
		if (!Result)
			return false;
	}

	if (m_Flags & EPropertyFlag_AutoSet)
	{
		Result = CompileAutoSetter ();
		if (!Result)
			return false;
	}

	if (m_pType->GetFlags () & EPropertyTypeFlag_Bindable)
	{
		Result = CompileBinder ();
		if (!Result)
			return false;
	}

	return true;
}

CPropertyType*
CProperty::CalcType ()
{
	ASSERT (!m_pType);

	if (!m_pGetter)
	{
		err::SetFormatStringError ("incomplete property: no 'get' method / 'propvalue' field");
		return NULL;
	}

	uint_t TypeFlags = 0;
	if (m_pOnChange)
		TypeFlags |= EPropertyTypeFlag_Bindable;

	m_pType = m_pSetter ?
		m_pModule->m_TypeMgr.GetPropertyType (m_pGetter->GetType (), *m_pSetter->GetTypeOverload (), TypeFlags) :
		m_pModule->m_TypeMgr.GetPropertyType (m_pGetter->GetType (), NULL, TypeFlags);

	if (m_Flags & (EPropertyFlag_AutoGet | EPropertyFlag_AutoSet))
		m_pModule->MarkForCompile (this);

	return m_pType;
}

bool
CProperty::Create (CPropertyType* pType)
{
	bool Result;

	EStorage StorageKind = m_StorageKind == EStorage_Abstract ? EStorage_Virtual : m_StorageKind;

	uint_t GetterFlags = 0;
	uint_t SetterFlags = 0;
	
	if (m_Flags & EModuleItemFlag_User) 
	{
		if (!(m_Flags & EPropertyFlag_AutoGet))
			GetterFlags |= EModuleItemFlag_User;

		if (!(m_Flags & EPropertyFlag_AutoSet))
			SetterFlags |= EModuleItemFlag_User;
	}

	if (pType->GetFlags () & EPropertyTypeFlag_Bindable)
	{
		Result = CreateOnChange ();
		if (!Result)
			return false;
	}

	CFunctionType* pGetterType = pType->GetGetterType ();

	if (m_Flags & EPropertyFlag_AutoGet)
	{
		Result = CreatePropValue (pGetterType->GetReturnType ());
		if (!Result)
			return false;
	}
	else
	{
		CFunction* pGetter = m_pModule->m_FunctionMgr.CreateFunction (EFunction_Getter, pGetterType);
		pGetter->m_StorageKind = StorageKind;
		pGetter->m_Flags |= GetterFlags;

		if (m_pParentType)
			pGetter->m_ThisArgTypeFlags = EPtrTypeFlag_Const;

		Result = AddMethod (pGetter);
		if (!Result)
			return false;
	}

	size_t SetterTypeOverloadCount = pType->GetSetterType ()->GetOverloadCount ();
	for (size_t i = 0; i < SetterTypeOverloadCount; i++)
	{
		CFunctionType* pSetterType = pType->GetSetterType ()->GetOverload (i);
		CFunction* pSetter = m_pModule->m_FunctionMgr.CreateFunction (EFunction_Setter, pSetterType);
		pSetter->m_StorageKind = StorageKind;
		pSetter->m_Flags |= SetterFlags;

		Result = AddMethod (pSetter);
		if (!Result)
			return false;
	}

	m_pType = m_pParentType ? m_pParentType->GetMemberPropertyType (pType) : pType;

	if (m_Flags & (EPropertyFlag_AutoGet | EPropertyFlag_AutoSet))
		m_pModule->MarkForCompile (this);

	return true;
}

void
CProperty::ConvertToMemberProperty (CNamedType* pParentType)
{
	ASSERT (!m_pParentType);
	m_pParentType = pParentType;
	m_pType = pParentType->GetMemberPropertyType (m_pType);
}

bool
CProperty::CreateOnChange ()
{
	if (m_pOnChange)
	{
		err::SetFormatStringError ("'%s' already has 'onchange'", m_Tag.cc ());
		return false;
	}

	CType* pType = m_pModule->GetSimpleType (EStdType_SimpleMulticast);
	
	if (m_pParentType)
	{
		m_pOnChange = CreateField (pType);
	}
	else
	{
		m_pOnChange = m_pModule->m_VariableMgr.CreateVariable (
			EStorage_Static, 
			"onchange", 
			CreateQualifiedName ("onchange"), 
			pType
			);
	}

	CFunctionType* pBinderType = (CFunctionType*) m_pModule->GetSimpleType (EStdType_Binder);
	CFunction* pBinder = m_pModule->m_FunctionMgr.CreateFunction (EFunction_Binder, pBinderType);
	pBinder->m_StorageKind = m_StorageKind == EStorage_Abstract ? EStorage_Virtual : m_StorageKind;

	if (m_pParentType)
		pBinder->m_ThisArgTypeFlags = EPtrTypeFlag_Const;

	m_pModule->MarkForCompile (this);
	
	return AddMethod (pBinder);
}

bool
CProperty::CreatePropValue (
	CType* pType,
	size_t BitCount,
	uint_t PtrTypeFlags,
	rtl::CBoxListT <CToken>* pConstructor,
	rtl::CBoxListT <CToken>* pInitializer
	)
{
	if (m_pPropValue)
	{
		err::SetFormatStringError ("'%s' already has 'propvalue'", m_Tag.cc ());
		return false;
	}

	if (m_pParentType)
	{
		m_pPropValue = CreateField (
			rtl::CString (), 
			pType, 
			BitCount, 
			PtrTypeFlags, 
			pConstructor, 
			pInitializer
			);
	}
	else
	{
		m_pPropValue = m_pModule->m_VariableMgr.CreateVariable (
			EStorage_Static, 
			"propvalue", 
			CreateQualifiedName ("propvalue"), 
			pType,
			PtrTypeFlags,
			pConstructor,
			pInitializer
			);
	}

	m_Flags |= EPropertyFlag_AutoGet;

	CFunctionType* pGetterType = m_pModule->m_TypeMgr.GetFunctionType (pType, NULL, 0, 0);
	CFunction* pGetter = m_pModule->m_FunctionMgr.CreateFunction (EFunction_Getter, pGetterType);
	pGetter->m_StorageKind = m_StorageKind == EStorage_Abstract ? EStorage_Virtual : m_StorageKind;

	if (m_pParentType)
		pGetter->m_ThisArgTypeFlags = EPtrTypeFlag_Const;

	m_pModule->MarkForCompile (this);

	return AddMethod (pGetter);	
}


CStructField*
CProperty::CreateField (
	const rtl::CString& Name,
	CType* pType,
	size_t BitCount,
	uint_t PtrTypeFlags,
	rtl::CBoxListT <CToken>* pConstructor,
	rtl::CBoxListT <CToken>* pInitializer
	)
{
	ASSERT (m_pParentType);
	
	if (!(m_pParentType->GetTypeKindFlags () & ETypeKindFlag_Derivable))
	{
		err::SetFormatStringError ("'%s' cannot have field members", m_pParentType->GetTypeString ().cc ());
		return NULL;
	}

	CDerivableType* pParentType = (CDerivableType*) m_pParentType;

	bool Result;

	CStructField* pField = pParentType->CreateField (Name, pType, BitCount, PtrTypeFlags, pConstructor, pInitializer);
	if (!pField)
		return NULL;

	// re-parent

	pField->m_pParentNamespace = this;

	if (!Name.IsEmpty ())
	{
		Result = AddItem (pField);
		if (!Result)
			return NULL;
	}
	
	return pField;
}

bool
CProperty::AddMethod (CFunction* pFunction)
{
	bool Result;

	EStorage StorageKind = pFunction->GetStorageKind ();
	EFunction FunctionKind = pFunction->GetFunctionKind ();
	uint_t FunctionKindFlags = GetFunctionKindFlags (FunctionKind);
	uint_t ThisArgTypeFlags = pFunction->m_ThisArgTypeFlags;

	if (m_pParentType)
	{
		switch (StorageKind)
		{
		case EStorage_Static:
			if (ThisArgTypeFlags)
			{
				err::SetFormatStringError ("static method cannot be '%s'", GetPtrTypeFlagString (ThisArgTypeFlags).cc ());
				return false;
			}

			break;

		case EStorage_Undefined:
			pFunction->m_StorageKind = EStorage_Member;
			// and fall through
		
		case EStorage_Member:
			pFunction->ConvertToMemberMethod (m_pParentType);
			break;

		case EStorage_Abstract:
		case EStorage_Virtual:
		case EStorage_Override:
			if (m_pParentType->GetTypeKind () != EType_Class)
			{
				err::SetFormatStringError (
					"'%s' method cannot be part of '%s'", 
					GetStorageKindString (StorageKind), 
					m_pParentType->GetTypeString ().cc ()
					);
				return false;
			}

			if (!pFunction->IsAccessor ())
				((CClassType*) m_pParentType)->m_VirtualMethodArray.Append (pFunction); // otherwise we are already on VirtualPropertyArray

			pFunction->ConvertToMemberMethod (m_pParentType);
			break;

		default:
			err::SetFormatStringError ("invalid storage specifier '%s' for method member", GetStorageKindString (StorageKind));
			return false;
		}
	}
	else
	{
		switch (StorageKind)
		{
		case EStorage_Undefined:
			pFunction->m_StorageKind = EStorage_Static;
			// and fall through

		case EStorage_Static:
			break;

		default:
			err::SetFormatStringError ("invalid storage specifier '%s' for static property member", GetStorageKindString (StorageKind));
			return false;
		}	

		if (ThisArgTypeFlags)
		{
			err::SetFormatStringError ("global property methods cannot be '%s'", GetPtrTypeFlagString (ThisArgTypeFlags).cc ());
			return false;
		}

	}

	pFunction->m_pParentNamespace = this;
	pFunction->m_pProperty = this;
	
	CFunction** ppTarget = NULL;

	switch (FunctionKind)
	{
	case EFunction_Constructor:
		ppTarget = &m_pConstructor;
		break;

	case EFunction_StaticConstructor:
		ppTarget = &m_pStaticConstructor;
		break;

	case EFunction_Destructor:
		ppTarget = &m_pDestructor;
		break;

	case EFunction_Getter:
		Result = m_Verifier.CheckGetter (pFunction->GetType ());
		if (!Result)
			return false;

		ppTarget = &m_pGetter;
		break;

	case EFunction_Setter:
		Result = m_Verifier.CheckSetter (pFunction->GetType ());
		if (!Result)
			return false;

		ppTarget = &m_pSetter;
		break;

	case EFunction_Binder:
		ppTarget = &m_pBinder;
		break;

	case EFunction_Named:
		return AddFunction (pFunction);

	default:
		err::SetFormatStringError (
			"invalid %s in '%s'", 
			GetFunctionKindString (FunctionKind), 
			m_Tag.cc ()
			);
		return false;
	}

	pFunction->m_Tag.Format ("%s.%s", m_Tag.cc (), GetFunctionKindString (FunctionKind));

	if (!*ppTarget)
	{
		*ppTarget = pFunction;
	}
	else
	{
		Result = (*ppTarget)->AddOverload (pFunction);
		if (!Result)
			return false;
	}

	return true;
}

bool
CProperty::AddProperty (CProperty* pProperty)
{
	ASSERT (pProperty->IsNamed ());
	bool Result = AddItem (pProperty);
	if (!Result)
		return false;

	pProperty->m_pParentNamespace = this;

	if (!m_pParentType)
		return true;

	EStorage StorageKind = pProperty->GetStorageKind ();
	switch (StorageKind)
	{
	case EStorage_Static:
		break;

	case EStorage_Undefined:
		pProperty->m_StorageKind = EStorage_Member;
		// and fall through

	case EStorage_Member:
		pProperty->m_pParentType = m_pParentType;
		break;

	case EStorage_Abstract:
	case EStorage_Virtual:
	case EStorage_Override:
		if (m_pParentType->GetTypeKind () != EType_Class)
		{
			err::SetFormatStringError (
				"'%s' property cannot be part of '%s'", 
				GetStorageKindString (StorageKind), 
				m_pParentType->GetTypeString ().cc ()
				);
			return false;
		}

		((CClassType*) m_pParentType)->m_VirtualPropertyArray.Append (pProperty);
		pProperty->m_pParentType = m_pParentType;
		break;

	default:
		err::SetFormatStringError ("invalid storage specifier '%s' for property member", GetStorageKindString (StorageKind));
		return false;
	}

	return true;
}

bool
CProperty::CallMemberFieldConstructors (const CValue& ThisValue)
{
	bool Result;

	size_t Count = m_MemberFieldConstructArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CStructField* pField = m_MemberFieldConstructArray [i];
		if (pField->m_Flags & EModuleItemFlag_Constructed)
		{
			pField->m_Flags &= ~EModuleItemFlag_Constructed;
			continue;
		}

		CType* pType = pField->GetType ();

		ASSERT (pType->GetTypeKindFlags () & ETypeKindFlag_Derivable);
		CFunction* pConstructor = ((CDerivableType*) pType)->GetDefaultConstructor ();
		if (!pConstructor)
			return false;

		CValue FieldValue;
		Result = 
			m_pModule->m_OperatorMgr.GetClassField (ThisValue, pField, NULL, &FieldValue) &&
			m_pModule->m_OperatorMgr.CallOperator (pConstructor, FieldValue);

		if (!Result)
			return false;
	}

	return true;
}

bool
CProperty::CallMemberPropertyConstructors (const CValue& ThisValue)
{
	bool Result;

	size_t Count = m_MemberPropertyConstructArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CProperty* pProperty = m_MemberPropertyConstructArray [i];
		if (pProperty->m_Flags & EModuleItemFlag_Constructed)
		{
			pProperty->m_Flags &= ~EModuleItemFlag_Constructed;
			continue;
		}

		CFunction* pConstructor = pProperty->GetDefaultConstructor ();
		if (!pConstructor)
			return false;

		Result = m_pModule->m_OperatorMgr.CallOperator (pConstructor, ThisValue);
		if (!Result)
			return false;				
	}

	return true;
}

bool
CProperty::CalcLayout ()
{
	ASSERT (m_StorageKind && m_VTable.IsEmpty ());

	size_t SetterCount = m_pSetter ? m_pSetter->GetOverloadCount () : 0;

	m_VTable.Reserve (2 + SetterCount);

	if (m_pBinder)
		m_VTable.Append (m_pBinder);

	m_VTable.Append (m_pGetter);

	for (size_t i = 0; i < SetterCount; i++)
	{
		CFunction* pSetter = m_pSetter->GetOverload (i);
		m_VTable.Append (pSetter);
	}

	CreateVTablePtr ();
	return true;
}

void
CProperty::CreateVTablePtr ()
{
	char Buffer [256];
	rtl::CArrayT <llvm::Constant*> LlvmVTable (ref::EBuf_Stack, Buffer, sizeof (Buffer));

	size_t Count = m_VTable.GetCount ();
	LlvmVTable.SetCount (Count);

	for (size_t i = 0; i < Count; i++)
	{
		CFunction* pFunction = m_VTable [i];

		if (pFunction->GetStorageKind () == EStorage_Abstract)
			pFunction = pFunction->GetType ()->GetAbstractFunction ();

		LlvmVTable [i] = pFunction->GetLlvmFunction ();
	}
	
	CStructType* pVTableStructType = m_pType->GetVTableStructType ();

	llvm::Constant* pLlvmVTableConstant = llvm::ConstantStruct::get (
		(llvm::StructType*) pVTableStructType->GetLlvmType (),
		llvm::ArrayRef <llvm::Constant*> (LlvmVTable, Count)
		);

	llvm::GlobalVariable* pLlvmVTableVariable = new llvm::GlobalVariable (
		*m_pModule->GetLlvmModule (),
		pVTableStructType->GetLlvmType (),
		false,
		llvm::GlobalVariable::InternalLinkage,
		pLlvmVTableConstant,
		(const char*) (m_Tag + ".vtbl")
		);

	m_VTablePtrValue.SetLlvmValue (
		pLlvmVTableVariable, 
		pVTableStructType->GetDataPtrType_c (),
		EValue_Const
		);
}

bool 
CProperty::CompileAutoGetter ()
{
	ASSERT (m_pGetter);

	bool Result;

	m_pModule->m_FunctionMgr.InternalPrologue (m_pGetter);
	
	CValue PropValue;
	Result = 
		m_pModule->m_OperatorMgr.GetPropertyPropValue (GetAutoAccessorPropertyValue (), &PropValue) &&
		m_pModule->m_ControlFlowMgr.Return (PropValue);

	if (!Result)
		return false;		
	
	m_pModule->m_FunctionMgr.InternalEpilogue ();
	return true;
}

bool 
CProperty::CompileAutoSetter ()
{
	ASSERT (m_pSetter && !m_pSetter->IsOverloaded ());

	bool Result;

	CValue SrcValue;

	if (IsMember ())
	{
		CValue ArgValueArray [2];
		m_pModule->m_FunctionMgr.InternalPrologue (m_pSetter, ArgValueArray, 2);
		SrcValue = ArgValueArray [1];
	}
	else
	{
		m_pModule->m_FunctionMgr.InternalPrologue (m_pSetter, &SrcValue, 1);
	}

	CValue PropValue;
	Result = 
		m_pModule->m_OperatorMgr.GetPropertyPropValue (GetAutoAccessorPropertyValue (), &PropValue) &&
		m_pModule->m_OperatorMgr.StoreDataRef (PropValue, SrcValue);

	if (!Result)
		return false;		

	if (m_pType->GetFlags () & EPropertyTypeFlag_Bindable)
	{
		Result = m_pModule->m_FunctionMgr.FireOnChangeEvent ();
		if (!Result)
			return false;
	}

	m_pModule->m_FunctionMgr.InternalEpilogue ();
	return true;
}

bool 
CProperty::CompileBinder ()
{
	ASSERT (m_pBinder);

	bool Result;

	m_pModule->m_FunctionMgr.InternalPrologue (m_pBinder);
	
	CValue OnChangeValue;
	Result = 
		m_pModule->m_OperatorMgr.GetPropertyOnChange (GetAutoAccessorPropertyValue (), &OnChangeValue) &&
		m_pModule->m_ControlFlowMgr.Return (OnChangeValue);

	if (!Result)
		return false;		
	
	m_pModule->m_FunctionMgr.InternalEpilogue ();
	return true;
}

CValue
CProperty::GetAutoAccessorPropertyValue ()
{
	if (!IsMember ())
		return this;

	CValue Value = this;
	Value.InsertToClosureHead (m_pModule->m_FunctionMgr.GetThisValue ());
	return Value;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
