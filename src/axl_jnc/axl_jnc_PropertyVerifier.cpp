#include "pch.h"
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
		if (FunctionKind == EFunction_Setter && pFunctionType->GetArgArray ().IsEmpty ())
		{
			err::SetFormatStringError ("setter must have at least one argument");
			return false;
		}

		rtl::CString IndexArgSignature = CreateIndexArgSignature (FunctionKind, pFunctionType);
		if (m_IndexArgSignature.IsEmpty ())
		{
			m_IndexArgSignature = IndexArgSignature;
		}
		else if (m_IndexArgSignature != IndexArgSignature)
		{
			err::SetFormatStringError ("index arguments mismatch in property accessors");
			return false;
		}
	}
	else if (!pFunctionType->GetArgArray ().IsEmpty ())
	{
		err::SetFormatStringError ("%s must have no arguments", GetFunctionKindString (FunctionKind));
		return false;
	}

	int FunctionKindMask = 1 << FunctionKind;
	if (!(m_MethodMask & FunctionKindMask))
	{
		m_MethodMask |= FunctionKindMask;
	}
	else if (FunctionKind != EFunction_Setter)
	{
		err::SetFormatStringError ("property already has '%s' method", GetFunctionKindString (FunctionKind));
		return false;
	}

	return true;
}

rtl::CString
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

	rtl::CArrayT <CFunctionArg*> ArgArray = pFunctionType->GetArgArray ();
	size_t ArgCount = ArgArray.GetCount ();
	ASSERT (ArgCount);

	return pFunctionType->CreateArgSignature (ArgArray, ArgCount - 1, 0);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
