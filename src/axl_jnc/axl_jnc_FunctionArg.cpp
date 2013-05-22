#include "pch.h"
#include "axl_jnc_FunctionArg.h"

namespace axl {
namespace jnc {

//.............................................................................

CFunctionArg::CFunctionArg ()
{
	m_ItemKind = EModuleItem_FunctionArg;
	m_pType = NULL;
	m_PtrTypeFlags = 0;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
