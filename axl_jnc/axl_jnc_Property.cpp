#include "stdafx.h"
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
	m_pAuPropValueType = NULL;
	m_TypeModifiers = 0;

	m_pConstructor = NULL;
	m_pStaticConstructor = NULL;
	m_pDestructor = NULL;
	m_pGetter = NULL;
	m_pSetter = NULL;

	m_pParentType = NULL;
	m_pParentTypeField = NULL;
	m_ParentClassVTableIndex = -1;

	m_PackFactor = 8;
	m_pDataStructType = NULL;
	m_pStaticDataVariable = NULL;
}

CPropertyType*
CProperty::CalcType ()
{
	ASSERT (!m_pType);

	if (!m_pGetter)
	{
		err::SetFormatStringError (_T("incomplete property: no 'get' method / 'propvalue' field"));
		return NULL;
	}

	int Flags = GetPropertyTypeFlagsFromModifiers (m_TypeModifiers);

	m_pType = m_pSetter ?
		m_pModule->m_TypeMgr.GetPropertyType (m_pGetter->GetType (), *m_pSetter->GetTypeOverload (), Flags) :
		m_pModule->m_TypeMgr.GetPropertyType (m_pGetter->GetType (), NULL, Flags);

	return m_pType;
}

bool
CProperty::Create (CPropertyType* pType)
{
	bool Result;

	EStorage StorageKind = m_StorageKind == EStorage_Abstract ? EStorage_Virtual : m_StorageKind;
	
	CFunction* pGetter = m_pModule->m_FunctionMgr.CreateFunction (EFunction_Getter, pType->GetGetterType ());
	pGetter->m_StorageKind = StorageKind;

	if (m_pParentType)
		pGetter->m_ThisArgTypeFlags = EPtrTypeFlag_Const;

	Result = AddMethod (pGetter);
	if (!Result)
		return false;

	size_t SetterTypeOverloadCount = pType->GetSetterType ()->GetOverloadCount ();
	for (size_t i = 0; i < SetterTypeOverloadCount; i++)
	{
		CFunctionType* pSetterType = pType->GetSetterType ()->GetOverload (i);
		CFunction* pSetter = m_pModule->m_FunctionMgr.CreateFunction (EFunction_Setter, pSetterType);
		pSetter->m_StorageKind = StorageKind;
		Result = AddMethod (pSetter);
		if (!Result)
			return false;
	}

	m_pType = m_pParentType ? m_pParentType->GetMemberPropertyType (pType) : pType;
	return true;
}

void
CProperty::ConvertToMemberProperty (CNamedType* pParentType)
{
	ASSERT (!m_pParentType);
	m_pParentType = pParentType;
	m_pType = pParentType->GetMemberPropertyType (m_pType);
}

CStructField*
CProperty::CreateField (
	const rtl::CString& Name,
	CType* pType,
	size_t BitCount,
	int PtrTypeFlags,
	rtl::CBoxListT <CToken>* pInitializer
	)
{
	if (!m_pDataStructType)
		CreateDataStructType ();

	CStructField* pField = m_pDataStructType->CreateField (Name, pType, BitCount, PtrTypeFlags, pInitializer);

	if (!Name.IsEmpty ())
	{
		bool Result = AddItem (pField);
		if (!Result)
			return NULL;
	}

	return pField;
}

CStructType*
CProperty::CreateDataStructType ()
{
	ASSERT (!m_pDataStructType);

	EType ParentTypeKind;

	m_pDataStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType (m_PackFactor);
	m_pDataStructType->m_Tag.Format (_T("%s.data_struct"), m_Tag);
	m_pDataStructType->m_pParentNamespace = this;

	if (m_pParentType)
	{
		ParentTypeKind = m_pParentType->GetTypeKind ();
		switch (ParentTypeKind)
		{
		case EType_Class:
			m_pParentTypeField = ((CClassType*) m_pParentType)->CreateField (m_pDataStructType);
			break;

		case EType_Struct:
			m_pParentTypeField = ((CStructType*) m_pParentType)->CreateField (m_pDataStructType);
			break;

		case EType_Union:
			m_pParentTypeField = ((CUnionType*) m_pParentType)->CreateField (m_pDataStructType);
			break;

		default:
			ASSERT (false);
		}
	}

	return m_pDataStructType;
}

