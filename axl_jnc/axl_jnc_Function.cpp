#include "stdafx.h"
#include "axl_jnc_Function.h"

namespace axl {
namespace jnc {

//.............................................................................

rtl::CString
CFunction::CreateArgString ()
{
	if (m_ArgList.IsEmpty ())
		return m_pType->IsVarArg () ? _T("(...)") : _T("()");

	rtl::CIteratorT <jnc::CFunctionFormalArg> Arg = m_ArgList.GetHead ();

	rtl::CString String;
	String.Format (
		_T("(%s %s"), 
		Arg->GetType ()->GetTypeString (),
		Arg->GetName ()
		);

	for (Arg++; Arg; Arg++)
		String.AppendFormat (
			_T(", %s %s"), 
			Arg->GetType ()->GetTypeString (),
			Arg->GetName ()
			);

	if (!m_pType->IsVarArg ())
		String.Append (_T(")"));
	else 
		String.Append (_T(", ...)"));

	return String;
}

//.............................................................................

CFunction*
CFunctionOverload::FindOverload (
	CType** ppArgType,
	size_t ArgCount
	) const
{
	// TODO: implement 

	return m_pFunction; 
}

bool
CFunctionOverload::AddOverload (CFunction* pFunction)
{
	if (!m_pFunction)
	{
		m_pFunction = pFunction;
		return true;
	}

	m_OverloadArray.Append (pFunction);

	// TODO: check that no overloads overlap

	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
