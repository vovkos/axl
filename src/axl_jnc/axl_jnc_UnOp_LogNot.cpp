#include "pch.h"
#include "axl_jnc_UnOp_LogNot.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CType*
CUnOp_LogNot::GetResultType (const CValue& OpValue)
{
	return m_pModule->m_TypeMgr.GetPrimitiveType (EType_Bool);
}

bool
CUnOp_LogNot::Operator (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	err::SetFormatStringError ("unary '!' is not yet implemented");
	return false;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
