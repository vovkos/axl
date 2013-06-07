#include "pch.h"
#include "axl_jnc_DeclTypeCalc.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CDeclTypeCalc::CDeclTypeCalc ()
{
	m_pModule = NULL;
	m_TypeModifiers = 0;
}

CType*
CDeclTypeCalc::CalcType (
	CDeclarator* pDeclarator,
	uint_t* pDataPtrTypeFlags
	)
{
	bool Result;

	CType* pType = pDeclarator->GetBaseType (); 
	rtl::CArrayT <TDeclPrefix> PrefixArray = pDeclarator->GetPrefixArray ();
	m_Suffix = pDeclarator->GetSuffixList ().GetTail ();
	m_pModule = pType->GetModule ();

	size_t PrefixCount = PrefixArray.GetCount ();
	for (size_t i = 0; i < PrefixCount; i++)
	{
		m_TypeModifiers = PrefixArray [i].m_TypeModifiers;

		EDeclPrefix PrefixKind = PrefixArray [i].m_PrefixKind;
		EType TypeKind = pType->GetTypeKind ();

		switch (PrefixKind)
		{
		case EDeclPrefix_Pointer:
			if (m_TypeModifiers & ETypeModifier_Class)
			{
				if (pType->GetTypeKind () != EType_Class)
				{
					err::SetFormatStringError (
						"cannot create a class pointer to '%s'", 
						pType->GetTypeString ().cc () // thanks a lot gcc
						);
					return NULL;
				}

				Result = PromoteClassPtrTypeModifiers (&m_TypeModifiers);
				if (!Result)
					return NULL;

				pType = GetClassPtrType ((CClassType*) pType);
			}
			else if (m_TypeModifiers & ETypeModifier_Function)
			{
				CFunctionType* pFunctionType = GetFunctionType (pType);
				if (!pFunctionType)
					return NULL;

				pType = GetFunctionPtrType (pFunctionType);
			}
			else if (m_TypeModifiers & ETypeModifier_Property)
			{
				CPropertyType* pPropertyType = GetPropertyType (pType);
				if (!pPropertyType)
					return NULL;

				pType = GetPropertyPtrType (pPropertyType);
			}
			else if (m_TypeModifiers & ETypeModifier_AutoEv)
			{
				CAutoEvType* pAutoEvType = GetAutoEvType (pType);
				if (!pAutoEvType)
					return NULL;

				pType = GetAutoEvPtrType (pAutoEvType);
			}
			else switch (TypeKind)
			{
			case EType_Function:
				pType = GetFunctionPtrType ((CFunctionType*) pType);
				break;

			case EType_Property:
				pType = GetPropertyPtrType ((CPropertyType*) pType);
				break;

			case EType_AutoEv:
				pType = GetAutoEvPtrType ((CAutoEvType*) pType);
				break;

			case EType_Import:
				pType = GetImportPtrType ((CImportType*) pType);
				break;

			default:
				pType = GetDataPtrType (pType);
			}

			break;

		case EDeclPrefix_Multicast:
		case EDeclPrefix_Event:
			pType = GetMulticastType (pType);
			if (!pType)
				return NULL;

			break;
		}

		if (!pType || !CheckUnusedModifiers ())
			return NULL;

		if (PrefixKind == EDeclPrefix_Event)
			m_TypeModifiers = ETypeModifier_Event;
	}

	m_TypeModifiers |= pDeclarator->GetTypeModifiers ();

	if (m_TypeModifiers & ETypeModifier_Property)
	{
		pType = GetPropertyType (pType);
		if (!pType)
			return NULL;
	}
	else if (m_TypeModifiers & ETypeModifier_Bindable)
	{
		pType = GetBindableDataType (pType);
		if (!pType)
			return NULL;
	}

	while (m_Suffix)
	{
		CDeclSuffix* pSuffix = (CDeclSuffix*) *m_Suffix;
		EDeclSuffix SuffixKind = pSuffix->GetSuffixKind ();

		switch (SuffixKind)
		{
		case EDeclSuffix_Array:
			pType = GetArrayType (pType);
			break;

		case EDeclSuffix_Function:
			if (m_TypeModifiers & ETypeModifier_AutoEv)
				pType = GetAutoEvType (pType);
			else
				pType = GetFunctionType (pType);

			if (!CheckUnusedModifiers ())
				return NULL;

			break;
		}

		if (!pType)
			return NULL;
	}

	if ((m_TypeModifiers & ETypeModifierMask_ClassPtr_p) != 0)
	{
		if (pType->GetTypeKind () == EType_Class)
		{
			pType = GetClassPtrType ((CClassType*) pType);
		}
		else if (pType->GetTypeKind () == EType_Import && 
			((CImportType*) pType)->GetImportTypeKind () == EImportType_Primary)
		{
			pType = m_pModule->m_TypeMgr.GetSecondaryImportType (
				EImportType_Data, 
				(CPrimaryImportType*) pType,
				m_TypeModifiers & ETypeModifierMask_ClassPtr_p
				);

			m_TypeModifiers &= ~ETypeModifierMask_ClassPtr_p;
		}
	}

	if (pDataPtrTypeFlags)
		*pDataPtrTypeFlags = GetDataPtrTypeFlags ();

	if (!CheckUnusedModifiers ())
		return NULL;

	return pType;
}

CType*
CDeclTypeCalc::CalcDataType (
	CType* pType,
	uint_t TypeModifiers,
	uint_t* pDataPtrTypeFlags
	)
{
	m_TypeModifiers = TypeModifiers;

	if (pType->GetTypeKind () == EType_Class)
		pType = GetClassPtrType ((CClassType*) pType);

	if (pDataPtrTypeFlags)
		*pDataPtrTypeFlags = GetDataPtrTypeFlags ();

	if (!CheckUnusedModifiers ())
		return NULL;

	return pType;
}

CType*
CDeclTypeCalc::CalcPtrType (
	CType* pType,
	uint_t TypeModifiers
	)
{
	m_TypeModifiers = TypeModifiers;

	EType TypeKind = pType->GetTypeKind ();
	switch (TypeKind)
	{
	case EType_Function:
		pType = GetFunctionPtrType ((CFunctionType*) pType);
		break;

	case EType_Property:
		pType = GetPropertyPtrType ((CPropertyType*) pType);
		break;

	case EType_AutoEv:
		pType = GetAutoEvPtrType ((CAutoEvType*) pType);
		break;

	default:
		pType = GetDataPtrType (pType);
	}

	if (!CheckUnusedModifiers ())
		return NULL;

	return pType;
}

bool
CDeclTypeCalc::CheckUnusedModifiers ()
{
	if (m_TypeModifiers)
	{
		err::SetFormatStringError ("unused modifier '%s'", GetTypeModifierString (m_TypeModifiers));
		return false;
	}

	return true;
}

uint_t 
CDeclTypeCalc::GetDataPtrTypeFlags ()
{
	uint_t Flags = 0;

	if (m_TypeModifiers & ETypeModifier_Const)
		Flags |= EPtrTypeFlag_Const;

	if (m_TypeModifiers & ETypeModifier_ReadOnly)
		Flags |= EPtrTypeFlag_ReadOnly;

	if (m_TypeModifiers & ETypeModifier_Mutable)
		Flags |= EPtrTypeFlag_Mutable;

	if (m_TypeModifiers & ETypeModifier_Volatile)
		Flags |= EPtrTypeFlag_Volatile;

	if (m_TypeModifiers & ETypeModifier_Event)
		Flags |= EPtrTypeFlag_Event;

	m_TypeModifiers &= ~ETypeModifierMask_DataPtr;

	return Flags;
}

