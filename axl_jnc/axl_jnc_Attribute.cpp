#include "stdafx.h"
#include "axl_jnc_Attribute.h"

namespace axl {
namespace jnc {

//.............................................................................

CAttribute*
CAttributeSet::CreateAttribute (
	const rtl::CString& Name,
	CValue* pValue
	)
{
	rtl::CHashTableMapIteratorT <const tchar_t*, CAttribute*> It = m_AttributeMap.Goto (Name);
	if (It->m_Value)
	{
		err::SetFormatStringError (_T("redefinition of attribute '%s'"));
		return NULL;
	}

	CAttribute* pAttribute = AXL_MEM_NEW (CAttribute);
	pAttribute->m_Name = Name;
	pAttribute->m_pValue = pValue;
	m_AttributeList.InsertTail (pAttribute);
	It->m_Value = pAttribute;
	return pAttribute;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
