#include "stdafx.h"
#include "axl_jnc_PropertyTemplate.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

bool
CPropertyTemplate::AddMethodMember (
	EFunction FunctionKind,
	CFunctionType* pFunctionType
	)
{
	if (FunctionKind != EFunction_Getter && FunctionKind != EFunction_Setter)
	{
		err::SetFormatStringError (_T("property templates can only have accessors"));
		return false;
	}

	bool Result = m_Verifier.AddMethodMember (FunctionKind, pFunctionType);
	if (!Result)
		return false;

	if (FunctionKind == EFunction_Getter)
	{
		m_pGetterType = pFunctionType;
		return true;
	}

	Result = m_SetterType.AddOverload (pFunctionType);
	if (!Result)
		return false;

	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
