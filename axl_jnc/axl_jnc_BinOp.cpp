#include "stdafx.h"
#include "axl_jnc_BinOp.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetBinOpKindString (EBinOp OpKind)
{
	static const tchar_t* StringTable [EBinOp__Count] = 
	{
		_T("undefined-binary-operator"),  // EBinOp_Undefined = 0,
		_T("+"),                          // EBinOp_Add,
		_T("-"),                          // EBinOp_Sub,
		_T("*"),                          // EBinOp_Mul,
		_T("/"),                          // EBinOp_Div,
		_T("%"),                          // EBinOp_Mod,	
		_T("<<"),                         // EBinOp_Shl,
		_T(">>"),                         // EBinOp_Shr,	
		_T("&"),                          // EBinOp_BwAnd,
		_T("^"),                          // EBinOp_BwXor,	
		_T("|"),                          // EBinOp_BwOr,
		_T("@"),                          // EBinOp_At,
		_T("[]"),                         // EBinOp_Idx,
		_T("=="),                         // EBinOp_Eq,
		_T("!="),                         // EBinOp_Ne,
		_T("<"),                          // EBinOp_Lt,
		_T(">"),                          // EBinOp_Le,
		_T("<="),                         // EBinOp_Gt,
		_T(">="),                         // EBinOp_Ge,
		_T("&&"),                         // EBinOp_LogAnd,
		_T("||"),                         // EBinOp_LogOr,
		_T("="),                          // EBinOp_Assign,
		_T(":="),                         // EBinOp_ClassAssign,
		_T("+="),                         // EBinOp_AddAssign,
		_T("-="),                         // EBinOp_SubAssign,
		_T("*="),                         // EBinOp_MulAssign,
		_T("/="),                         // EBinOp_DivAssign,
		_T("%="),                         // EBinOp_ModAssign,
		_T("<<="),                        // EBinOp_ShlAssign,
		_T(">>="),                        // EBinOp_ShrAssign,
		_T("&="),                         // EBinOp_AndAssign,
		_T("^="),                         // EBinOp_XorAssign,
		_T("|="),                         // EBinOp_OrAssign,
		_T("@="),                         // EBinOp_AtAssign,
	};

	return (size_t) OpKind < EBinOp__Count ? 
		StringTable [OpKind] : 
		StringTable [EBinOp_Undefined];
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
