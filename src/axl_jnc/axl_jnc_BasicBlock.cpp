#include "pch.h"
#include "axl_jnc_BasicBlock.h"
#include "axl_jnc_Function.h"

namespace axl {
namespace jnc {

//.............................................................................

CBasicBlock::CBasicBlock ()
{
	m_pModule = NULL;
	m_pLlvmBlock = NULL;
	m_pFunction = NULL;
	m_Flags = 0;
}

CValue
CBasicBlock::GetBlockAddressValue ()
{
	llvm::BlockAddress* pLlvmAddress = llvm::BlockAddress::get (m_pFunction->GetLlvmFunction (), m_pLlvmBlock);
	
	CValue Value;
	Value.SetLlvmValue (pLlvmAddress, GetSimpleType (m_pModule, EStdType_BytePtr));
	return Value;	
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
 