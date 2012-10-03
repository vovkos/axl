#include "stdafx.h"
#include "axl_jnc_FunctionType.h"

namespace axl {
namespace jnc {

//.............................................................................

CFunctionType::CFunctionType ()
{
	m_TypeKind = EType_Function;
	m_pReturnType = NULL;
	m_Flags = 0;
}

rtl::CStringA
CFunctionType::CreateSignature (
	CType* pReturnType,
	CType** ppArgType,
	size_t ArgCount,
	int Flags
	)
{
	rtl::CStringA String = "F(";
	
	String.Append (pReturnType->GetSignature ());
	
	for (size_t i = 0; i < ArgCount; i++)
	{
		CType* pType = ppArgType [i];
		String.Append(pType->GetSignature ());
	}

	if (!(Flags & EFunctionTypeFlag_IsVarArg))
		String.Append (")");
	else
		String.Append ("...)");

	return String;
}

rtl::CString
CFunctionType::CreateTypeString (
	CType* pReturnType,
	CType** ppArgType,
	size_t ArgCount,
	int Flags
	)
{
	rtl::CString String = pReturnType->GetTypeString ();

	String.Append (_T(" ("));
	
	if (ArgCount)
	{
		CType* pType = ppArgType [0];
		String.Append(pType->GetTypeString ());
	}

	for (size_t i = 1; i < ArgCount; i++)
	{
		CType* pType = ppArgType [i];
		String.Append(_T(", "));
		String.Append(pType->GetTypeString ());
	}

	if (!(Flags & EFunctionTypeFlag_IsVarArg))
		String.Append (_T(")"));
	else if (ArgCount)
		String.Append (_T(", ...)"));
	else
		String.Append (_T("...)"));

	return String;
}

//.............................................................................

bool
CFunctionTypeOverload::AddOverload (CFunctionType* pType)
{
	if (!m_pType)
	{
		m_pType = pType;
		return true;
	}

	// TODO: check no duplicate overloads

	m_OverloadArray.Append (pType);
	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
