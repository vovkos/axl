#include "stdafx.h"
#include "axl_jnc_QualifiedName.h"

namespace axl {
namespace jnc {

//.............................................................................

rtl::CString
CQualifiedName::GetFullName () const
{
	if (m_List.IsEmpty ())
		return m_First;

	rtl::CString Name = m_First;	
	rtl::CBoxIteratorT <rtl::CString> It = m_List.GetHead ();
	for (; It; It++)
	{
		Name.Append ('.');
		Name.Append (*It);
	}

	return Name;
}

void
CQualifiedName::Copy (const CQualifiedName& Name)
{
	m_First = Name.m_First;
	m_List.Clear ();

	rtl::CBoxIteratorT <rtl::CString> It = Name.m_List.GetHead ();
	for (; It; It++)
		m_List.InsertTail (*It);
}

void
CQualifiedName::TakeOver (CQualifiedName* pName)
{
	m_First = pName->m_First;
	m_List.TakeOver (&pName->m_List);
	pName->Clear ();
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
