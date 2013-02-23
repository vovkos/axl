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
	m_pType = NULL;
	m_TypeModifiers = 0;

	m_pConstructor = NULL;
	m_pStaticConstructor = NULL;
	m_pDestructor = NULL;
	m_pGetter = NULL;
	m_pSetter = NULL;

	m_pParentClassType = NULL;
	m_pParentClassFieldMember = NULL;
	m_ParentClassVTableIndex = -1;

	m_PackFactor = 8;
	m_pFieldStructType = NULL;
	m_pPropValue = NULL;
	m_pOnChangeEvent = NULL;
	m_pStaticFieldStructType = NULL;
	m_pStaticDataVariable = NULL;
}

CPropertyType*
CProperty::CalcType ()
{
	ASSERT (!m_pType);

	if ((m_TypeModifiers & ETypeModifier_AutoGet) && !m_pPropValue)
	{
		err::SetFormatStringError (_T("incomplete autoget property: no 'propvalue' field"));
		return NULL;
	}

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

	if (m_pParentClassType)
		pGetter->m_ThisArgTypeFlags = EPtrTypeFlag_Const;

	Result = AddMethodMember (pGetter);
	if (!Result)
		return false;

	size_t SetterTypeOverloadCount = pType->GetSetterType ()->GetOverloadCount ();
	for (size_t i = 0; i < SetterTypeOverloadCount; i++)
	{
		CFunctionType* pSetterType = pType->GetSetterType ()->GetOverload (i);
		CFunction* pSetter = m_pModule->m_FunctionMgr.CreateFunction (EFunction_Setter, pSetterType);
		pSetter->m_StorageKind = StorageKind;
		Result = AddMethodMember (pSetter);
		if (!Result)
			return false;
	}

	m_pType = m_pParentClassType ? m_pParentClassType->GetPropertyMemberType (pType) : pType;
	return true;
}

void
CProperty::ConvertToPropertyMember (CClassType* pClassType)
{
	ASSERT (!m_pParentClassType);
	m_pParentClassType = pClassType;
	m_pType = pClassType->GetPropertyMemberType (m_pType);
}

CStructField*
CProperty::CreateFieldMember (
	EStorage StorageKind,
	const rtl::CString& Name,
	CType* pType,
	size_t BitCount,
	int PtrTypeFlags
	)
{
	CStructType* pFieldStructType;

	if (!m_pParentClassType)
	{
		if (StorageKind)
		{
			err::SetFormatStringError (_T("global property members cannot have storage specifier"));
			return NULL;
		}

		StorageKind = EStorage_Static;
	}

	switch (StorageKind)
	{
	case EStorage_Mutable:
	case EStorage_Undefined:
		if (!m_pFieldStructType)
		{
			m_pFieldStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType (m_PackFactor);
			m_pFieldStructType->m_Tag.Format (_T("%s.field_struct"), m_Tag);
			m_pFieldStructType->m_pParentNamespace = this;

			if (m_pParentClassType)
				m_pParentClassFieldMember = m_pParentClassType->CreateFieldMember (StorageKind, m_pFieldStructType);
		}

		pFieldStructType = m_pFieldStructType;
		break;

	case EStorage_Static:
		if (!m_pStaticFieldStructType)
		{
			m_pStaticFieldStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType (m_PackFactor);
			m_pStaticFieldStructType->m_StorageKind = EStorage_Static;
			m_pStaticFieldStructType->m_Tag.Format (_T("%s.static_field_struct"), m_Tag);
			m_pStaticFieldStructType->m_pParentNamespace = this;
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
CProperty::AddMethodMember (CFunction* pFunction)
{
	bool Result;

	EStorage StorageKind = pFunction->GetStorageKind ();
	EFunction FunctionKind = pFunction->GetFunctionKind ();
	int FunctionKindFlags = GetFunctionKindFlags (FunctionKind);
	int ThisArgTypeFlags = pFunction->m_ThisArgTypeFlags;

	if (m_pParentClassType)
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

		case EStorage_Abstract:
		case EStorage_Virtual:
		case EStorage_Override:
			if (!pFunction->IsAccessor ())
				m_pParentClassType->m_VirtualMethodArray.Append (pFunction); // otherwise we are already on VirtualPropertyArray

			// and fall through;

		case EStorage_Undefined:
			pFunction->ConvertToMethodMember (m_pParentClassType);
			break;

		default:
			err::SetFormatStringError (_T("invalid storage specifier '%s' for method member"), GetStorageKindString (StorageKind));
			return false;
		}
	}
	else
	{
		if (StorageKind)
		{
			err::SetFormatStringError (_T("global property members cannot have storage specifier"));
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

	case EFunction_AutoEv:
		pFunction->m_Tag.Format (_T("%s.autoev"), m_Tag);
		return true;

	default:
		err::SetFormatStringError (_T("invalid %s in '%s'"), GetFunctionKindString (FunctionKind), m_Tag);
		return false;
	}

	Result = m_Verifier.AddMethodMember (FunctionKind, pFunction->GetType ());
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
CProperty::AddPropertyMember (CProperty* pProperty)
{
	EStorage StorageKind = pProperty->GetStorageKind ();

	if (m_pParentClassType)
	{
		switch (StorageKind)
		{
		case EStorage_Static:
			break;

		case EStorage_Abstract:
		case EStorage_Virtual:
		case EStorage_Override:
			m_pParentClassType->m_VirtualPropertyArray.Append (pProperty);
			// and fall through;

		case EStorage_Undefined:
			pProperty->m_pParentClassType = m_pParentClassType;
			break;

		default:
			err::SetFormatStringError (_T("invalid storage specifier '%s' for property member"), GetStorageKindString (StorageKind));
			return false;
		}
	}
	else if (StorageKind)
	{
		err::SetFormatStringError (_T("global property members cannot have storage specifier"));
		return NULL;
	}

	return true;
}

bool
CProperty::CalcLayout ()
{
	bool Result;

	if (!m_VTable.IsEmpty ())
		return true;

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
			_T("static_field"),
			m_Tag + _T(".static_field"), 
			m_pStaticFieldStructType
			);
	}

	if (!m_pSetter)
	{
		m_VTable.Copy (m_pGetter);
		return true;
	}

	size_t SetterCount = m_pSetter->GetOverloadCount ();
	m_VTable.SetCount (SetterCount + 1);
	m_VTable [0] = m_pGetter;

	for (size_t i = 0; i < SetterCount; i++)
	{
		CFunction* pSetter = m_pSetter->GetOverload (i);
		m_VTable [i + 1] = pSetter;
	}

	return true;
}

bool
CProperty::GetVTablePtrValue (CValue* pValue)
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

		#pragma AXL_TODO ("think about how to report undefined functions")

/*		if (!pFunction->IsDefined ())
		{
			err::SetFormatStringError (
				_T("cannot create property '%s': '%s' has no body"), 
				m_Tag,
				pFunction->m_Tag
				);
			return false;
		}
*/
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

	*pValue = m_VTablePtrValue;
	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
