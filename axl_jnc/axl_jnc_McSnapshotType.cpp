#include "stdafx.h"
#include "axl_jnc_McSnapshotType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CMcSnapshotType::CMcSnapshotType ()
{
	m_TypeKind = EType_McSnapshot;
	m_pTargetType = NULL;
	m_pCallMethod = NULL;
}

void
CMcSnapshotType::PrepareTypeString ()
{
	m_TypeString += _T("mcsnapshot ");
	m_TypeString += m_pTargetType->GetTypeModifierString ();
	m_TypeString += m_pTargetType->GetTargetType ()->GetArgTypeString ();
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
