#include "stdafx.h"
#include "axl_jnc_ConstMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CConstMgr::CConstMgr ()
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
