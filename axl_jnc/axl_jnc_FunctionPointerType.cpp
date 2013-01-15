#include "stdafx.h"
#include "axl_jnc_FunctionPointerType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CFunctionPointerType::CFunctionPointerType ()
{
	m_TypeKind = EType_FunctionPointer;
	m_Size = sizeof (TFunctionPtr);
	m_pFunctionType = NULL;
	m_pShortFunctionType = NULL;
	m_pPointerStructType = NULL;
	m_pEventType = NULL;
}

CStructType* 
CFunctionPointerType::GetPointerStructType ()
{
	if (m_pPointerStructType)
		return m_pPointerStructType;

	m_pPointerStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	m_pPointerStructType->m_Tag.Format (_T("pfn"));
	m_pPointerStructType->CreateMember (m_pFunctionType->GetPointerType (EType_Pointer_u));
	m_pPointerStructType->CreateMember (m_pModule->m_TypeMgr.GetStdType (EStdType_AbstractInterfacePtr));
	m_pPointerStructType->CalcLayout ();

	return m_pPointerStructType;
}

CEventType* 
CFunctionPointerType::GetEventType ()
{
	return m_pModule->m_TypeMgr.GetEventType (this);
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
