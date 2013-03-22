#include "stdafx.h"
#include "axl_jnc_PropertyTemplate.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CPropertyTemplate::CPropertyTemplate ()
{
	m_ItemKind = EModuleItem_PropertyTemplate;
	m_NamespaceKind = ENamespace_PropertyTemplate;
	m_pItemDecl = this;
	m_pGetterType = NULL;
	m_pAuPropValueType = NULL;
	m_TypeModifiers = 0;
}

bool
CPropertyTemplate::AddMethod (
	EFunction FunctionKind,
	CFunctionType* pFunctionType
	)
{
	if (FunctionKind != EFunction_Getter && FunctionKind != EFunction_Setter)
	{
		err::SetFormatStringError (_T("property templates can only have accessors"));
		return false;
	}

	bool Result = m_Verifier.AddMethod (FunctionKind, pFunctionType);
	if (!Result)
		return false;

	if (FunctionKind == EFunction_Getter)
	{
		m_pGetterType = pFunctionType;
		return true;
	}

	Result = m_SetterType.AddOverload (pFunctionType);
	if (!Result)
		return false;

	return true;
}

CPropertyType*
CPropertyTemplate::CalcType ()
{
	if (!m_pGetterType)
	{
		err::SetFormatStringError (_T("incomplete property template: no 'get' method / 'propvalue' field"));
		return NULL;
	}

	int Flags = GetPropertyTypeFlagsFromModifiers (m_TypeModifiers);
	return m_pModule->m_TypeMgr.GetPropertyType (m_pGetterType, m_SetterType, Flags);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
