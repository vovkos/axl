#include "stdafx.h"
#include "axl_jnc_AutoEvType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CAutoEvType::CAutoEvType ()
{
	m_TypeKind = EType_AutoEv;
	m_pStarterType = NULL;
	m_pStopperType = NULL;
	m_pStdObjectAutoEvMemberType = NULL;
	m_pVTableStructType = NULL;
	m_pShortType = NULL;
}

CAutoEvPtrType* 
CAutoEvType::GetAutoEvPtrType (
	EType TypeKind,
	EAutoEvPtrType PtrTypeKind,
	int Flags
	)
{
	return m_pModule->m_TypeMgr.GetAutoEvPtrType (this, TypeKind, PtrTypeKind, Flags);
}

CAutoEvType*
CAutoEvType::GetAutoEvMemberType (CClassType* pClassType)
{
	return m_pModule->m_TypeMgr.GetAutoEvMemberType (pClassType, this);
}

CAutoEvType*
CAutoEvType::GetStdObjectAutoEvMemberType ()
{
	return m_pModule->m_TypeMgr.GetStdObjectAutoEvMemberType (this);
}

CAutoEvType*
CAutoEvType::GetShortType  ()
{
	return m_pModule->m_TypeMgr.GetShortAutoEvType (this);
}

CStructType*
CAutoEvType::GetVTableStructType ()
{
	return m_pModule->m_TypeMgr.GetAutoEvVTableStructType (this);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
