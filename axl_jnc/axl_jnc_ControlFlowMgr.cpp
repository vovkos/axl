#include "stdafx.h"
#include "axl_jnc_ControlFlowMgr.h"

namespace axl {
namespace jnc {

//.............................................................................

void
CControlFlowMgr::Clear ()
{
	m_BlockList.Clear ();
	m_pCurrentBlock = NULL;
}

CBasicBlock* 
CControlFlowMgr::CreateBlock ()
{
	CBasicBlock* pBlock = AXL_MEM_NEW (CBasicBlock);
	m_BlockList.InsertTail (pBlock);
	return pBlock;
}

void
CControlFlowMgr::SetCurrentBlock (
	CBasicBlock* pBlock,
	int Flags
	)
{
	m_pCurrentBlock = pBlock;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
