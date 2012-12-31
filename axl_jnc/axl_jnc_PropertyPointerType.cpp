#include "stdafx.h"
#include "axl_jnc_PropertyPointerType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CPropertyPointerType::CPropertyPointerType ()
{
	m_TypeKind = EType_PropertyPointer;
	m_Size = sizeof (TPropertyPtr);
	m_pPropertyType = NULL;
	m_pMemberPropertyType = NULL;
	m_pPointerStructType = NULL;
}

CPropertyType* 
CPropertyPointerType::GetMemberPropertyType ()
{
	if (m_pMemberPropertyType)
		return m_pMemberPropertyType;

	return m_pMemberPropertyType;
}

CStructType* 
CPropertyPointerType::GetPointerStructType ()
{
	if (m_pPointerStructType)
		return m_pPointerStructType;

	m_pPointerStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	m_pPointerStructType->m_Tag.Format (_T("pprop"));
	m_pPointerStructType->CreateMember (m_pPropertyType->GetVTableStructType ()->GetPointerType (EType_Pointer_u));
	m_pPointerStructType->CreateMember (m_pModule->m_TypeMgr.GetStdType (EStdType_AbstractInterfacePtr));
	m_pPointerStructType->CalcLayout ();

	return m_pPointerStructType;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
