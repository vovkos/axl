#include "pch.h"
#include "axl_jnc_BinOp.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const char*
GetBinOpKindString (EBinOp OpKind)
{
	static const char* StringTable [EBinOp__Count] = 
	{
		"undefined-binary-operator",  // EBinOp_Undefined = 0,
		"+",                          // EBinOp_Add,
		"-",                          // EBinOp_Sub,
		"*",                          // EBinOp_Mul,
		"/",                          // EBinOp_Div,
		"%",                          // EBinOp_Mod,	
		"<<",                         // EBinOp_Shl,
		">>",                         // EBinOp_Shr,	
		"&",                          // EBinOp_BwAnd,
		"^",                          // EBinOp_BwXor,	
		"|",                          // EBinOp_BwOr,
		"@",                          // EBinOp_At,
		"[]",                         // EBinOp_Idx,
		"==",                         // EBinOp_Eq,
		"!=",                         // EBinOp_Ne,
		"<",                          // EBinOp_Lt,
		">",                          // EBinOp_Le,
		"<=",                         // EBinOp_Gt,
		">=",                         // EBinOp_Ge,
		"&&",                         // EBinOp_LogAnd,
		"||",                         // EBinOp_LogOr,
		"=",                          // EBinOp_Assign,
		":=",                         // EBinOp_ClassAssign,
		"+=",                         // EBinOp_AddAssign,
		"-=",                         // EBinOp_SubAssign,
		"*=",                         // EBinOp_MulAssign,
		"/=",                         // EBinOp_DivAssign,
		"%=",                         // EBinOp_ModAssign,
		"<<=",                        // EBinOp_ShlAssign,
		">>=",                        // EBinOp_ShrAssign,
		"&=",                         // EBinOp_AndAssign,
		"^=",                         // EBinOp_XorAssign,
		"|=",                         // EBinOp_OrAssign,
		"@=",                         // EBinOp_AtAssign,
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

bool
IBinaryOperator::GetResultType (
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue
	)
{
	CType* pType = GetResultType (OpValue1, OpValue2);
	if (!pType)
		return false;

	pResultValue->SetType (pType);
	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
