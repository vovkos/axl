#include "pch.h"
#include "axl_jnc_ImportType.h"

namespace axl {
namespace jnc {

//.............................................................................

CImportType::CImportType ()
{
	m_TypeKind = EType_Import;
	m_pAnchorNamespace = NULL;
	m_pExternType = NULL;
}

const rtl::CString&
CImportType::GetQualifiedName ()
{
	if (!m_QualifiedName.IsEmpty ())
		return m_QualifiedName;

	m_QualifiedName = m_pAnchorNamespace->CreateQualifiedName (m_Name);
	return m_QualifiedName;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {

