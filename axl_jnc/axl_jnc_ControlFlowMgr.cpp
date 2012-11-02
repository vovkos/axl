#include "stdafx.h"
#include "axl_jnc_ControlFlowMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CControlFlowMgr::CControlFlowMgr ():
	m_LlvmBuilder (llvm::getGlobalContext())
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);
}

void
CControlFlowMgr::Clear ()
{
	m_BlockList.Clear ();
	m_pCurrentBlock = NULL;
}

CBasicBlock* 
CControlFlowMgr::CreateBlock (
	const rtl::CString& Name,
	bool IsCurrent
	)
{
	CFunction* pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();

	CBasicBlock* pBlock = AXL_MEM_NEW (CBasicBlock);
	pBlock->m_Name = Name;
	pBlock->m_pLlvmBlock = llvm::BasicBlock::Create (
		llvm::getGlobalContext (), 
		(const tchar_t*) Name,
		pFunction ? pFunction->GetLlvmFunction () : NULL
		);

	m_BlockList.InsertTail (pBlock);

	if (IsCurrent)
		SetCurrentBlock (pBlock, 0);

	return pBlock;
}

void
CControlFlowMgr::SetCurrentBlock (
	CBasicBlock* pBlock,
	int Flags
	)
{
	m_pCurrentBlock = pBlock;
	m_LlvmBuilder.SetInsertPoint (pBlock->GetLlvmBlock ());
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
