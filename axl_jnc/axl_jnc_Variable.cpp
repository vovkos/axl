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
	m_pScope = NULL;
	m_pLlvmValue = NULL;
}

CVariable::~CVariable ()
{
	//if (m_VariableKind == EVariable_Global && m_pLlvmGlobalVariable)
	//	delete m_pLlvmGlobalVariable;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
