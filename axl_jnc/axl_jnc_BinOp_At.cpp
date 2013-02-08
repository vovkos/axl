#include "stdafx.h"
#include "axl_jnc_BinOp_At.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

bool
CBinOp_At::Operator (
	const CValue& RawOpValue1,
	const CValue& RawOpValue2,
	CValue* pResultValue
	)
{
	err::SetFormatStringError (_T("binary '@' is not yet implemented"));
	return false;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
