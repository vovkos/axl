#include "stdafx.h"
#include "axl_jnc_AutoEvType.h"

namespace axl {
namespace jnc {

//.............................................................................

CAutoEvType::CAutoEvType ()
{
	m_TypeKind = EType_AutoEv;
	m_pStarterType = NULL;
	m_pStopperType = NULL;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {

