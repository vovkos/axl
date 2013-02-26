#include "stdafx.h"
#include "axl_jnc_UnOp.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetUnOpKindString (EUnOp OpKind)
{
	static const tchar_t* StringTable [EUnOp__Count] = 
	{
		_T("undefined-unary-operator"),  // EUnOp_Undefined = 0,
		_T("+"),                         // EUnOp_Plus,
		_T("-"),                         // EUnOp_Minus,
		_T("~"),                         // EUnOp_BwNot,	
		_T("&"),                         // EUnOp_Addr,
		_T("*"),                         // EUnOp_Indir,	
		_T("!"),                         // EUnOp_LogNot,
		_T("++"),                        // EUnOp_PreInc,
		_T("--"),                        // EUnOp_PreDec,
		_T("postfix ++"),                // EUnOp_PostInc,
		_T("postfix --"),                // EUnOp_PostDec,	
		_T("->"),                        // EUnOp_Ptr,
	};

	return (size_t) OpKind < EUnOp__Count ? 
		StringTable [OpKind] : 
		StringTable [EUnOp_Undefined];
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
