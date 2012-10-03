#include "stdafx.h"
#include "axl_jnc_AttributeMgr.h"

namespace axl {
namespace jnc {

//.............................................................................

void
CAttributeMgr::AssignAttributeSet (CModuleItem* pItem)
{
	if (!m_pCurrentAttributeSet)
		return;

	pItem->m_pAttributeSet = m_pCurrentAttributeSet;
	m_pCurrentAttributeSet->m_pParentItem = pItem;
	m_pCurrentAttributeSet = NULL;
}

CAttributeSet*
CAttributeMgr::CreateAttributeSet ()
{
	if (m_pCurrentAttributeSet && m_pCurrentAttributeSet->GetParentItem ())
	{
		err::SetFormatStringError (_T("orphaned attribute set"));
		return NULL;
	}

	CAttributeSet* pAttributeSet = AXL_MEM_NEW (CAttributeSet);
	m_AttributeSetList.InsertTail (pAttributeSet);
	m_pCurrentAttributeSet = pAttributeSet;
	return pAttributeSet;
}

CAttribute*
CAttributeMgr::CreateAttribute (
	const rtl::CString& Name,
	CValue* pValue
	)
{
	if (!m_pCurrentAttributeSet)
		CreateAttributeSet ();

	return m_pCurrentAttributeSet->CreateAttribute (Name, pValue);
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
