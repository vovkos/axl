#include "pch.h"
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
	m_pStdObjectMemberAutoEvType = NULL;
	m_pVTableStructType = NULL;
	m_pShortType = NULL;
}

CAutoEvPtrType* 
CAutoEvType::GetAutoEvPtrType (
	EType TypeKind,
	EAutoEvPtrType PtrTypeKind,
	uint_t Flags
	)
{
	return m_pModule->m_TypeMgr.GetAutoEvPtrType (this, TypeKind, PtrTypeKind, Flags);
}

CAutoEvType*
CAutoEvType::GetMemberAutoEvType (CClassType* pClassType)
{
	return m_pModule->m_TypeMgr.GetMemberAutoEvType (pClassType, this);
}

CAutoEvType*
CAutoEvType::GetStdObjectMemberAutoEvType ()
{
	return m_pModule->m_TypeMgr.GetStdObjectMemberAutoEvType (this);
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
