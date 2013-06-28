#include "pch.h"
#include "axl_jnc_EnumType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CEnumType::CEnumType ()
{
	m_TypeKind = EType_Enum;
	m_Flags = ETypeFlag_Pod;
	m_pBaseType = NULL;
	m_pBaseType_i = NULL;
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
		return NULL;

	m_CurrentValue = Value + 1;
	return pMember;
}

bool
CEnumType::CalcLayout ()
{
	if (m_pBaseType_i)
		m_pBaseType = m_pBaseType_i->GetActualType ();

	if (!(m_pBaseType->GetTypeKindFlags () & ETypeKindFlag_Integer))
	{
		err::SetFormatStringError ("enum base type must be integer type");
		return NULL;
	}

	m_Size = m_pBaseType->GetSize ();
	m_AlignFactor = m_pBaseType->GetAlignFactor ();
	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {