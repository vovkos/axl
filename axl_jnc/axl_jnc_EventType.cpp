#include "stdafx.h"
#include "axl_jnc_EventType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetEventOperatorKindString (EEventOp OpKind)
{
	switch (OpKind)
	{
	case EEventOp_SetHandler:
		return _T("set-event-handler");

	case EEventOp_AddHandler:
		return _T("add-event-handler");

	case EEventOp_RemoveHandler:
		return _T("remove-event-handler");

	default:
		return _T("undefined-event-operator");
	}
}

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

	CPointerType* pHandlerPointerType = GetHandlerStructType ()->GetPointerType (EType_Pointer_u);

	m_pEventStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	m_pEventStructType->m_Tag.Format (_T("event"));
	m_pEventStructType->CreateMember (pHandlerPointerType);
	m_pEventStructType->CreateMember (pHandlerPointerType);
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

	CPointerType* pHandlerPointerType = m_pHandlerStructType->GetPointerType (EType_Pointer_u);

	m_pHandlerStructType->CreateMember (m_pFunctionPointerType);
	m_pHandlerStructType->CreateMember (pHandlerPointerType);
	m_pHandlerStructType->CreateMember (pHandlerPointerType);
	m_pHandlerStructType->CalcLayout ();

	return m_pHandlerStructType;
}

rtl::CString
CEventType::CreateTypeString (CFunctionType* pFunctionType)
{
	rtl::CString String = _T("event ");
	String += pFunctionType->CreateArgTypeString ();
	return String;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
