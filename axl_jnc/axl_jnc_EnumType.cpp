#include "stdafx.h"
#include "axl_jnc_EnumType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CEnumType::CEnumType ()
{
	m_TypeKind = EType_Enum;
	m_Flags = ETypeFlag_Pod | ETypeFlag_Moveable;
	m_CurrentValue = 0;
}

CEnumConst*
CEnumType::CreateConst (
	const rtl::CString& Name,
	intptr_t Value
	)
{
	CEnumConst* pMember = AXL_MEM_NEW (CEnumConst);
	pMember->m_Name = Name;
	pMember->m_pParentEnumType = this;
	pMember->m_Value = Value;
	m_ConstList.InsertTail (pMember);
	
	bool Result = AddItem (pMember);
	if (!Result)
		return false;

	m_CurrentValue = Value + 1;
	return pMember;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {