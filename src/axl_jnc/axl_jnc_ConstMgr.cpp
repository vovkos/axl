#include "pch.h"
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

void
CConstMgr::Clear ()
{
	m_ConstList.Clear ();
	m_UnsafeThinDataPtrValidator = ref::EPtr_Null;
}

CThinDataPtrValidator*
CConstMgr::GetUnsafeThinDataPtrValidator ()
{
	if (m_UnsafeThinDataPtrValidator)
		return m_UnsafeThinDataPtrValidator;

	void* pRangeBegin = NULL;

	m_UnsafeThinDataPtrValidator = AXL_REF_NEW (CThinDataPtrValidator);
	m_UnsafeThinDataPtrValidator->m_Size = -1;
	m_UnsafeThinDataPtrValidator->m_ScopeValidatorValue.SetConstSizeT (0);
	m_UnsafeThinDataPtrValidator->m_RangeBeginValue.CreateConst (&pRangeBegin, m_pModule->GetSimpleType (EStdType_BytePtr));

	return m_UnsafeThinDataPtrValidator;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
