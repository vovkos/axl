#include "pch.h"
#include "axl_jnc_ScopeLevelStack.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

void
CScopeLevelStack::TakeOver (CScopeLevelStack* pSrcStack)
{
	m_pModule = pSrcStack->m_pModule;
	m_List.TakeOver (&pSrcStack->m_List);
	m_Stack = pSrcStack->m_Stack;
	pSrcStack->m_Stack.Clear ();
}

CValue
CScopeLevelStack::GetScopeLevel (size_t Level)
{
	TEntry* pEntry = GetEntry (Level);
	if (pEntry->m_ScopeLevelValue)
		return pEntry->m_ScopeLevelValue;

	CFunction* pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();
	CBasicBlock* pPrevBlock = m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->GetEntryBlock ());

	CType* pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT);

	CValue ScopeBaseLevelValue = m_pModule->m_FunctionMgr.GetScopeLevel ();
	CValue ScopeIncValue (Level, pType);
	CValue ScopeLevelValue;

	m_pModule->m_LlvmIrBuilder.CreateAdd_i (
		ScopeBaseLevelValue,
		ScopeIncValue,
		pType,
		&ScopeLevelValue
		);

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pPrevBlock);

	pEntry->m_ScopeLevelValue = ScopeLevelValue;
	return ScopeLevelValue;
}

CValue
CScopeLevelStack::GetObjHdr (size_t Level)
{
	TEntry* pEntry = GetEntry (Level);
	if (pEntry->m_ObjHdrValue)
		return pEntry->m_ObjHdrValue;

	CFunction* pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();
	CBasicBlock* pPrevBlock = m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->GetEntryBlock ());

	CType* pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT);
	CValue ObjHdrValue;

	m_pModule->m_LlvmIrBuilder.CreateAlloca (pType, "scopeLevel", pType, &ObjHdrValue);

	CValue ScopeLevelValue = GetScopeLevel (Level);
	m_pModule->m_LlvmIrBuilder.CreateStore (ScopeLevelValue, ObjHdrValue);

	pType = m_pModule->m_TypeMgr.GetStdType (EStdType_ObjHdrPtr);
	m_pModule->m_LlvmIrBuilder.CreateBitCast (ObjHdrValue, pType, &ObjHdrValue);

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pPrevBlock);

	pEntry->m_ObjHdrValue = ObjHdrValue;
	return ObjHdrValue;
}

CScopeLevelStack::TEntry*
CScopeLevelStack::GetEntry (size_t Level)
{
	size_t Count = m_Stack.GetCount ();
	if (Level >= Count)
		m_Stack.SetCount (Level + 1);

	if (m_Stack [Level])
		return m_Stack [Level];

	TEntry* pEntry = AXL_MEM_NEW (TEntry);
	m_List.InsertTail (pEntry);
	m_Stack [Level] = pEntry;
	return pEntry;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
