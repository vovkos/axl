#include "stdafx.h"
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
	int* pDataPtrTypeFlags
	)
{
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
					err::SetFormatStringError (_T("cannot create a class pointer to '%s'"), pType->GetTypeString ());
					return NULL;
				}

				pType = GetClassPtrType ((CClassType*) pType);
			}
			if (m_TypeModifiers & ETypeModifier_Function)
			{
				CFunctionType* pFunctionType = GetFunctionType (pType);
				if (!pFunctionType)
					return false;

				pType = GetFunctionPtrType (pFunctionType);
			}
			else if (m_TypeModifiers & ETypeModifier_Property)
			{
				CPropertyType* pPropertyType = GetPropertyType (pType);
				if (!pPropertyType)
					return false;

				pType = GetPropertyPtrType (pPropertyType);
			}
			else if (m_TypeModifiers & ETypeModifier_AutoEv)
			{
				CAutoEvType* pAutoEvType = GetAutoEvType (pType);
				if (!pAutoEvType)
					return false;

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

			default:
				pType = GetDataPtrType (pType);
			}

			break;

		case EDeclPrefix_Multicast:
		case EDeclPrefix_Event:
			pType = GetMulticastType (pType);
			if (!pType)
				return false;

			break;
		}

		if (!pType || !CheckUnusedModifiers ())
			return false;

		if (PrefixKind == EDeclPrefix_Event)
			m_TypeModifiers = ETypeModifier_Event;
	}

	m_TypeModifiers |= pDeclarator->GetTypeModifiers ();

	if (m_TypeModifiers & ETypeModifier_Property)
	{
		pType = GetPropertyType (pType);
		if (!pType)
			return false;
	}
	else if (m_TypeModifiers & ETypeModifier_Bindable)
	{
		pType = GetBindableDataType (pType);
		if (!pType)
			return false;
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
				return false;

			break;
		}

		if (!pType)
			return false;
	}

	if (pType->GetTypeKind () == EType_Class && (m_TypeModifiers & ETypeModifierMask_ClassPtr_p) != 0)
		pType = GetClassPtrType ((CClassType*) pType);

	if (pDataPtrTypeFlags)
	{
		int Flags = 0;

		if (m_TypeModifiers & ETypeModifier_Const)
			Flags |= EPtrTypeFlag_Const;

		if (m_TypeModifiers & ETypeModifier_ReadOnly)
			Flags |= EPtrTypeFlag_ReadOnly;

		if (m_TypeModifiers & ETypeModifier_Mutable)
			Flags |= EPtrTypeFlag_Mutable;

		if (m_TypeModifiers & ETypeModifier_Volatile)
			Flags |= EPtrTypeFlag_Volatile;

		if (m_TypeModifiers & ETypeModifier_Event)
		{
			if (pType->GetTypeKind () != EType_Multicast)
			{
				err::SetFormatStringError (_T("'event' modifier can only be applied to multicast"));
				return false;
			}

			Flags |= EPtrTypeFlag_Event;
		}

		m_TypeModifiers &= ~ETypeModifierMask_DataPtr;

		*pDataPtrTypeFlags = Flags;
	}

	if (!CheckUnusedModifiers ())
		return false;

	return pType;
}

bool
CDeclTypeCalc::CheckUnusedModifiers ()
{
	if (m_TypeModifiers)
	{
		err::SetFormatStringError (_T("unused modifier '%s'"), GetTypeModifierString (m_TypeModifiers));
		return false;
	}

	return true;
}

