#include "pch.h"
#include "axl_jnc_Variable.h"

namespace axl {
namespace jnc {

//.............................................................................

CVariable::CVariable ()
{
	m_ItemKind = EModuleItem_Variable;
	m_pType = NULL;
	m_PtrTypeFlags = 0;
	m_pScope = NULL;
	m_pLlvmValue = NULL;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
