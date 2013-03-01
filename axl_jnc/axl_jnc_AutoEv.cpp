#include "stdafx.h"
#include "axl_jnc_AutoEv.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CAutoEv::CAutoEv ()
{
	m_ItemKind = EModuleItem_AutoEv;
	m_pType = NULL;
	m_pStart = NULL;
	m_pStop = NULL;
	m_pParentClassType = NULL;
	m_pParentClassFieldMember = NULL;
	m_pFieldStructType = NULL;
	m_pStaticDataVariable = NULL;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