CType*
CDeclTypeCalc::GetIntegerType (CType* pType)
{
	ASSERT (m_TypeModifiers & ETypeModifierMask_Integer);

	if (!pType->IsIntegerType ())
	{
		err::SetFormatStringError ("'%s' modifier cannot be applied to '%s'", 
			GetTypeModifierString (m_TypeModifiers & ETypeModifierMask_Integer), 
			pType->GetTypeString ().cc () 
			);
		return NULL;
	}

	if (m_TypeModifiers & ETypeModifier_Unsigned)
	{
		EType ModTypeKind = GetUnsignedIntegerTypeKind (pType->GetTypeKind ());
		pType = m_pModule->m_TypeMgr.GetPrimitiveType (ModTypeKind);
	}

	if (m_TypeModifiers & ETypeModifier_BigEndian)
	{
		EType ModTypeKind = GetBigEndianIntegerTypeKind (pType->GetTypeKind ());
		pType = m_pModule->m_TypeMgr.GetPrimitiveType (ModTypeKind);
	}

	m_TypeModifiers &= ~ETypeModifierMask_Integer;
	return pType;
}

CArrayType*
CDeclTypeCalc::GetArrayType (CType* pElementType)
{
	ASSERT (m_Suffix && m_Suffix->GetSuffixKind () == EDeclSuffix_Array);

	CDeclArraySuffix* pSuffix = (CDeclArraySuffix*) *m_Suffix--;

	EType TypeKind = pElementType->GetTypeKind ();
	switch (TypeKind)
	{
	case EType_Void:
	case EType_Function:
	case EType_Property:
		err::SetFormatStringError (
			"cannot create array of '%s'", 
			pElementType->GetTypeString ().cc () 
			);
		return NULL;

	case EType_Class:
		pElementType = GetClassPtrType ((CClassType*) pElementType);
		break;

	default:
		if (m_TypeModifiers & ETypeModifierMask_Integer)
		{
			pElementType = GetIntegerType (pElementType);
			if (!pElementType)
				return NULL;
		}
	}

	return m_pModule->m_TypeMgr.GetArrayType (pElementType, pSuffix->GetElementCount ());
}

CType*
CDeclTypeCalc::PrepareReturnType (CType* pType)
{
	while (m_Suffix && m_Suffix->GetSuffixKind () == EDeclSuffix_Array)
	{
		pType = GetArrayType (pType);
		if (!pType)
			return NULL;
	}

	EType TypeKind = pType->GetTypeKind ();
	switch (TypeKind)
	{
	case EType_Function:
	case EType_Property:
	case EType_Multicast:
		err::SetFormatStringError (
			"function cannot return '%s'", 
			pType->GetTypeString ().cc () 
			);
		return NULL;

	case EType_Class:
		pType = GetClassPtrType ((CClassType*) pType);
		break;

	default:
		if (m_TypeModifiers & ETypeModifierMask_Integer)
			pType = GetIntegerType (pType);
	}

	return pType;
}

CFunctionType*
CDeclTypeCalc::GetFunctionType (CType* pReturnType)
{
	pReturnType = PrepareReturnType (pReturnType);
	if (!pReturnType)
		return NULL;

	if (!m_Suffix || m_Suffix->GetSuffixKind () != EDeclSuffix_Function)
	{
		err::SetFormatStringError ("missing function suffix");
		return NULL;
	}

	CDeclFunctionSuffix* pSuffix = (CDeclFunctionSuffix*) *m_Suffix--;

	ECallConv CallConv = GetCallConvFromModifiers (m_TypeModifiers);

	m_TypeModifiers &= ~ETypeModifierMask_Function;

	return m_pModule->m_TypeMgr.CreateUserFunctionType (
		CallConv,
		pReturnType, 
		pSuffix->GetArgArray (),
		pSuffix->GetFunctionTypeFlags ()
		);
}

