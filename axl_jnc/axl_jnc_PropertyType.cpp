#include "stdafx.h"
#include "axl_jnc_PropertyType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {
	
//.............................................................................

CPropertyType::CPropertyType ()
{
	m_TypeKind = EType_Property;

	m_pGetterType = NULL;
	m_pAbstractPropertyMemberType = NULL;
	m_pShortType = NULL;
	m_pBindablePropertyType = NULL;
	m_pVTableStructType = NULL;
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
		m_TypeModifierString += _T(" const");

	if (m_Flags & EPropertyTypeFlag_Bindable)
		m_TypeModifierString += _T(" bindable");

	if (m_Flags & EPropertyTypeFlag_AutoGet)
		m_TypeModifierString += _T(" autoget");

	if (IsIndexed ())
		m_TypeModifierString += _T(" indexed");

	return m_TypeModifierString;
}

void
CPropertyType::PrepareTypeString ()
{
	CType* pReturnType = GetReturnType ();

	m_TypeString = pReturnType->GetTypeString ();
	m_TypeString += GetTypeModifierString ();
	m_TypeString += _T(" property");
	
	if (IsIndexed ())
	{
		m_TypeString += _T(' ');
		m_TypeString += m_pGetterType->GetArgTypeString ();
	}
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