bool
CProperty::AddMethod (CFunction* pFunction)
{
	bool Result;

	EStorage StorageKind = pFunction->GetStorageKind ();
	EFunction FunctionKind = pFunction->GetFunctionKind ();
	int FunctionKindFlags = GetFunctionKindFlags (FunctionKind);
	int ThisArgTypeFlags = pFunction->m_ThisArgTypeFlags;

	if (m_pParentType)
	{
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
			pFunction->ConvertToMemberMethod (m_pParentType);
			break;

		case EStorage_Abstract:
		case EStorage_Virtual:
		case EStorage_Override:
			if (m_pParentType->GetTypeKind () != EType_Class)
			{
				err::SetFormatStringError (_T("'%s' method cannot be part of '%s'"), GetStorageKindString (StorageKind), m_pParentType->GetTypeString ());
				return false;
			}

			if (!pFunction->IsAccessor ())
				((CClassType*) m_pParentType)->m_VirtualMethodArray.Append (pFunction); // otherwise we are already on VirtualPropertyArray

			pFunction->ConvertToMemberMethod (m_pParentType);
			break;

		default:
			err::SetFormatStringError (_T("invalid storage specifier '%s' for method member"), GetStorageKindString (StorageKind));
			return false;
		}
	}
	else
	{
		if (StorageKind != EStorage_Static)
		{
			err::SetFormatStringError (_T("invalid storage specifier '%s' for global property member"), GetStorageKindString (StorageKind));
			return false;
		}

		if (ThisArgTypeFlags)
		{
			err::SetFormatStringError (_T("global property methods cannot be '%s'"), GetPtrTypeFlagString (ThisArgTypeFlags));
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
		ppTarget = &m_pGetter;
		break;

	case EFunction_Setter:
		ppTarget = &m_pSetter;
		break;

	case EFunction_Named:
		return AddFunction (pFunction);

	default:
		err::SetFormatStringError (_T("invalid %s in '%s'"), GetFunctionKindString (FunctionKind), m_Tag);
		return false;
	}

	Result = m_Verifier.AddMethod (FunctionKind, pFunction->GetType ());
	if (!Result)
		return false;

	pFunction->m_Tag.Format (_T("%s.%s"), m_Tag, GetFunctionKindString (FunctionKind));

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
			err::SetFormatStringError (_T("'%s' property cannot be part of '%s'"), GetStorageKindString (StorageKind), m_pParentType->GetTypeString ());
			return false;
		}

		((CClassType*) m_pParentType)->m_VirtualPropertyArray.Append (pProperty);
		pProperty->m_pParentType = m_pParentType;
		break;

	default:
		err::SetFormatStringError (_T("invalid storage specifier '%s' for property member"), GetStorageKindString (StorageKind));
		return false;
	}

	return true;
}

bool
CProperty::AddAutoEv (CAutoEv* pAutoEv)
{
	ASSERT (pAutoEv->IsNamed ());
	bool Result = AddItem (pAutoEv);
	if (!Result)
		return false;

	pAutoEv->m_pParentNamespace = this;

	if (!m_pParentType)
		return true;

	EStorage StorageKind = pAutoEv->GetStorageKind ();
	switch (StorageKind)
	{
	case EStorage_Static:
		break;

	case EStorage_Undefined:
		pAutoEv->m_StorageKind = EStorage_Member;
		//and fall through

	case EStorage_Member:
		if (m_pParentType->GetTypeKind () != EType_Class)
		{
			err::SetFormatStringError (_T("autoev cannot be part of '%s'"), m_pParentType->GetTypeString ());
			return false;
		}

		((CClassType*) m_pParentType)->m_AutoEvArray.Append (pAutoEv);
		pAutoEv->m_pParentClassType = (CClassType*) m_pParentType;
		break;

	default:
		err::SetFormatStringError (_T("invalid storage '%s' for autoev member"), GetStorageKindString (StorageKind));
		return false;
	}

	return true;
}

bool
CProperty::CalcLayout ()
{
	bool Result;

	ASSERT (m_StorageKind);

	if (!m_VTable.IsEmpty ()) // done already
		return true;

	if (m_pType->GetFlags () & EPropertyTypeFlag_Augmented)
	{
		if (!m_pDataStructType)
			CreateDataStructType ();

		m_pDataStructType->AddBaseType (m_pType->GetAuFieldStructType ());			
	}

	if (m_pDataStructType)
	{
		Result = m_pDataStructType->CalcLayout ();
		if (!Result)
			return false;
	}

	if (m_pDataStructType && !m_pParentType)
	{
		Result = m_pDataStructType->CalcLayout ();
		if (!Result)
			return false;

		m_pStaticDataVariable = m_pModule->m_VariableMgr.CreateVariable (
			EStorage_Static,
			_T("static_data"),
			m_Tag + _T(".static_data"), 
			m_pDataStructType
			);	
	}

	size_t SetterCount = m_pSetter ? m_pSetter->GetOverloadCount () : 0;
	m_VTable.SetCount (SetterCount + 1);
	m_VTable [0] = m_pGetter;

	for (size_t i = 0; i < SetterCount; i++)
	{
		CFunction* pSetter = m_pSetter->GetOverload (i);
		m_VTable [i + 1] = pSetter;
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

	rtl::CString VariableTag;
	VariableTag.Format (_T("%s.vtbl"), GetQualifiedName ());
	llvm::GlobalVariable* pLlvmVTableVariable = new llvm::GlobalVariable (
			*m_pModule->m_pLlvmModule,
			pVTableStructType->GetLlvmType (),
			false,
			llvm::GlobalVariable::ExternalLinkage,
			pLlvmVTableConstant,
			(const tchar_t*) VariableTag
			);

	m_VTablePtrValue.SetLlvmValue (
		pLlvmVTableVariable, 
		pVTableStructType->GetDataPtrType (EDataPtrType_Unsafe),
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
		m_pModule->m_OperatorMgr.GetStdField (
			GetAutoAccessorPropertyValue (),
			EStdField_Value,
			&PropValue
			) &&
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
		m_pModule->m_OperatorMgr.GetStdField (
			GetAutoAccessorPropertyValue (),
			EStdField_Value,
			&PropValue
			) &&
		m_pModule->m_OperatorMgr.StoreDataRef (PropValue, SrcValue);

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
	CClosure* pClosure = Value.CreateClosure ();
	pClosure->GetArgList ()->InsertTail (m_pModule->m_FunctionMgr.GetThisValue ());
	return Value;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
