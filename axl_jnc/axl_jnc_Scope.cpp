#include "stdafx.h"
#include "axl_jnc_Scope.h"

namespace axl {
namespace jnc {

//.............................................................................

CScope::CScope ()
{
	m_ItemKind = EModuleItem_Scope;
	m_NamespaceKind = ENamespace_Scope;
	m_pItemDecl = this;
	m_Level = 1; // 0 is global
	m_pFunction = NULL;
	m_pBreakBlock = NULL;
	m_pContinueBlock = NULL;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
