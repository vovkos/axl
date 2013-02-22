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
			else switch (TypeKind)
			{
			case EType_Function:
				pType = GetFunctionPtrType ((CFunctionType*) pType);
				break;

			case EType_Property:
				pType = GetPropertyPtrType ((CPropertyType*) pType);
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
			pType = GetFunctionType (pType);
			if (!CheckUnusedModifiers ())
				return false;

			break;
		}

		if (!pType)
			return false;
	}

	if (pType->GetTypeKind () == EType_Class)
		pType = GetClassPtrType ((CClassType*) pType);

	if (pDataPtrTypeFlags)
	{
		int Flags = 0;

		if (m_TypeModifiers & ETypeModifier_Const)
			Flags |= EPtrTypeFlag_Const;

		if (m_TypeModifiers & ETypeModifier_ReadOnly)
			Flags |= EPtrTypeFlag_ReadOnly;

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

CFunctionType*
CDeclTypeCalc::GetFunctionType (CType* pReturnType)
{
	while (m_Suffix && m_Suffix->GetSuffixKind () == EDeclSuffix_Array)
	{
		pReturnType = GetArrayType (pReturnType);
		if (!pReturnType)
			return NULL;
	}

	EType TypeKind = pReturnType->GetTypeKind ();
	switch (TypeKind)
	{
	case EType_Function:
	case EType_Property:
	case EType_Multicast:
		err::SetFormatStringError (_T("function cannot return '%s'"), pReturnType->GetTypeString ());
		return NULL;

	case EType_Class:
		pReturnType= GetClassPtrType ((CClassType*) pReturnType);
		break;

	default:
		if (m_TypeModifiers & ETypeModifierMask_Integer)
		{
			pReturnType = GetIntegerType (pReturnType);
			if (!pReturnType)
				return NULL;
		}
	}

	if (!m_Suffix || m_Suffix->GetSuffixKind () != EDeclSuffix_Function)
	{
		err::SetFormatStringError (_T("missing function suffix"));
		return NULL;
	}

	CDeclFunctionSuffix* pSuffix = (CDeclFunctionSuffix*) *m_Suffix--;

	ECallConv CallConv = ECallConv_Default;

	if (m_TypeModifiers & ETypeModifier_Cdecl)
		CallConv = ECallConv_Cdecl;

	if (m_TypeModifiers & ETypeModifier_Stdcall)
		CallConv = ECallConv_Stdcall;

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
	while (m_Suffix && m_Suffix->GetSuffixKind () == EDeclSuffix_Array)
	{
		pReturnType = GetArrayType (pReturnType);
		if (!pReturnType)
			return NULL;
	}

	EType TypeKind = pReturnType->GetTypeKind ();
	switch (TypeKind)
	{
	case EType_Void:
	case EType_Function:
	case EType_Property:
	case EType_Multicast:
		err::SetFormatStringError (_T("property cannot return '%s'"), pReturnType->GetTypeString ());
		return NULL;

	case EType_Class:
		pReturnType= GetClassPtrType ((CClassType*) pReturnType);
		break;

	default:
		if (m_TypeModifiers & ETypeModifierMask_Integer)
		{
			pReturnType = GetIntegerType (pReturnType);
			if (!pReturnType)
				return NULL;
		}
	}

	CPropertyType* pPropertyType;
	int TypeFlags = 0;
	ECallConv CallConv = ECallConv_Default;

	if (m_TypeModifiers & ETypeModifier_Cdecl)
		CallConv = ECallConv_Cdecl;

	if (m_TypeModifiers & ETypeModifier_Stdcall)
		CallConv = ECallConv_Stdcall;

	if (m_TypeModifiers & ETypeModifier_Const)
		TypeFlags |= EPropertyTypeFlag_ReadOnly;

	if (m_TypeModifiers & ETypeModifier_Bindable)
		TypeFlags |= EPropertyTypeFlag_Bindable;

	if (!(m_TypeModifiers & ETypeModifier_Indexed))
	{
		pPropertyType = m_pModule->m_TypeMgr.GetSimplePropertyType (
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
		pPropertyType = m_pModule->m_TypeMgr.GetIndexedPropertyType (
			CallConv, 
			pReturnType, 
			pSuffix->GetArgTypeArray (),
			TypeFlags
			);
	}

	m_TypeModifiers &= ~ETypeModifierMask_Property;
	return pPropertyType;
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

	if (m_TypeModifiers & ETypeModifier_NoNull)
		TypeFlags |= EPtrTypeFlag_NoNull;

	m_TypeModifiers &= ~ETypeModifierMask_DataPtr;
	return m_pModule->m_TypeMgr.GetDataPtrType (pDataType, PtrTypeKind, TypeFlags);
}

CClassPtrType*
CDeclTypeCalc::GetClassPtrType (CClassType* pClassType)
{
	EClassPtrType PtrTypeKind = GetClassPtrTypeKindFromModifiers (m_TypeModifiers);
	int TypeFlags = 0;

	if (m_TypeModifiers & (ETypeModifier_Const | ETypeModifier_Const_p))
		TypeFlags |= EPtrTypeFlag_Const;

	if (m_TypeModifiers & (ETypeModifier_NoNull | ETypeModifier_NoNull_p))
		TypeFlags |= EPtrTypeFlag_NoNull;

	m_TypeModifiers &= ~ETypeModifierMask_ClassPtr;
	return m_pModule->m_TypeMgr.GetClassPtrType (pClassType, PtrTypeKind, TypeFlags);
}

CFunctionPtrType*
CDeclTypeCalc::GetFunctionPtrType (CFunctionType* pFunctionType)
{
	EFunctionPtrType PtrTypeKind = GetFunctionPtrTypeKindFromModifiers (m_TypeModifiers);
	int TypeFlags = 0;

	if (m_TypeModifiers & ETypeModifier_NoNull)
		TypeFlags |= EPtrTypeFlag_NoNull;

	m_TypeModifiers &= ~ETypeModifierMask_FunctionPtr;
	return m_pModule->m_TypeMgr.GetFunctionPtrType (pFunctionType, PtrTypeKind, TypeFlags);
}

CPropertyPtrType*
CDeclTypeCalc::GetPropertyPtrType (CPropertyType* pPropertyType)
{
	EPropertyPtrType PtrTypeKind = GetPropertyPtrTypeKindFromModifiers (m_TypeModifiers);
	int TypeFlags = 0;

	if (m_TypeModifiers & ETypeModifier_NoNull)
		TypeFlags |= EPtrTypeFlag_NoNull;

	m_TypeModifiers &= ~ETypeModifierMask_PropertyPtr;
	return m_pModule->m_TypeMgr.GetPropertyPtrType (pPropertyType, PtrTypeKind, TypeFlags);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
