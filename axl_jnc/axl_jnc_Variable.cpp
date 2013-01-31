#include "stdafx.h"
#include "axl_jnc_Variable.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetVariableKindString (EVariable VariableKind)
{
	switch (VariableKind)
	{
	case EVariable_Global:
		return _T("global-variable");

	case EVariable_Local:
		return _T("local-variable");

	default:
		return _T("undefined-variable-kind");
	};
}

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

} // namespace jnc {
} // namespace axl {
