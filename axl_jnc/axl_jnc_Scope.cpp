#include "stdafx.h"
#include "axl_jnc_Scope.h"

namespace axl {
namespace jnc {

//.............................................................................

CScope::CScope ()
{
	m_ItemKind = EModuleItem_Scope;
	m_NamespaceKind = ENamespace_Scope;
	m_pCodeBegin = NULL;
	m_pCodeEnd = NULL;
	m_pBreakBlock = NULL;
	m_pContinueBlock = NULL;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
