#include "stdafx.h"
#include "axl_jnc_Variable.h"

namespace axl {
namespace jnc {

//.............................................................................

CVariable::CVariable ()
{
	m_ItemKind = EModuleItem_Variable;
	m_VariableKind = EVariable_Global;
	m_pType = NULL;
	m_pLlvmAlloca = NULL;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