CPropertyType*
CDeclTypeCalc::GetPropertyType (CType* pReturnType)
{
	pReturnType = PrepareReturnType (pReturnType);
	if (!pReturnType)
		return NULL;

	if (pReturnType->GetTypeKind () == EType_Void)
	{
		err::SetFormatStringError ("property cannot return 'void'");
		return NULL;
	}

	ECallConv CallConv = GetCallConvFromModifiers (m_TypeModifiers);

	uint_t TypeFlags = 0;
	if (m_TypeModifiers & ETypeModifier_Const)
		TypeFlags |= EPropertyTypeFlag_Const;

	if (m_TypeModifiers & ETypeModifier_AutoGet)
		TypeFlags |= EPropertyTypeFlag_AutoGet;

	if (m_TypeModifiers & ETypeModifier_Bindable)
		TypeFlags |= EPropertyTypeFlag_Bindable;

	m_TypeModifiers &= ~ETypeModifierMask_Property;

	if (!(m_TypeModifiers & ETypeModifier_Indexed))
	{
		return m_pModule->m_TypeMgr.GetSimplePropertyType (
			CallConv, 
			pReturnType, 
			TypeFlags
			);
	}
	else
	{
		if (!m_Suffix || m_Suffix->GetSuffixKind () != EDeclSuffix_Function)
		{
			err::SetFormatStringError ("missing indexed property suffix");
			return NULL;
		}

		CDeclFunctionSuffix* pSuffix = (CDeclFunctionSuffix*) *m_Suffix--;
		return m_pModule->m_TypeMgr.CreateIndexedPropertyType (
			CallConv, 
			pReturnType, 
			pSuffix->GetArgArray (),
			TypeFlags
			);
	}
}

CPropertyType*
CDeclTypeCalc::GetBindableDataType (CType* pDataType)
{
	pDataType = PrepareReturnType (pDataType);
	if (!pDataType)
		return NULL;

	if (pDataType->GetTypeKind () == EType_Void)
	{
		err::SetFormatStringError ("bindable data cannot be 'void'");
		return NULL;
	}

	if (m_TypeModifiers & ETypeModifier_Indexed)
	{
		err::SetFormatStringError ("bindable data cannot be 'indexed'");
		return NULL;
	}

	ECallConv CallConv = GetCallConvFromModifiers (m_TypeModifiers);
	uint_t TypeFlags = 
		EPropertyTypeFlag_Bindable | 
		EPropertyTypeFlag_AutoGet | 
		EPropertyTypeFlag_AutoSet;

	m_TypeModifiers &= ~ETypeModifierMask_Property;
	return m_pModule->m_TypeMgr.GetSimplePropertyType (CallConv, pDataType, TypeFlags);
}

CAutoEvType*
CDeclTypeCalc::GetAutoEvType (CType* pReturnType)
{
	if (pReturnType->GetTypeKind () != EType_Void)
	{
		err::SetFormatStringError ("autoev function must be 'void'");
		return NULL;
	}

	CFunctionType* pStarterType = GetFunctionType (pReturnType);
	if (!pStarterType)
		return NULL;

	m_TypeModifiers &= ~ETypeModifier_AutoEv;

	return m_pModule->m_TypeMgr.GetAutoEvType (pStarterType);
}

CMulticastType*
CDeclTypeCalc::GetMulticastType (CType* pReturnType)
{
	if (pReturnType->GetTypeKind () != EType_Void)
	{
		err::SetFormatStringError (
			"multicast can only return void, not '%s'", 
			pReturnType->GetTypeString ().cc () 
			);
		return NULL;
	}
	
	CFunctionType* pFunctionType = GetFunctionType (pReturnType);
	if (!pFunctionType)
		return NULL;

	CFunctionPtrType* pFunctionPtrType = GetFunctionPtrType (pFunctionType);
	if (!pFunctionPtrType)
		return NULL;

	m_TypeModifiers &= ~ETypeModifierMask_Multicast;
	return m_pModule->m_TypeMgr.GetMulticastType (pFunctionPtrType);
}

CDataPtrType* 
CDeclTypeCalc::GetDataPtrType (CType* pDataType)
{
	EType TypeKind = pDataType->GetTypeKind ();
	switch (TypeKind)
	{
	case EType_Function:
	case EType_Property:
		ASSERT (false);
		break;

	case EType_Class:
		pDataType = GetClassPtrType ((CClassType*) pDataType);
		break;

	default:
		if (m_TypeModifiers & ETypeModifierMask_Integer)
		{
			pDataType = GetIntegerType (pDataType);
			if (!pDataType)
				return NULL;
		}
	}

	EDataPtrType PtrTypeKind = GetDataPtrTypeKindFromModifiers (m_TypeModifiers);
	uint_t TypeFlags = 0;

	if (m_TypeModifiers & ETypeModifier_Const)
		TypeFlags |= EPtrTypeFlag_Const;

	if (m_TypeModifiers & ETypeModifier_Volatile)
		TypeFlags |= EPtrTypeFlag_Volatile;

	if (m_TypeModifiers & ETypeModifier_Nullable)
		TypeFlags |= EPtrTypeFlag_Nullable;

	m_TypeModifiers &= ~ETypeModifierMask_DataPtr;
	return pDataType->GetDataPtrType (PtrTypeKind, TypeFlags);
}

