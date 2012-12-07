#include "stdafx.h"
#include "axl_jnc_PropertyType.h"

namespace axl {
namespace jnc {

//.............................................................................

CPropertyType::CPropertyType ()
{
	m_ItemKind = EModuleItem_Property; // not just type 
	m_TypeKind = EType_Property;
	m_pGetter = NULL;
	m_pPointerType = NULL;
	m_pVTableStructType = NULL;	
	m_ParentVTableIndex = -1;
}

CClassType* 
CPropertyType::GetParentClassType ()
{
	return m_pParentNamespace->GetNamespaceKind () == ENamespace_Class ? 
		(CClassType*) m_pParentNamespace : 
		NULL;
}

rtl::CStringA
CPropertyType::CreateSignature ()
{
	rtl::CStringA String = "R{";
	
	String.Append (m_pGetter->GetType ()->GetSignature ());
	
	size_t Count = m_Setter.GetOverloadCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunction* pSetter = m_Setter.GetFunction (i);
		String.Append(pSetter->GetType ()->GetSignature ());
	}

	String.Append ("}");
	return String;
}

rtl::CString
CPropertyType::CreateTypeString ()
{
	ASSERT (m_pGetter);

	rtl::CString String;
	String.Format (
		m_Setter.IsEmpty () ? _T("%s const property") : _T("%s property"), 
		m_pGetter->GetType ()->GetReturnType ()->GetTypeString ()
		);

	return String;
}

void
CPropertyType::TagAccessors ()
{
	ASSERT (!m_Name.IsEmpty ());

	rtl::CString Tag = GetQualifiedName ();

	m_pGetter->m_Tag = Tag;
	m_pGetter->m_Tag += _T(".get");

	Tag += _T(".set");

	size_t Count = m_Setter.GetOverloadCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunction* pFunction = m_Setter.GetFunction (i);
		pFunction->m_Tag = Tag;
	}
}

bool
CPropertyType::CalcLayout ()
{
	CreateVTableStructType ();

	AddFunctionToVTable (m_pGetter);

	size_t Count = m_Setter.GetOverloadCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunction* pSetter = m_Setter.GetFunction (i);
		AddFunctionToVTable (pSetter);
	}

	return m_pVTableStructType->CalcLayout ();
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
