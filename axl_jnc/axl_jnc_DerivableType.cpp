#include "stdafx.h"
#include "axl_jnc_DerivableType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CBaseType::CBaseType ()
{
	m_pType = NULL;
	m_Offset = 0;
	m_LlvmIndex = -1;
	m_VTableIndex = -1;
	m_Flags = 0;
}

//.............................................................................

void
CBaseTypeCoord::Init ()
{
	m_pType = NULL;
	m_Offset = 0;
	m_VTableIndex = 0;
	m_LlvmIndexArray.SetBuffer (ref::EBuf_Field, m_Buffer, sizeof (m_Buffer));
}

//.............................................................................

CBaseType*
CDerivableType::AddBaseType (CDerivableType* pType)
{
	rtl::CStringHashTableMapIteratorAT <CBaseType*> It = m_BaseTypeMap.Goto (pType->GetSignature ());
	if (It->m_Value)
	{
		err::SetFormatStringError (_T("'%s' is already a base type"), pType->GetTypeString ());
		return NULL;
	}

	CBaseType* pBaseType = AXL_MEM_NEW (CBaseType);
	pBaseType->m_pType = pType;
	m_BaseTypeList.InsertTail (pBaseType);
	It->m_Value = pBaseType;
	return pBaseType;
}

void
CDerivableType::ClearConstructedFlag ()
{
	rtl::CIteratorT <CBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
		BaseType->m_Flags &= ~EBaseTypeFlag_Constructed;
}

bool
CDerivableType::FindBaseTypeTraverseImpl (
	CType* pType,
	CBaseTypeCoord* pCoord,
	size_t Level
	)
{
	rtl::CStringHashTableMapIteratorAT <CBaseType*> It = m_BaseTypeMap.Find (pType->GetSignature ());
	if (It)
	{
		if (!pCoord)
			return true;

		CBaseType* pBaseType = It->m_Value;
		pCoord->m_pType = pBaseType->m_pType;
		pCoord->m_Offset = pBaseType->m_Offset;
		pCoord->m_LlvmIndexArray.SetCount (Level + 1);
		pCoord->m_LlvmIndexArray [Level] = pBaseType->m_LlvmIndex;
		return true;
	}

	rtl::CIteratorT <CBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		CBaseType* pBaseType = *BaseType;

		bool Result = pBaseType->m_pType->FindBaseTypeTraverseImpl (pType, pCoord, Level + 1);
		if (Result)
		{
			if (pCoord)
			{
				pCoord->m_Offset += pBaseType->m_Offset;
				pCoord->m_LlvmIndexArray [Level] = pBaseType->m_LlvmIndex;
			}

			return true;
		}
	}

	return false;
}

CModuleItem*
CDerivableType::FindItemTraverseImpl (
	const tchar_t* pName,
	CBaseTypeCoord* pCoord,
	int Flags,
	size_t Level
	)
{
	CModuleItem* pItem;

	if (!(Flags & ETraverse_NoThis))
	{
		pItem = FindItem (pName);
		if (pItem)
		{
			if (pCoord)
			{
				pCoord->m_pType = this;
				pCoord->m_LlvmIndexArray.SetCount (Level);
			}

			return pItem;
		}
	}

	if (!(Flags & ETraverse_NoExtensionNamespace) && m_pExtensionNamespace)
	{
		pItem = m_pExtensionNamespace->FindItem (pName);
		if (pItem)
		{
			if (pCoord)
			{
				pCoord->m_pType = this;
				pCoord->m_LlvmIndexArray.SetCount (Level);
			}

			return pItem;
		}
	}

	Flags &= ~ETraverse_NoThis;

	if (!(Flags & ETraverse_NoBaseType))
	{
		rtl::CIteratorT <CBaseType> BaseType = m_BaseTypeList.GetHead ();
		for (; BaseType; BaseType++)
		{
			CBaseType* pBaseType = *BaseType;

			pItem = pBaseType->m_pType->FindItemTraverseImpl (pName, pCoord, Flags, Level + 1);
			if (pItem)
			{
				if (pCoord)
				{
					pCoord->m_Offset += pBaseType->m_Offset;
					pCoord->m_LlvmIndexArray [Level] = pBaseType->m_LlvmIndex;
					pCoord->m_VTableIndex += pBaseType->m_VTableIndex;
				}

				return pItem;
			}
		}
	}

	if (!(Flags & ETraverse_NoParentNamespace) && m_pParentNamespace)
	{
		pItem = m_pParentNamespace->FindItemTraverse (pName, NULL, Flags);
		if (pItem)
		{
			if (pCoord)
				pCoord->m_pType = NULL; // not this type

			return pItem;
		}
	}

	return NULL;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
