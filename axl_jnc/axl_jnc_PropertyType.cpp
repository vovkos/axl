#include "stdafx.h"
#include "axl_jnc_PropertyType.h"

namespace axl {
namespace jnc {

//.............................................................................

CPropertyType::CPropertyType ()
{
	m_TypeKind = EType_Property;
	m_pGetterType = NULL;
	m_SetterType = NULL;
}

rtl::CStringA
CPropertyType::CreateSignature (
	CFunctionType* pGetterType,
	const CFunctionTypeOverload& SetterType
	)
{
	rtl::CStringA String = "R{";
	
	String.Append (pGetterType->GetSignature ());
	
	size_t Count = SetterType.GetOverloadCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunctionType* pType = SetterType.GetType (i);
		String.Append(pType->GetSignature ());
	}

	String.Append ("}");
	return String;
}

rtl::CString
CPropertyType::CreateTypeString (
	CFunctionType* pGetterType,
	const CFunctionTypeOverload& SetterType
	)
{
	rtl::CString String;
	String.Format (
		SetterType.IsEmpty () ? _T("%s const property") : _T("%s property"), 
		pGetterType->GetReturnType ()->GetTypeString ()
		);

	return String;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