CType*
CDeclTypeCalc::GetIntegerType (CType* pType)
{
	ASSERT (m_TypeModifiers & ETypeModifierMask_Integer);

	if (!pType->IsIntegerType ())
	{
		err::SetFormatStringError (_T("'%s' modifier cannot be applied to '%s'"), GetTypeModifierString (m_TypeModifiers), pType->GetTypeString ());
		return false;
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
		err::SetFormatStringError (_T("cannot create array of '%s'"), pElementType->GetTypeString ());
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
		err::SetFormatStringError (_T("function cannot return '%s'"), pType->GetTypeString ());
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
		err::SetFormatStringError (_T("missing function suffix"));
		return NULL;
	}

	CDeclFunctionSuffix* pSuffix = (CDeclFunctionSuffix*) *m_Suffix--;

	ECallConv CallConv = GetCallConvFromModifiers (m_TypeModifiers);

	m_TypeModifiers &= ~ETypeModifierMask_Function;

	return m_pModule->m_TypeMgr.GetFunctionType (
		CallConv,
		pReturnType, 
		pSuffix->GetArgTypeArray (),
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
		err::SetFormatStringError (_T("property cannot return 'void'"));
		return NULL;
	}

	ECallConv CallConv = GetCallConvFromModifiers (m_TypeModifiers);

	int TypeFlags = 0;
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
			err::SetFormatStringError (_T("missing indexed property suffix"));
			return NULL;
		}

		CDeclFunctionSuffix* pSuffix = (CDeclFunctionSuffix*) *m_Suffix--;
		return m_pModule->m_TypeMgr.GetIndexedPropertyType (
			CallConv, 
			pReturnType, 
			pSuffix->GetArgTypeArray (),
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
		err::SetFormatStringError (_T("bindable data cannot be 'void'"));
		return NULL;
	}

	if (m_TypeModifiers & ETypeModifier_Indexed)
	{
		err::SetFormatStringError (_T("bindable data cannot be 'indexed'"));
		return NULL;
	}

	ECallConv CallConv = GetCallConvFromModifiers (m_TypeModifiers);
	int TypeFlags = 
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
		err::SetFormatStringError (_T("autoev function must be 'void'"));
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
		err::SetFormatStringError (_T("multicast can only return void, not '%s'"), pReturnType->GetTypeString ());
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
	int TypeFlags = 0;

	if (m_TypeModifiers & ETypeModifier_Const)
		TypeFlags |= EPtrTypeFlag_Const;

	if (m_TypeModifiers & ETypeModifier_Volatile)
		TypeFlags |= EPtrTypeFlag_Volatile;

	if (m_TypeModifiers & ETypeModifier_Nullable)
		TypeFlags |= EPtrTypeFlag_Nullable;

	m_TypeModifiers &= ~ETypeModifierMask_DataPtr;
	return m_pModule->m_TypeMgr.GetDataPtrType (pDataType, PtrTypeKind, TypeFlags);
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
	int TypeFlags = 0;

	if (m_TypeModifiers & ETypeModifier_Const_p)
		TypeFlags |= EPtrTypeFlag_Const;

	if (m_TypeModifiers & ETypeModifier_Nullable_p)
		TypeFlags |= EPtrTypeFlag_Nullable;

	m_TypeModifiers &= ~ETypeModifierMask_ClassPtr_p;
	return m_pModule->m_TypeMgr.GetClassPtrType (pClassType, PtrTypeKind, TypeFlags);
}

CFunctionPtrType*
CDeclTypeCalc::GetFunctionPtrType (CFunctionType* pFunctionType)
{
	EFunctionPtrType PtrTypeKind = GetFunctionPtrTypeKindFromModifiers (m_TypeModifiers);
	int TypeFlags = 0;

	if (m_TypeModifiers & ETypeModifier_Nullable)
		TypeFlags |= EPtrTypeFlag_Nullable;

	m_TypeModifiers &= ~ETypeModifierMask_FunctionPtr;
	return m_pModule->m_TypeMgr.GetFunctionPtrType (pFunctionType, PtrTypeKind, TypeFlags);
}

CPropertyPtrType*
CDeclTypeCalc::GetPropertyPtrType (CPropertyType* pPropertyType)
{
	EPropertyPtrType PtrTypeKind = GetPropertyPtrTypeKindFromModifiers (m_TypeModifiers);
	int TypeFlags = 0;

	if (m_TypeModifiers & ETypeModifier_Nullable)
		TypeFlags |= EPtrTypeFlag_Nullable;

	m_TypeModifiers &= ~ETypeModifierMask_PropertyPtr;
	return m_pModule->m_TypeMgr.GetPropertyPtrType (pPropertyType, PtrTypeKind, TypeFlags);
}

CAutoEvPtrType*
CDeclTypeCalc::GetAutoEvPtrType (CAutoEvType* pAutoEvType)
{
	EAutoEvPtrType PtrTypeKind = GetAutoEvPtrTypeKindFromModifiers (m_TypeModifiers);
	int TypeFlags = 0;

	if (m_TypeModifiers & ETypeModifier_Nullable)
		TypeFlags |= EPtrTypeFlag_Nullable;

	m_TypeModifiers &= ~ETypeModifierMask_AutoEvPtr;
	return m_pModule->m_TypeMgr.GetAutoEvPtrType (pAutoEvType, PtrTypeKind, TypeFlags);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
