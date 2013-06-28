#include "pch.h"
#include "axl_jnc_Variable.h"

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

//.............................................................................

} // namespace jnc {
} // namespace axl {