CClassPtrType*
CDeclTypeCalc::GetClassPtrType (CClassType* pClassType)
{
	if (m_TypeModifiers & ETypeModifier_Class)
	{
		m_TypeModifiers |= PromoteClassPtrTypeModifiers (m_TypeModifiers);
		m_TypeModifiers &= ~(ETypeModifier_Class | ETypeModifierMask_ClassPtr);
	}

	EClassPtrType PtrTypeKind = GetClassPtrTypeKindFromModifiers (m_TypeModifiers);
	uint_t TypeFlags = 0;

	if (m_TypeModifiers & ETypeModifier_Const_p)
		TypeFlags |= EPtrTypeFlag_Const;

	if (m_TypeModifiers & ETypeModifier_Nullable_p)
		TypeFlags |= EPtrTypeFlag_Nullable;

	m_TypeModifiers &= ~ETypeModifierMask_ClassPtr_p;
	return pClassType->GetClassPtrType (PtrTypeKind, TypeFlags);
}

CFunctionPtrType*
CDeclTypeCalc::GetFunctionPtrType (CFunctionType* pFunctionType)
{
	EFunctionPtrType PtrTypeKind = GetFunctionPtrTypeKindFromModifiers (m_TypeModifiers);
	uint_t TypeFlags = 0;

	if (m_TypeModifiers & ETypeModifier_Nullable)
		TypeFlags |= EPtrTypeFlag_Nullable;

	m_TypeModifiers &= ~ETypeModifierMask_FunctionPtr;
	return pFunctionType->GetFunctionPtrType (PtrTypeKind, TypeFlags);
}

CPropertyPtrType*
CDeclTypeCalc::GetPropertyPtrType (CPropertyType* pPropertyType)
{
	EPropertyPtrType PtrTypeKind = GetPropertyPtrTypeKindFromModifiers (m_TypeModifiers);
	uint_t TypeFlags = 0;

	if (m_TypeModifiers & ETypeModifier_Nullable)
		TypeFlags |= EPtrTypeFlag_Nullable;

	m_TypeModifiers &= ~ETypeModifierMask_PropertyPtr;
	return pPropertyType->GetPropertyPtrType (PtrTypeKind, TypeFlags);
}

CAutoEvPtrType*
CDeclTypeCalc::GetAutoEvPtrType (CAutoEvType* pAutoEvType)
{
	EAutoEvPtrType PtrTypeKind = GetAutoEvPtrTypeKindFromModifiers (m_TypeModifiers);
	uint_t TypeFlags = 0;

	if (m_TypeModifiers & ETypeModifier_Nullable)
		TypeFlags |= EPtrTypeFlag_Nullable;

	m_TypeModifiers &= ~ETypeModifierMask_AutoEvPtr;
	return pAutoEvType->GetAutoEvPtrType (PtrTypeKind, TypeFlags);
}

CType*
CDeclTypeCalc::GetImportPtrType (CImportType* pImportType)
{
	EImportType ImportTypeKind = pImportType->GetImportTypeKind ();
	if (ImportTypeKind != EImportType_Primary)
		return GetDataPtrType (pImportType);

	uint_t TypeModifiers = m_TypeModifiers & ETypeModifierMask_ImportPtr;
	m_TypeModifiers &= ~ETypeModifierMask_ImportPtr;
	
	return m_pModule->m_TypeMgr.GetSecondaryImportType (
		EImportType_Pointer, 
		(CPrimaryImportType*) pImportType, 
		TypeModifiers
		);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
