#include "pch.h"
#include "axl_jnc_BitFieldType.h"

namespace axl {
namespace jnc {

//.............................................................................

CBitFieldType::CBitFieldType ()
{
	m_TypeKind = EType_BitField;
	m_Flags = ETypeFlag_Pod | ETypeFlag_Moveable;
	m_pBaseType = NULL;
	m_BitOffset = 0;
	m_BitCount = 0;
}

void
CBitFieldType::PrepareTypeString ()
{
	m_TypeString.Format (
		"%s:%d:%d",
		m_pBaseType->GetTypeString ().cc (), // thanks a lot gcc
		m_BitOffset,
		m_BitOffset + m_BitCount
		); 
}

//.............................................................................

} // namespace jnc {
} // namespace axl {

