#include "stdafx.h"
#include "axl_jnc_BinOp.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetBinOpKindString (EBinOp OpKind)
{
	switch (OpKind)
	{
	case EBinOp_Add:
		return _T("+");

	case EBinOp_Sub:
		return _T("-");

	case EBinOp_Mul:
		return _T("*");

	case EBinOp_Div:
		return _T("/");

	case EBinOp_Mod:
		return _T("%");

	case EBinOp_Shl:
		return _T("<<");

	case EBinOp_Shr:
		return _T(">>");

	case EBinOp_BwAnd:
		return _T("&");

	case EBinOp_BwXor:
		return _T("^");

	case EBinOp_BwOr:
		return _T("|");

	case EBinOp_At:
		return _T("@");

	case EBinOp_Idx:
		return _T("[]");

	case EBinOp_LogAnd:
		return _T("&&");

	case EBinOp_LogOr:
		return _T("||");

	case EBinOp_Eq:
		return _T("==");

	case EBinOp_Ne:
		return _T("!=");

	case EBinOp_Lt:
		return _T("<");

	case EBinOp_Le:
		return _T("<=");

	case EBinOp_Gt:
		return _T(">");

	case EBinOp_Ge:
		return _T(">=");

	case EBinOp_Assign:
		return _T("=");

	case EBinOp_ClassAssign:
		return _T(":=");

	case EBinOp_AddAssign:
		return _T("+=");

	case EBinOp_SubAssign:
		return _T("-=");

	case EBinOp_MulAssign:
		return _T("*=");

	case EBinOp_DivAssign:
		return _T("/=");

	case EBinOp_ModAssign:
		return _T("%=");

	case EBinOp_ShlAssign:
		return _T("<<=");

	case EBinOp_ShrAssign:
		return _T(">>=");

	case EBinOp_AndAssign:
		return _T("&=");

	case EBinOp_XorAssign:
		return _T("^=");

	case EBinOp_OrAssign:
		return _T("|=");

	case EBinOp_AtAssign:
		return _T("@=");

	default:
		return _T("undefined-binary-operator");
	};
}

//.............................................................................

IBinaryOperator::IBinaryOperator()
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);

	m_OpKind = EBinOp_Undefined;
	m_OpFlags1 = m_OpFlags2 = 0;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
