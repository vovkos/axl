#include "pch.h"
#include "axl_jnc_BasicBlock.h"

namespace axl {
namespace jnc {

//.............................................................................

CBasicBlock::CBasicBlock ()
{
	m_pLlvmBlock = NULL;
	m_pFunction = NULL;
	m_Flags = 0;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
 