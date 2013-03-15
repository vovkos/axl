#include "stdafx.h"
#include "axl_jnc_PropertyVerifier.h"

namespace axl {
namespace jnc {

//.............................................................................

bool
CPropertyVerifier::AddMethod (
	EFunction FunctionKind,
	CFunctionType* pFunctionType
	)
{
	if (FunctionKind == EFunction_Getter || FunctionKind == EFunction_Setter)
	{
		if (FunctionKind == EFunction_Setter && pFunctionType->GetArgTypeArray ().IsEmpty ())
		{
			err::SetFormatStringError (_T("setter must have at least one argument"));
			return false;
		}

		rtl::CString IndexArgSignature = CreateIndexArgSignature (FunctionKind, pFunctionType);
		if (m_IndexArgSignature.IsEmpty ())
		{
			m_IndexArgSignature = IndexArgSignature;
		}
		else if (m_IndexArgSignature != IndexArgSignature)
		{
			err::SetFormatStringError (_T("index arguments mismatch in property accessors"));
			return false;
		}
	}
	else if (!pFunctionType->GetArgTypeArray ().IsEmpty ())
	{
		err::SetFormatStringError (_T("%s must have no arguments"), GetFunctionKindString (FunctionKind));
		return false;
	}

	int FunctionKindMask = 1 << FunctionKind;
	if (!(m_MethodMask & FunctionKindMask))
	{
		m_MethodMask |= FunctionKindMask;
	}
	else if (FunctionKind != EFunction_Setter)
	{
		err::SetFormatStringError (_T("property already has '%s' method"), GetFunctionKindString (FunctionKind));
		return false;
	}

	return true;
}

rtl::CStringA
CPropertyVerifier::CreateIndexArgSignature (
	EFunction FunctionKind,
	CFunctionType* pFunctionType
	)
{
	ASSERT (FunctionKind == EFunction_Getter || FunctionKind == EFunction_Setter);

	// refine!!!

	if (pFunctionType->IsMemberMethodType ())
		pFunctionType = pFunctionType->GetShortType ();

	if (FunctionKind == EFunction_Getter)
		return pFunctionType->CreateArgSignature ();

	rtl::CArrayT <CType*> ArgTypeArray = pFunctionType->GetArgTypeArray ();
	size_t ArgCount = ArgTypeArray.GetCount ();
	ASSERT (ArgCount);
	return pFunctionType->CreateArgSignature (ArgTypeArray, ArgCount - 1, 0);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
