#include "stdafx.h"
#include "axl_jnc_Variable.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetVariableKindString (EVariable VariableKind)
{
	static const tchar_t* StringTable [EVariable__Count] = 
	{
		_T("undefined-variable-kind"),  // EVariable_Undefined = 0,
		_T("global-variable"),          // EVariable_Global,
		_T("local-variable"),           // EVariable_Local,
	};

	return VariableKind >= 0 && VariableKind < EVariable__Count ? 
		StringTable [VariableKind] : 
		StringTable [EVariable_Undefined];
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
