#include "stdafx.h"
#include "axl_jnc_BinOp_Logic.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

bool
CBinOp_LogAnd::Operator (
	const CValue& RawOpValue1,
	const CValue& RawOpValue2,
	CValue* pResultValue
	)
{
	err::SetFormatStringError (_T("binary '&&' is not yet implemented"));
	return false;
}

//.............................................................................

bool
CBinOp_LogOr::Operator (
	const CValue& RawOpValue1,
	const CValue& RawOpValue2,
	CValue* pResultValue
	)
{
	err::SetFormatStringError (_T("binary '||' is not yet implemented"));
	return false;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
