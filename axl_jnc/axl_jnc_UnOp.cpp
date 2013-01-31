#include "stdafx.h"
#include "axl_jnc_UnOp.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetUnOpKindString (EUnOp OpKind)
{
	switch (OpKind)
	{
	case EUnOp_Minus:
		return _T("-");

	case EUnOp_BwNot:
		return _T("~");

	case EUnOp_Addr:
		return _T("&");

	case EUnOp_Indir:
		return _T("*");

	case EUnOp_LogNot:
		return _T("!");

	case EUnOp_PreInc:
		return _T("++");

	case EUnOp_PreDec:
		return _T("--");

	case EUnOp_PostInc:
		return _T("postfix ++");

	case EUnOp_PostDec:
		return _T("postfix --");

	default:
		return _T("undefined-unary-operator");
	};
}

//.............................................................................

IUnaryOperator::IUnaryOperator ()
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);

	m_OpKind = EUnOp_Undefined;
	m_OpFlags = 0;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
