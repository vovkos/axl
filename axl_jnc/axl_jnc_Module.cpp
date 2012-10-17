#include "stdafx.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CModule::CModule ()
{
	m_AttributeMgr.m_pModule = this;
	m_TypeMgr.m_pModule = this;
	m_NamespaceMgr.m_pModule = this;
	m_FunctionMgr.m_pModule = this;
	m_VariableMgr.m_pModule = this;
	m_ConstMgr.m_pModule = this;
	m_ControlFlowMgr.m_pModule = this;
}

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
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
