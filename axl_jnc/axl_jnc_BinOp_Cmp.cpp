#include "stdafx.h"
#include "axl_jnc_BinOp_Cmp.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

llvm::Value*
CBinOp_Eq::LlvmOpInt (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return pModule->m_LlvmBuilder.CreateEq_i (OpValue1, OpValue2, pResultValue);
}

llvm::Value*
CBinOp_Eq::LlvmOpFp (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue
	)
{
	return pModule->m_LlvmBuilder.CreateEq_f (OpValue1, OpValue2, pResultValue);
}

//.............................................................................

llvm::Value*
CBinOp_Ne::LlvmOpInt (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return pModule->m_LlvmBuilder.CreateNe_i (OpValue1, OpValue2, pResultValue);
}

llvm::Value*
CBinOp_Ne::LlvmOpFp (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue
	)
{
	return pModule->m_LlvmBuilder.CreateNe_f (OpValue1, OpValue2, pResultValue);
}
	
//.............................................................................

llvm::Value*
CBinOp_Lt::LlvmOpInt (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return IsUnsigned ? 
		pModule->m_LlvmBuilder.CreateLt_u (OpValue1, OpValue2, pResultValue) :
		pModule->m_LlvmBuilder.CreateLt_i (OpValue1, OpValue2, pResultValue);
}

llvm::Value*
CBinOp_Lt::LlvmOpFp (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue
	)
{
	return pModule->m_LlvmBuilder.CreateLt_f (OpValue1, OpValue2, pResultValue);
}

//.............................................................................

llvm::Value*
CBinOp_Le::LlvmOpInt (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return IsUnsigned ? 
		pModule->m_LlvmBuilder.CreateLe_u (OpValue1, OpValue2, pResultValue) :
		pModule->m_LlvmBuilder.CreateLe_i (OpValue1, OpValue2, pResultValue);
}

llvm::Value*
CBinOp_Le::LlvmOpFp (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue
	)
{
	return pModule->m_LlvmBuilder.CreateLe_f (OpValue1, OpValue2, pResultValue);
}

//.............................................................................

llvm::Value*
CBinOp_Gt::LlvmOpInt (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return IsUnsigned ? 
		pModule->m_LlvmBuilder.CreateGt_u (OpValue1, OpValue2, pResultValue) :
		pModule->m_LlvmBuilder.CreateGt_i (OpValue1, OpValue2, pResultValue);
}

llvm::Value*
CBinOp_Gt::LlvmOpFp (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue
	)
{
	return pModule->m_LlvmBuilder.CreateGt_f (OpValue1, OpValue2, pResultValue);
}

//.............................................................................

llvm::Value*
CBinOp_Ge::LlvmOpInt (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue,
	bool IsUnsigned
	)
{
	return IsUnsigned ? 
		pModule->m_LlvmBuilder.CreateGe_u (OpValue1, OpValue2, pResultValue) :
		pModule->m_LlvmBuilder.CreateGe_i (OpValue1, OpValue2, pResultValue);
}

llvm::Value*
CBinOp_Ge::LlvmOpFp (
	CModule* pModule,
	const CValue& OpValue1,
	const CValue& OpValue2,
	CValue* pResultValue
	)
{
	return pModule->m_LlvmBuilder.CreateGe_f (OpValue1, OpValue2, pResultValue);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
