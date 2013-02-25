#include "stdafx.h"
#include "axl_jnc_PropertyType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t* 
GetPropertyTypeFlagString (EPropertyTypeFlag Flag)
{
	static const tchar_t* StringTable [] = 
	{
		_T("autoget"),   // EPropertyTypeFlag_AutoGet = 0x0100,
		_T("bindable"),  // EPropertyTypeFlag_Bindable  = 0x0200,
	};

	size_t i = rtl::GetLoBitIdx32 (Flag >> 8);

	return i < countof (StringTable) ? 
		StringTable [i] : 
		_T("undefined-property-type-flag");
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

int
GetPropertyTypeFlagsFromModifiers (int Modifiers)
{
	int Flags;

	if (Modifiers & ETypeModifier_AutoGet)
		Flags |= EPropertyTypeFlag_AutoGet;

	if (Modifiers & ETypeModifier_Bindable)
		Flags |= EPropertyTypeFlag_Bindable;

	return Flags;
}
	
//.............................................................................

CPropertyType::CPropertyType ()
{
	m_TypeKind = EType_Property;

	m_pGetterType = NULL;
	m_pAbstractPropertyMemberType = NULL;
	m_pShortType = NULL;
	m_pBindablePropertyType = NULL;
	m_pVTableStructType = NULL;
	m_pAuFieldStructType = NULL;
	m_pAuPropValue = NULL;
	m_pAuOnChangeEvent = NULL;
	m_pPropertyPtrTypeTuple = NULL;
}

CPropertyPtrType* 
CPropertyType::GetPropertyPtrType (
	EType TypeKind,
	EPropertyPtrType PtrTypeKind,
	int Flags
	)
{
	return m_pModule->m_TypeMgr.GetPropertyPtrType (this, TypeKind, PtrTypeKind, Flags);
}

CPropertyType*
CPropertyType::GetAbstractPropertyMemberType ()
{
	return m_pModule->m_TypeMgr.GetAbstractPropertyMemberType (this);
}

CPropertyType*
CPropertyType::GetShortType  ()
{
	return m_pModule->m_TypeMgr.GetShortPropertyType (this);
}

CPropertyType*
CPropertyType::GetBindablePropertyType ()
{
	return m_pModule->m_TypeMgr.GetBindablePropertyType (this);
}

CStructType*
CPropertyType::GetVTableStructType ()
{
	return m_pModule->m_TypeMgr.GetPropertyVTableStructType (this);
}

rtl::CStringA
CPropertyType::CreateSignature (
	CFunctionType* pGetterType,
	const CFunctionTypeOverload& SetterType,
	int Flags
	)
{
	rtl::CStringA String = "Y";
	
	if (Flags & EPropertyTypeFlag_Bindable)
		String += 'b';

	if (Flags & EPropertyTypeFlag_AutoGet)
		String += 'a';

	String += pGetterType->GetSignature ();

	size_t OverloadCount = SetterType.GetOverloadCount ();
	for (size_t i = 0; i < OverloadCount; i++)
	{
		CFunctionType* pOverloadType = SetterType.GetOverload (i);
		String += pOverloadType->GetSignature ();
	}

	return String;
}

rtl::CString
CPropertyType::GetTypeModifierString ()
{
	if (!m_TypeModifierString.IsEmpty ())
		return m_TypeModifierString;

	if (IsReadOnly ())
		m_TypeModifierString += _T("const ");

	if (m_Flags & EPropertyTypeFlag_Bindable)
		m_TypeModifierString += _T("bindable ");

	if (m_Flags & EPropertyTypeFlag_AutoGet)
		m_TypeModifierString += _T("autoget ");

	if (IsIndexed ())
		m_TypeModifierString += _T("indexed ");

	return m_TypeModifierString;
}

void
CPropertyType::PrepareTypeString ()
{
	CType* pReturnType = GetReturnType ();

	m_TypeString = pReturnType->GetTypeString ();
	m_TypeString += _T(' ');
	m_TypeString += GetTypeModifierString ();
	m_TypeString += _T("property");
	
	if (IsIndexed ())
	{
		m_TypeString += _T(' ');
		m_TypeString += m_pGetterType->GetArgTypeString ();
	}
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
