#include "stdafx.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

void
CModule::Clear ()
{
	m_TypeMgr.Clear ();
	m_NamespaceMgr.Clear ();
	m_FunctionMgr.Clear ();
	m_VariableMgr.Clear ();
	m_ConstMgr.Clear ();
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
