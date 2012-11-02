#include "stdafx.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

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
	m_pLlvmModule = NULL;
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
