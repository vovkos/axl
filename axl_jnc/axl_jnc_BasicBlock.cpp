#include "stdafx.h"
#include "axl_jnc_BasicBlock.h"

namespace axl {
namespace jnc {

//.............................................................................

CBasicBlock::CBasicBlock ()
{
	m_pLlvmBlock = NULL;
	m_pFunction = NULL;
	m_HasReturn = EHasReturn_Undefined;
	m_Flags = 0;
}

EHasReturn
CBasicBlock::HasReturn ()
{
	if (m_Flags & EBasicBlockFlag_IsHasReturnReady)
	{
		ASSERT (m_HasReturn != EHasReturn_Undefined);
		return m_HasReturn;
	}

	if (m_Flags & EBasicBlockFlag_IsHasReturnCalc)
		return EHasReturn_Undefined;

	static EHasReturn HasReturnTable [4] [4] = 
	{
		{ EHasReturn_Undefined, EHasReturn_None, EHasReturn_Some, EHasReturn_All  },
		{ EHasReturn_Undefined, EHasReturn_None, EHasReturn_Some, EHasReturn_Some },
		{ EHasReturn_Undefined, EHasReturn_Some, EHasReturn_Some, EHasReturn_Some },
		{ EHasReturn_Undefined, EHasReturn_Some, EHasReturn_Some, EHasReturn_All  },
	};
	
	size_t Count = m_JumpArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CBasicBlock* pBlock = m_JumpArray [i];
		EHasReturn HasReturn = pBlock->HasReturn ();
		
		m_HasReturn = HasReturnTable [m_HasReturn] [HasReturn];
	}

	if (m_HasReturn == EHasReturn_Undefined)
		m_HasReturn = EHasReturn_None;

	m_Flags |= EBasicBlockFlag_IsHasReturnReady;
	return m_HasReturn;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
