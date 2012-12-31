#include "stdafx.h"
#include "axl_jnc_PropertyType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CPropertyType::CPropertyType ()
{
	m_ItemKind = EModuleItem_Property; // not just type 
	m_TypeKind = EType_Property;
	m_PropertyKind = EProperty_Undefined;
	m_pGetter = NULL;
	m_pPropertyPointerType = NULL;
	m_pVTableStructType = NULL;	
	m_pParentClassType = NULL;
	m_ParentVTableIndex = -1;
}

rtl::CStringA
CPropertyType::GetAccessorSignature ()
{
	if (!m_AccessorSignature.IsEmpty ())
		return m_AccessorSignature;

	m_AccessorSignature = "{";
	
	m_AccessorSignature.Append (m_pGetter->GetType ()->GetSignature ());
	
	size_t Count = m_Setter.GetOverloadCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunction* pSetter = m_Setter.GetFunction (i);
		m_AccessorSignature.Append(pSetter->GetType ()->GetSignature ());
	}

	m_AccessorSignature.Append ("}");
	return m_AccessorSignature;
}

rtl::CStringA
CPropertyType::GetShortAccessorSignature ()
{
	if (!m_ShortAccessorSignature.IsEmpty ())
		return m_ShortAccessorSignature;

	if (m_PropertyKind != EProperty_Member)
	{
		m_ShortAccessorSignature = GetAccessorSignature ();
		return m_ShortAccessorSignature;
	}

	m_ShortAccessorSignature = "{";

	m_ShortAccessorSignature.Append (m_pGetter->GetShortType ()->GetSignature ());
	
	size_t Count = m_Setter.GetOverloadCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunction* pSetter = m_Setter.GetFunction (i);
		m_ShortAccessorSignature.Append(pSetter->GetShortType ()->GetSignature ());
	}

	m_ShortAccessorSignature.Append ("}");
	return m_ShortAccessorSignature;
}

rtl::CString
CPropertyType::CreateTypeString ()
{
	ASSERT (m_pGetter);

	rtl::CString String;
	String.Format (
		m_Setter.IsEmpty () ? _T("%s const property %s") : _T("%s property %s"), 
		m_pGetter->GetType ()->GetReturnType ()->GetTypeString (),
		GetQualifiedName ()
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
	if (m_Flags & ETypeFlag_IsLayoutReady)
		return true;

	bool Result = PreCalcLayout ();
	if (!Result)
		return false;

	CreateVTableStructType ();

	AddFunctionToVTable (m_pGetter);

	size_t Count = m_Setter.GetOverloadCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunction* pSetter = m_Setter.GetFunction (i);
		AddFunctionToVTable (pSetter);
	}

	Result = m_pVTableStructType->CalcLayout ();
	if (!Result)
		return false;

	PostCalcLayout ();

	return true;
}

CPropertyPointerType* 
CPropertyType::GetPropertyPointerType ()
{
	return m_pModule->m_TypeMgr.GetPropertyPointerType (this);
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
