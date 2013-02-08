#include "stdafx.h"
#include "axl_jnc_CastOp_Array.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

ECast
CCast_Array::GetCastKind (
	const CValue& OpValue,
	CType* pType
	)
{
	return ECast_Explicit;
}

bool
CCast_Array::ConstCast (
	const CValue& OpValue,
	CType* pType,
	void* pDst
	)
{
	err::SetFormatStringError (_T("CCast_Array::ConstCast is not yet implemented"));
	return false;
}

bool
CCast_Array::LlvmCast (
	EAlloc AllocKind,
	const CValue& OpValue,
	CType* pType,
	CValue* pResultValue
	)
{
	err::SetFormatStringError (_T("CCast_Array::LlvmCast is not yet implemented"));
	return false;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
