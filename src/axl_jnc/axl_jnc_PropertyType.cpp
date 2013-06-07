#include "pch.h"
#include "axl_jnc_PropertyType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const char* 
GetPropertyTypeFlagString (EPropertyTypeFlag Flag)
{
	static const char* StringTable [] = 
	{
		"autoget",   // EPropertyTypeFlag_AutoGet = 0x0100,
		"bindable",  // EPropertyTypeFlag_Bindable  = 0x0200,
	};

	size_t i = rtl::GetLoBitIdx32 (Flag >> 8);

	return i < countof (StringTable) ? 
		StringTable [i] : 
		"undefined-property-type-flag";
}

rtl::CString
GetPropertyTypeFlagString (uint_t Flags)
{
	if (!Flags)
		return rtl::CString ();

	EPropertyTypeFlag Flag = GetFirstPropertyTypeFlag (Flags);
	rtl::CString String = GetPropertyTypeFlagString (Flag);
	Flags &= ~Flag;

	while (Flags)
	{
		Flag = GetFirstPropertyTypeFlag (Flags);

		String += ' ';
		String += GetPropertyTypeFlagString (Flag);

		Flags &= ~Flag;
	}

	return String;
}

uint_t
GetPropertyTypeFlagsFromModifiers (uint_t Modifiers)
{
	uint_t Flags = 0;

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
	m_pStdObjectMemberPropertyType = NULL;
	m_pShortType = NULL;
	m_pBindablePropertyType = NULL;
	m_pVTableStructType = NULL;
	m_pAuFieldStructType = NULL;
	memset (m_AuFieldArray, 0, sizeof (m_AuFieldArray));
	m_pPropertyPtrTypeTuple = NULL;
}

CPropertyPtrType* 
CPropertyType::GetPropertyPtrType (
	EType TypeKind,
	EPropertyPtrType PtrTypeKind,
	uint_t Flags
	)
{
	return m_pModule->m_TypeMgr.GetPropertyPtrType (this, TypeKind, PtrTypeKind, Flags);
}

CPropertyType*
CPropertyType::GetMemberPropertyType (CClassType* pClassType)
{
	return m_pModule->m_TypeMgr.GetMemberPropertyType (pClassType, this);
}

CPropertyType*
CPropertyType::GetStdObjectMemberPropertyType ()
{
	return m_pModule->m_TypeMgr.GetStdObjectMemberPropertyType (this);
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

CStructField* 
CPropertyType::GetStdField (EStdField Field)
{
	ASSERT ((size_t) Field < EStdField__Count);
	
	CStructField* pField = m_AuFieldArray [Field];
	if (!pField)
	{
		err::SetFormatStringError (
			"'%s' has no field '%s'", 
			GetTypeString ().cc (), // thanks a lot gcc
			GetStdFieldString (Field)
			);
		return NULL;
	}

	return pField;
}

rtl::CString
CPropertyType::CreateSignature (
	CFunctionType* pGetterType,
	const CFunctionTypeOverload& SetterType,
	uint_t Flags
	)
{
	rtl::CString String = "Y";
	
	if (Flags & EPropertyTypeFlag_Bindable)
		String += 'b';

	if (Flags & EPropertyTypeFlag_AutoGet)
		String += 'g';

	if (Flags & EPropertyTypeFlag_AutoSet)
		String += 's';

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
		m_TypeModifierString += "const ";

	if (m_Flags & EPropertyTypeFlag_Bindable)
		m_TypeModifierString += "bindable ";

	if (m_Flags & EPropertyTypeFlag_AutoGet)
		m_TypeModifierString += "autoget ";

	if (m_Flags & EPropertyTypeFlag_AutoSet)
		m_TypeModifierString += "autoset ";

	if (IsIndexed ())
		m_TypeModifierString += "indexed ";

	return m_TypeModifierString;
}

void
CPropertyType::PrepareTypeString ()
{
	CType* pReturnType = GetReturnType ();

	m_TypeString = pReturnType->GetTypeString ();
	m_TypeString += ' ';
	m_TypeString += GetTypeModifierString ();
	m_TypeString += "property";
	
	if (IsIndexed ())
	{
		m_TypeString += ' ';
		m_TypeString += m_pGetterType->GetArgString ();
	}
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
