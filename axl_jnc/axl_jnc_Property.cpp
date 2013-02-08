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
	m_pEventMember = NULL;
	m_pStaticFieldStructType = NULL;
	m_pStaticDataVariable = NULL;
}

bool
CProperty::Create (CPropertyType* pType)
{
	bool Result;
	
	CFunction* pGetter = m_pModule->m_FunctionMgr.CreateFunction (EFunction_Getter, pType->GetGetterType ());
	Result = AddMethodMember (pGetter);
	if (!Result)
		return false;

	size_t SetterTypeOverloadCount = pType->GetSetterType ()->GetOverloadCount ();
	for (size_t i = 0; i < SetterTypeOverloadCount; i++)
	{
		CFunctionType* pSetterType = pType->GetSetterType ()->GetOverload (i);
		CFunction* pSetter = m_pModule->m_FunctionMgr.CreateFunction (EFunction_Setter, pSetterType);
		Result = AddMethodMember (pSetter);
		if (!Result)
			return false;
	}

	m_pType = m_pParentClassType ? m_pParentClassType->GetPropertyMemberType (pType) : pType;
	return true;
}

CStructMember*
CProperty::CreateFieldMember (
	EStorage StorageKind,
	const rtl::CString& Name,
	CType* pType,
	size_t BitCount
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

	if (!StorageKind)
	{
		if (!m_pFieldStructType)
		{
			m_pFieldStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType (m_PackFactor);
			m_pFieldStructType->m_pFieldParent = this;
			m_pFieldStructType->m_Tag.Format (_T("%s.field_struct"), m_Tag);

			if (m_pParentClassType)
				m_pParentClassFieldMember = m_pParentClassType->CreateFieldMember (StorageKind, m_pFieldStructType);
		}

		pFieldStructType = m_pFieldStructType;
	}
	else if (StorageKind == EStorage_Static)
	{
		if (!m_pStaticFieldStructType)
		{
			m_pStaticFieldStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType (m_PackFactor);
			m_pStaticFieldStructType->m_StorageKind = EStorage_Static;
			m_pStaticFieldStructType->m_pFieldParent = this;
			m_pStaticFieldStructType->m_Tag.Format (_T("%s.static_field_struct"), m_Tag);
		}

		pFieldStructType = m_pStaticFieldStructType;
	}
	else
	{
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
CProperty::AddMethodMember (CFunction* pFunction)
{
	bool Result;

	EStorage StorageKind = pFunction->GetStorageKind ();
	EFunction FunctionKind = pFunction->GetFunctionKind ();

	if (m_pParentClassType)
	{
		if (StorageKind != EStorage_Static)
		{
			pFunction->m_pClassType = m_pParentClassType;
			pFunction->m_pType = m_pParentClassType->GetMethodMemberType (pFunction->m_pType);

			if (StorageKind == EStorage_Virtual)
				m_pParentClassType->m_VirtualMethodArray.Append (pFunction);
		}
	}
	else
	{
		if (StorageKind)
		{
			err::SetFormatStringError (_T("global property members cannot have storage specifier"));
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
		pFunction->m_Tag.Format (_T("%s.%s"), m_Tag, pFunction->m_Name);
		return true;

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
		m_pParentClassType->AddPropertyMember (pProperty);
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
			m_Tag, 
			m_pStaticFieldStructType, 
			false
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
		if (!pFunction->IsDefined ())
		{
			err::SetFormatStringError (
				_T("cannot create property '%s': '%s' has no body"), 
				m_Tag,
				pFunction->m_Tag
				);
			return false;
		}

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

/*

bool
CPropertyType::CreateFromTemplate (CPropertyType* pTemplateType)
{
	bool Result;

	CFunction* pTemplateAccessor = pTemplateType->m_UnnamedMethodArray [EFunction_Getter]; 
	CFunction* pAccessor = m_pModule->m_FunctionMgr.CreateFunction (EFunction_Getter, pTemplateAccessor->GetType ());
	Result = AddMethodMember (pAccessor);
	if (!Result)
		return false;

	pTemplateAccessor = pTemplateType->m_UnnamedMethodArray [EFunction_Setter];
	if (!pTemplateAccessor)
		return true;

	size_t OverloadCount = pTemplateAccessor->GetOverloadCount ();
	for (size_t i = 0; i < OverloadCount; i++)
	{
		CFunction* pOverload = pTemplateAccessor->GetOverload (i);
		pAccessor = m_pModule->m_FunctionMgr.CreateFunction (EFunction_Setter, pOverload->GetType ());
		Result = AddMethodMember (pAccessor);
		if (!Result)
			return false;
	}

	return true;
}

CStructMember*
CPropertyType::CreateFieldMember (
	EStorage StorageKind,
	const rtl::CString& Name,
	CType* pType,
	size_t BitCount
	)
{
	if (!IsNamed ())
	{
		err::SetFormatStringError (_T("property template cannot have members other than accessors"));
		return NULL;
	}

	if (m_pParentClassType)
	{
		if (StorageKind != EStorage_Static)
		{
			if (!m_pFieldStructType)
			{
				CreateFieldStructType ();
				m_pParentClassFieldMember = m_pParentClassType->CreateFieldMember (StorageKind, m_pFieldStructType);
			}
		}
	}
	else 
	{
		if (StorageKind)
		{
			err::SetFormatStringError (_T("global property members cannot have storage specifier"));
			return NULL;
		}

		StorageKind = EStorage_Static;
	}

	return CreateFieldMemberImpl (StorageKind, Name, pType, BitCount);
}

bool
CPropertyType::AddMethodMember (CFunction* pFunction)
{
	EStorage StorageKind = pFunction->GetStorageKind ();
	EFunction FunctionKind = pFunction->GetFunctionKind ();

	if (!IsNamed () && FunctionKind != EFunction_Getter && FunctionKind != EFunction_Setter)
	{
		err::SetFormatStringError (_T("property template cannot have members other than accessors"));
		return NULL;
	}

	if (m_pParentClassType)
	{
		if (StorageKind != EStorage_Static)
		{
			pFunction->m_pClassType = m_pParentClassType;
			pFunction->m_pShortType = pFunction->m_pType;
			pFunction->m_pType = m_pParentClassType->GetMethodType (pFunction->m_pType);

			if (StorageKind == EStorage_Virtual)
				m_pParentClassType->m_VirtualMethodArray.Append (pFunction);
		}
	}
	else
	{
		if (StorageKind)
		{
			err::SetFormatStringError (_T("global property members cannot have storage specifier"));
			return NULL;
		}
	}

	pFunction->m_pParentNamespace = this;
	pFunction->m_pPropertyType = this;
	
	int ValidateFlags = 0;

	switch (FunctionKind)
	{
	case EFunction_Getter:
		ValidateFlags = EUnnamedMethodFlag_CanHaveArgs;
		break;

	case EFunction_Setter:
		ValidateFlags = EUnnamedMethodFlag_CanHaveArgs | EUnnamedMethodFlag_MustHaveArgs;
		break;

	case EFunction_Constructor:
	case EFunction_StaticConstructor:
	case EFunction_Destructor:
		ValidateFlags = 0;
		break;

	case EFunction_Named:
	case EFunction_AutoEv:
		pFunction->m_Tag.Format (_T("%s.%s"), m_Tag, pFunction->m_Name);
		return true;

	default:
		err::SetFormatStringError (_T("invalid %s in '%s'"), GetFunctionKindString (FunctionKind), GetTypeString ());
		return NULL;
	}

	if ((FunctionKind == EFunction_Getter || FunctionKind == EFunction_Setter) && 
		(m_UnnamedMethodArray [EFunction_Getter] || m_UnnamedMethodArray [EFunction_Setter]))
	{
		rtl::CString IndexArgSignature = CreateIndexArgSignature (pFunction);
		if (IndexArgSignature != GetIndexArgSignature ())
		{
			err::SetFormatStringError (_T("index arguments mismatch in property accessors"));
			return NULL;
		}
	}
	
	pFunction->m_Tag.Format (_T("%s.%s"), m_Tag, GetFunctionKindString (FunctionKind));
	return AddFunctionToUnnamedMethodTableImpl (pFunction, ValidateFlags);
}

bool
CPropertyType::AddPropertyMember (CPropertyType* pPropertyType)
{
	if (!IsNamed ())
	{
		err::SetFormatStringError (_T("property template cannot have members other than accessors"));
		return NULL;
	}

	EStorage StorageKind = pPropertyType->GetStorageKind ();

	if (m_pParentClassType)
	{
		m_pParentClassType->AddPropertyMember (pPropertyType);
	}
	else
	{
		if (StorageKind)
		{
			err::SetFormatStringError (_T("global property members cannot have storage specifier"));
			return NULL;
		}
	}

	return true;
}

rtl::CStringA
CPropertyType::GetIndexArgSignature ()
{
	if (!m_IndexArgSignature.IsEmpty ())
		return m_IndexArgSignature;

	CFunction* pAccessor = m_UnnamedMethodArray [EFunction_Getter];
	if (!pAccessor)
	{
		pAccessor = m_UnnamedMethodArray [EFunction_Setter];
		ASSERT (pAccessor);
	}

	m_IndexArgSignature = CreateIndexArgSignature (pAccessor);
	return m_IndexArgSignature;
}

rtl::CString 
CPropertyType::CreateIndexArgSignature (CFunction* pFunction)
{
	if (pFunction->GetFunctionKind () == EFunction_Getter)
		return pFunction->GetType ()->CreateArgSignature ();

	rtl::CArrayT <CType*> ArgTypeArray = pFunction->GetType ()->GetArgTypeArray ();
	size_t ArgCount = ArgTypeArray.GetCount ();
	return ArgCount  ?
		pFunction->GetType ()->CreateArgSignature (ArgTypeArray, ArgCount - 1, 0) :
		rtl::CString ();
}

rtl::CString
CPropertyType::CreateTypeString ()
{
	rtl::CString String;

	CFunction* pGetter = m_UnnamedMethodArray [EFunction_Getter];
	CFunction* pSetter = m_UnnamedMethodArray [EFunction_Setter];

	if (!pGetter)
	{
		if (IsNamed ())
			String.Format ("property %s", m_QualifiedName);
		else
			String = m_Tag;

		return String;
	}

	String.Format (
		pSetter ? _T("%s property") : _T("%s const property"), 
		pGetter->GetType ()->GetReturnType ()->GetTypeString ()
		);

	rtl::CArrayT <CType*> ArgTypeArray = pGetter->GetType ()->GetArgTypeArray ();
	size_t ArgCount = ArgTypeArray.GetCount ();
	for (size_t i = 0; i < ArgCount; i++)
	{
		CType* pArgType = ArgTypeArray [i];
		String.AppendFormat (_T(" [%s]"), pArgType->GetTypeString ());
	}

	if (IsNamed ())
	{
		String.Append (' ');
		String.Append (m_QualifiedName);
	}

	return String;
}

CPropertyPtrType* 
CPropertyType::GetPropertyPtrType (EType TypeKind)
{
	return m_pModule->m_TypeMgr.GetPropertyPtrType (TypeKind, this);
}

bool
CPropertyType::CalcLayout ()
{
	if (m_Flags & ETypeFlag_LayoutReady)
		return true;

	bool Result = PreCalcLayout ();
	if (!Result)
		return false;

	CFunction* pGetter = m_UnnamedMethodArray [EFunction_Getter];
	CFunction* pSetter = m_UnnamedMethodArray [EFunction_Setter];

	// vtable

	m_pVTableStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	m_pVTableStructType->m_Tag.Format (_T("%s.vtbl"), m_Tag);

	AddFunctionToVTableImpl (pGetter);

	if (pSetter)
	{
		size_t Count = pSetter->GetOverloadCount ();
		for (size_t i = 0; i < Count; i++)
		{
			CFunction* pSetterOverload = pSetter->GetOverload (i);
			AddFunctionToVTableImpl (pSetterOverload);
		}
	}

	Result = CalcFieldAndVTableLayoutImpl ();
	if (!Result)
		return false;

	PostCalcLayout ();
	return true;
}

namespace axl {
namespace jnc {

//.............................................................................

class CCompositeType: public CNamedType
{
protected:
	enum EUnnamedMethodFlag
	{
		EUnnamedMethodFlag_CanOverload  = 0x01,
		EUnnamedMethodFlag_CanHaveArgs  = 0x02,
		EUnnamedMethodFlag_MustHaveArgs = 0x04,
	};

protected:

	CFunction* m_UnnamedMethodArray [EFunction__UnnamedMethodCount];

public:
	CCompositeType ();

	CFunction*
	GetUnnamedMethod (EFunction MethodKind)
	{
		ASSERT (MethodKind < EFunction__UnnamedMethodCount);
		return m_UnnamedMethodArray [MethodKind];
	}

	CStructType* 
	GetVTableStructType ()
	{
		ASSERT (m_pVTableStructType);
		return m_pVTableStructType;
	}

protected:
	CStructType*
	CreateFieldStructType ();

	CStructType*
	CreateStaticFieldStructType ();

	CStructMember*
	CreateFieldMemberImpl (
		EStorage StorageKind,
		const rtl::CString& Name,
		CType* pType,
		size_t BitCount
		);

	bool
	AddFunctionToUnnamedMethodTableImpl (
		CFunction* pFunction,
		int ValidateFlags
		);

	void
	AddFunctionToVTableImpl (CFunction* pFunction);

	bool
	CalcFieldAndVTableLayoutImpl ();
};

//.............................................................................

CCompositeType::CCompositeType ()
{
	m_PackFactor = 8;
	m_pFieldStructType = NULL;
	m_pStaticFieldStructType = NULL;
	m_pStaticDataVariable = NULL;
	m_pVTableStructType = NULL;

	memset (m_UnnamedMethodArray, 0, sizeof (m_UnnamedMethodArray));
}



void
CCompositeType::AddFunctionToVTableImpl (CFunction* pFunction)
{
	size_t VTableIndex = m_VTable.GetCount ();

	if (m_TypeKind == EType_Property)
	{
		pFunction->m_PropertyVTableIndex = VTableIndex;
	}
	else
	{
		ASSERT (IsClassType ());
		pFunction->m_pVirtualOriginClassType = (CClassType*) this;
		pFunction->m_ClassVTableIndex = VTableIndex;
	}

	CDataPtrType* pPointerType = pFunction->GetType ()->GetDataPtrType (EType_DataPtr_u);
	m_pVTableStructType->CreateMember (pPointerType);
	m_VTable.Append (pFunction);
}

bool
CCompositeType::AddFunctionToUnnamedMethodTableImpl (
	CFunction* pFunction,
	int ValidateFlags
	)
{
	bool Result;

	EFunction FunctionKind = pFunction->GetFunctionKind ();
	ASSERT (FunctionKind < EFunction__UnnamedMethodCount);

	if (!m_UnnamedMethodArray [FunctionKind])
	{
		m_UnnamedMethodArray [FunctionKind] = pFunction;
	}
	else
	{
		if (!(ValidateFlags & EUnnamedMethodFlag_CanOverload))
		{
			err::SetFormatStringError (_T("'%s' already has %s"), GetTypeString (), GetFunctionKindString (FunctionKind));
			return false;
		}

		Result = m_UnnamedMethodArray [FunctionKind]->AddOverload (pFunction);
		if (!Result)
			return false;
	}

	if (!pFunction->GetType ()->GetArgTypeArray ().IsEmpty ())
	{
		if (!(ValidateFlags & EUnnamedMethodFlag_CanHaveArgs))
		{
			err::SetFormatStringError (_T("%s must have no arguments"), pFunction->m_Tag);
			return false;
		}
	}
	else 
	{
		if (ValidateFlags & EUnnamedMethodFlag_MustHaveArgs)
		{
			err::SetFormatStringError (_T("%s must have at least one argument"), pFunction->m_Tag);
			return false;
		}
	}

	return true;
}

bool
CCompositeType::CalcFieldAndVTableLayoutImpl ()
{
	bool Result;

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

	Result = m_pVTableStructType->CalcLayout ();
	if (!Result)
		return false;

	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {

*/