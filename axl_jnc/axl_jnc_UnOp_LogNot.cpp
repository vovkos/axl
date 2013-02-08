#include "stdafx.h"
#include "axl_jnc_UnOp_LogNot.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

bool
CUnOp_LogNot::Operator (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	err::SetFormatStringError (_T("unary '!' is not yet implemented"));
	return false;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
