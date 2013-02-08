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
	m_pFunctionPtrType = NULL;
	m_pEventStructType = NULL;
	m_pHandlerStructType = NULL;
}

CStructType* 
CEventType::GetEventStructType ()
{
	if (m_pEventStructType)
		return m_pEventStructType;

	CDataPtrType* pHandlerPtrType = GetHandlerStructType ()->GetDataPtrType (EDataPtrType_Unsafe);

	m_pEventStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	m_pEventStructType->m_Tag.Format (_T("event"));
	m_pEventStructType->CreateMember (pHandlerPtrType);
	m_pEventStructType->CreateMember (pHandlerPtrType);
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

	CDataPtrType* pHandlerPtrType = m_pHandlerStructType->GetDataPtrType (EDataPtrType_Unsafe);

	m_pHandlerStructType->CreateMember (m_pFunctionPtrType);
	m_pHandlerStructType->CreateMember (pHandlerPtrType);
	m_pHandlerStructType->CreateMember (pHandlerPtrType);
	m_pHandlerStructType->CalcLayout ();

	return m_pHandlerStructType;
}

void
CEventType::PrepareTypeString ()
{
	m_TypeString = _T("event ");
	m_TypeString += m_pFunctionPtrType->GetTypeModifierString ();
	m_TypeString += m_pFunctionPtrType->GetTargetType ()->GetArgTypeString ();
}

void
CEventType::PrepareLlvmType ()
{
	m_pLlvmType = GetEventStructType ()->GetLlvmType ();
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
