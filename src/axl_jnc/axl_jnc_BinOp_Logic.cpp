#include "pch.h"
#include "axl_jnc_BinOp_Logic.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CType*
CBinOp_LogAnd::GetResultType (
	const CValue& OpValue1,
	const CValue& OpValue2
	)
{
	return m_pModule->m_TypeMgr.GetPrimitiveType (EType_Bool);
}

bool
CBinOp_LogAnd::Operator (
	const CValue& RawOpValue1,
	const CValue& RawOpValue2,
	CValue* pResultValue
	)
{
	err::SetFormatStringError ("binary '&&' is not yet implemented");
	return false;
}

//.............................................................................

CType*
CBinOp_LogOr::GetResultType (
	const CValue& OpValue1,
	const CValue& OpValue2
	)
{
	return m_pModule->m_TypeMgr.GetPrimitiveType (EType_Bool);
}

bool
CBinOp_LogOr::Operator (
	const CValue& RawOpValue1,
	const CValue& RawOpValue2,
	CValue* pResultValue
	)
{
	err::SetFormatStringError ("binary '||' is not yet implemented");
	return false;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
