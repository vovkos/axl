#include "pch.h"
#include "axl_jnc_ImportType.h"

namespace axl {
namespace jnc {

//.............................................................................

CImportType::CImportType ()
{
	m_TypeKind = EType_Import;
	m_ImportTypeKind = EImportType_Undefined;
	m_pActualType = NULL;
}

bool
CImportType::CalcLayout ()
{
	ASSERT (m_pActualType);

	if (m_Flags & ETypeFlag_LayoutReady)
		return true;

	bool Result = PreCalcLayout ();
	if (!Result)
		return false;

	Result = m_pActualType->CalcLayout ();
	if (!Result)
		return false;

	PostCalcLayout ();
	return true;
}

void
CImportType::PrepareLlvmType ()
{
	ASSERT (m_pActualType && m_pActualType->GetTypeKind () != EType_Import);
	m_pLlvmType = m_pActualType->GetLlvmType ();
}

//.............................................................................

rtl::CString
CPrimaryImportType::GetQualifiedName ()
{
	if (!m_QualifiedName.IsEmpty ())
		return m_QualifiedName;

	m_QualifiedName = m_pAnchorNamespace->CreateQualifiedName (m_Name);
	return m_QualifiedName;
}

void
CPrimaryImportType::PrepareTypeString ()
{
	if (m_pActualType)
		m_TypeString = m_pActualType->GetTypeString ();
	else
		m_TypeString.Format ("import %s", GetQualifiedName ());
}

//.............................................................................

rtl::CString
CSecondaryImportType::CreateSignature (
	EImportType ImportTypeKind,
	CPrimaryImportType* pPrimaryImportType,
	uint_t TypeModifiers
	)
{
	rtl::CString Signature = ImportTypeKind == EImportType_Pointer ? "ZP" : "ZD";	
	Signature += pPrimaryImportType->GetQualifiedName ();

	if (TypeModifiers)
		Signature.AppendFormat (":%d", TypeModifiers);

	return Signature;
}

void
CSecondaryImportType::PrepareTypeString ()
{
	ASSERT (m_pPrimaryImportType);

	if (m_pActualType)
	{
		m_TypeString = m_pActualType->GetTypeString ();
		return;
	}

	m_TypeString = m_ImportTypeKind == EImportType_Pointer ? "import_p " : "import_d ";
	
	if (m_TypeModifiers & ETypeModifierMask_ClassPtr_p)
	{
		m_TypeString += GetTypeModifierString (m_TypeModifiers & ETypeModifierMask_ClassPtr_p);
		m_TypeString += ' ';
	}

	m_TypeString += m_pPrimaryImportType->GetQualifiedName ();

	if (m_TypeModifiers & ~ETypeModifierMask_ClassPtr_p)
	{
		m_TypeString += ' ';
		m_TypeString += GetTypeModifierString (m_TypeModifiers & ~ETypeModifierMask_ClassPtr_p);
	}

	if (m_ImportTypeKind == EImportType_Pointer)
		m_TypeString += '*';
}

//.............................................................................

} // namespace jnc {
} // namespace axl {

