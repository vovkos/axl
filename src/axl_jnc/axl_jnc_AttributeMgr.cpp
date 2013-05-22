#include "pch.h"
#include "axl_jnc_AttributeMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CAttributeMgr::CAttributeMgr ()
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);
}

CAttributeBlock*
CAttributeMgr::CreateAttributeBlock ()
{
	CAttributeBlock* pAttributeBlock = AXL_MEM_NEW (CAttributeBlock);
	pAttributeBlock->m_pModule = m_pModule;
	m_AttributeBlockList.InsertTail (pAttributeBlock);
	return pAttributeBlock;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
