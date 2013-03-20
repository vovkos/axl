#include "stdafx.h"
#include "axl_jnc_NamedType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CType*
CNamedType::GetThisArgType (int ThisArgTypeFlags)
{
	if (m_TypeKind == EType_Class)
		return ((CClassType*) this)->GetClassPtrType (EClassPtrType_Normal, ThisArgTypeFlags);
	else
		return GetDataPtrType (EDataPtrType_Normal, ThisArgTypeFlags);
}

CFunctionType* 
CNamedType::GetMemberMethodType (
	CFunctionType* pShortType,
	int ThisArgTypeFlags
	)
{
	return m_pModule->m_TypeMgr.GetMemberMethodType (this, pShortType, ThisArgTypeFlags);
}

CPropertyType* 
CNamedType::GetMemberPropertyType (CPropertyType* pShortType)
{
	return m_pModule->m_TypeMgr.GetMemberPropertyType (this, pShortType);
}

CModuleItem*
CNamedType::FindItemTraverseImpl (
	const tchar_t* pName,
	CBaseTypeCoord* pCoord,
	int Flags
	)
{
	CModuleItem* pItem;

	if (!(Flags & ETraverse_NoThis))
	{
		pItem = FindItem (pName);
		if (pItem)
			return pItem;
	}

	if (!(Flags & ETraverse_NoExtensionNamespace) && m_pExtensionNamespace)
	{
		pItem = m_pExtensionNamespace->FindItem (pName);
		if (pItem)
			return pItem;
	}

	if (!(Flags & ETraverse_NoParentNamespace) && m_pParentNamespace)
	{
		pItem = m_pParentNamespace->FindItemTraverse (pName, pCoord, Flags & ~ETraverse_NoThis);
		if (pItem)
			return pItem;
	}

	return NULL;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
