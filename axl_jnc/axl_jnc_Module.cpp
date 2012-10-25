#include "stdafx.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

// warning C4355: 'this' : used in base member initializer list

#pragma warning (disable: 4355)

CModule::CModule ():
	m_TypeMgr (this),
	m_AttributeMgr (this),
	m_NamespaceMgr (this),
	m_FunctionMgr (this),
	m_VariableMgr (this),
	m_ConstMgr (this),
	m_ControlFlowMgr (this),
	m_OperatorMgr (this)
{
	m_pLlvmModule = NULL;
}

#pragma warning (default: 4355)

void
CModule::Clear ()
{
	m_FilePath.Clear ();
	m_TypeMgr.Clear ();
	m_NamespaceMgr.Clear ();
	m_FunctionMgr.Clear ();
	m_VariableMgr.Clear ();
	m_ConstMgr.Clear ();
	m_ControlFlowMgr.Clear ();
	m_OperatorMgr.Clear ();

	if (m_pLlvmModule)
		AXL_MEM_DELETE (m_pLlvmModule);
}

bool
CModule::Create (const rtl::CString& FilePath)
{
	Clear ();
	m_OperatorMgr.AddStdOperators ();
	m_FilePath = FilePath;
	m_pLlvmModule = (llvm::Module*) AXL_MEM_ALLOC (sizeof (llvm::Module));
	new (m_pLlvmModule) llvm::Module ((const tchar_t*) m_FilePath, llvm::getGlobalContext());
	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
