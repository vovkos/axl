#include "pch.h"
#include "axl_log_JancyRepresenter.h"

namespace axl {
namespace log {

//.............................................................................

TJancyRepresenterTargetVTable CJancyRepresenter::m_JancyRepresenterTargetVTable =
{
	&CJancyRepresenter::JancyRepresenterTarget_AddPart,
};

CJancyRepresenter::CJancyRepresenter ()
{
	m_pJancyRepresenter = NULL;
	m_pJancyRepresenterTargetType = NULL;
	m_pRuntime = NULL;
}

bool
CJancyRepresenter::Create (
	jnc::CClassType* pType,
	jnc::CRuntime* pRuntime
	)
{
	Destroy ();

	jnc::CModule* pModule = pType->GetModule ();

	m_pJancyRepresenter = (TJancyRepresenter*) pRuntime->CreateAndPinObject (pType);
	if (!m_pJancyRepresenter)
		return false;

	// m_pJancyRepresenterTargetType = pModule->m_TypeMgr.
	m_pRuntime = pRuntime;
	return true;
}

void
CJancyRepresenter::Destroy ()
{
	if (m_pJancyRepresenter)
	{
		ASSERT (m_pRuntime);
		m_pRuntime->UnpinObject (m_pJancyRepresenter);
	}

	m_pJancyRepresenter = NULL;
	m_pRuntime = NULL;
}

bool
CJancyRepresenter::Represent (
	CRepresenterTarget* pTarget,
	uint_t PacketCode,
	const void* p,
	size_t Size,
	uint64_t FoldFlags
	)
{
	if (!m_pJancyRepresenter)
		return false;

	jnc::TObject Object = { 0 };
	Object.m_pType = m_pJancyRepresenterTargetType;
	Object.m_ScopeLevel = 0; // TODO: calc property scope level
	Object.m_Flags = jnc::EObjectFlag_Alive | jnc::EObjectFlag_Stack;

	TJancyRepresenterTarget Target;
	Target.m_pObject = &Object;
	Target.m_pVTable = &m_JancyRepresenterTargetVTable;
	Target.m_PacketCode = pTarget->GetPacketCode ();
	Target.m_Timestamp = pTarget->GetTimestamp ();
	Target.m_LineAttr = pTarget->m_LineAttr;
	Target.m_pActualTarget = pTarget;

	jnc::CScopeThreadRuntime ScopeRuntime (m_pRuntime);
	TJancyRepresenterVTable* pVTable = (TJancyRepresenterVTable*) m_pJancyRepresenter->m_pVTable;

	jnc::TDataPtr Ptr;
	Ptr.m_p = (void*) p;
	Ptr.m_pRangeBegin = (void*) p;
	Ptr.m_pRangeEnd = (char*) p + Size;
	Ptr.m_ScopeLevel = 0;

	return pVTable->m_pfRepresent (m_pJancyRepresenter, &Target, PacketCode, Ptr, Size, FoldFlags);
}

void
CJancyRepresenter::JancyRepresenterTarget_AddPart (
	TJancyRepresenterTarget* pTarget,
	EPart PartKind,
	uint_t PartCode,
	jnc::TDataPtr Ptr,
	size_t Size
	)
{
	if (Size == -1)
		Size = strlen ((char*) Ptr.m_p);

	pTarget->m_pActualTarget->m_LineAttr = pTarget->m_LineAttr;
	pTarget->m_pActualTarget->AddPart (PartKind, PartCode, Ptr.m_p, Size);
}

//.............................................................................

} // namespace log
} // namespace axl
