#include "pch.h"
#include "axl_jnc_Variable.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CVariable::CVariable ()
{
	m_ItemKind = EModuleItem_Variable;
	m_pType = NULL;
	m_pType_i = NULL;
	m_PtrTypeFlags = 0;
	m_pScope = NULL;
	m_pTlsField = NULL;
	m_pLlvmValue = NULL;
	m_pLlvmAllocValue = NULL;
}

bool
CVariable::CalcLayout ()
{
	if (m_pType_i)
		m_pType = m_pType_i->GetActualType ();

	return m_pType->EnsureLayout ();
}

llvm::Value* 
CVariable::GetLlvmValue ()
{
	if (m_pLlvmValue)
		return m_pLlvmValue;
	
	ASSERT (m_StorageKind == EStorage_Thread);
	m_pModule->m_VariableMgr.AllocateTlsVariable (this);
	return m_pLlvmValue;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
