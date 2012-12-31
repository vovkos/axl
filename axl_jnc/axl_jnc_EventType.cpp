#include "stdafx.h"
#include "axl_jnc_EventType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CEventType::CEventType ()
{
	m_TypeKind = EType_Event;
	m_Size = sizeof (TEvent);
	m_pFunctionPointerType = NULL;
	m_pEventStructType = NULL;
	m_pHandlerStructType = NULL;
}

CStructType* 
CEventType::GetEventStructType ()
{
	if (m_pEventStructType)
		return m_pEventStructType;

	m_pEventStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	m_pEventStructType->m_Tag.Format (_T("event"));
	m_pEventStructType->CreateMember (GetHandlerStructType ()->GetPointerType (EType_Pointer_u));
	m_pEventStructType->CalcLayout ();

	return m_pEventStructType;
}

CStructType* 
CEventType::GetHandlerStructType ()
{
	if (m_pHandlerStructType)
		return m_pHandlerStructType;

	m_pHandlerStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	m_pHandlerStructType->m_Tag.Format (_T("handler"));
	m_pHandlerStructType->CreateMember (m_pFunctionPointerType);
	m_pHandlerStructType->CreateMember (m_pHandlerStructType->GetPointerType (EType_Pointer_u));
	m_pHandlerStructType->CreateMember (m_pHandlerStructType->GetPointerType (EType_Pointer_u));
	m_pHandlerStructType->CalcLayout ();

	return m_pHandlerStructType;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
