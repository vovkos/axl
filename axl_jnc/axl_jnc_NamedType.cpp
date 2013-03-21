#include "stdafx.h"
#include "axl_jnc_NamedType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CNamedType::CNamedType ()
{
	m_NamespaceKind = ENamespace_Type;
	m_pItemDecl = this;
	m_StaticPackFactor = 8;
	m_pStaticStructType = NULL;
	m_pStaticVariable = NULL;
}

CStructType*
CNamedType::CreateStaticStructType ()
{
	CStructType* pStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType (m_StaticPackFactor);
	pStructType->m_StorageKind = EStorage_Static;
	pStructType->m_pParentNamespace = this;
	pStructType->m_Tag.Format (_T("%s.static_struct"), m_Tag);
	m_pStaticStructType = pStructType;
	return pStructType;
}

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

bool
CNamedType::CalcLayout ()
{
	if (m_Flags & ETypeFlag_LayoutReady)
		return true;

	bool Result = PreCalcLayout ();
	if (!Result)
		return false;

	if (m_pExtensionNamespace)
		ApplyExtensionNamespace ();

	if (m_pStaticStructType)
	{
		Result = CreateStaticVariable ();
		if (!Result)
			return false;
	}

	PostCalcLayout ();
	return true;
}

void
CNamedType::ApplyExtensionNamespace ()
{
	ASSERT (m_pExtensionNamespace);

	size_t Count = m_pExtensionNamespace->GetItemCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CModuleItem* pItem = m_pExtensionNamespace->GetItem (i);
		EModuleItem ItemKind = pItem->GetItemKind ();

		switch (ItemKind)
		{
		case EModuleItem_Function:
			((CFunction*) pItem)->ConvertToMemberMethod (this);
			break;

		case EModuleItem_Property:
			((CProperty*) pItem)->ConvertToMemberProperty (this);
			break;
		}
	}
}

bool
CNamedType::CreateStaticVariable ()
{
	ASSERT (m_pStaticStructType);

	bool Result = m_pStaticStructType->CalcLayout ();
	if (!Result)
		return false;

	m_pStaticVariable = m_pModule->m_VariableMgr.CreateVariable (
		EStorage_Static,
		_T("static_field"),
		m_Tag + _T(".static_field"), 
		m_pStaticStructType
		);
	
	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
