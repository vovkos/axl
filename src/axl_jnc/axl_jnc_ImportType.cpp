#include "pch.h"
#include "axl_jnc_ImportType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

rtl::CString
CNamedImportType::GetQualifiedName ()
{
	if (!m_QualifiedName.IsEmpty ())
		return m_QualifiedName;

	m_QualifiedName = m_pAnchorNamespace->CreateQualifiedName (m_Name);
	return m_QualifiedName;
}

CImportPtrType* 
CNamedImportType::GetImportPtrType (
	uint_t TypeModifiers,
	uint_t Flags
	)
{
	return m_pModule->m_TypeMgr.GetImportPtrType (this, TypeModifiers, Flags);
}

//.............................................................................

CImportPtrType::CImportPtrType ()
{
	m_TypeKind = EType_ImportPtr;
	m_pTargetType = NULL;
	m_TypeModifiers = 0;
}

void
CImportPtrType::PrepareTypeString ()
{
	ASSERT (m_pTargetType);

	if (m_pActualType)
	{
		m_TypeString = m_pActualType->GetTypeString ();
		return;
	}

	m_TypeString = "import ";
	
	if (m_TypeModifiers)
	{
		m_TypeString += GetTypeModifierString (m_TypeModifiers);
		m_TypeString += ' ';
	}

	m_TypeString += m_pTargetType->GetQualifiedName ();
	m_TypeString += '*';
}

//.............................................................................

} // namespace jnc {
} // namespace axl {

