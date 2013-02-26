#include "stdafx.h"
#include "axl_jnc_VariableMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CVariableMgr::CVariableMgr ()
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);
	m_pScopeLevelVariable = NULL;
}

void
CVariableMgr::Clear ()
{
	m_LocalVariableList.Clear ();
	m_GlobalVariableList.Clear ();
	m_pScopeLevelVariable = NULL;
}

CVariable*
CVariableMgr::CreateVariable (
	EVariable VariableKind,
	const rtl::CString& Name,
	const rtl::CString& QualifiedName,
	CType* pType,
	int PtrTypeFlags
	)
{
	if (pType->GetTypeKind () == EType_Class)
		pType = ((CClassType*) pType)->GetClassPtrType ();

	CVariable* pVariable = AXL_MEM_NEW (CVariable);
	pVariable->m_pModule = m_pModule;
	pVariable->m_VariableKind = VariableKind;
	pVariable->m_Name = Name;
	pVariable->m_QualifiedName = QualifiedName;
	pVariable->m_Tag = QualifiedName;
	pVariable->m_pType = pType;
	pVariable->m_PtrTypeFlags = PtrTypeFlags;

	if (VariableKind == EVariable_Local)
	{
		ASSERT (m_pModule->m_FunctionMgr.GetCurrentFunction ());
		
		CValue PtrValue;
		m_pModule->m_LlvmBuilder.CreateAlloca (pType, Name, NULL, &PtrValue);

		pVariable->m_StorageKind = EStorage_Local;
		pVariable->m_pScope = m_pModule->m_NamespaceMgr.GetCurrentScope ();
		pVariable->m_pLlvmValue = PtrValue.GetLlvmValue ();
		m_LocalVariableList.InsertTail (pVariable);
	}
	else
	{
		ASSERT (VariableKind == EVariable_Global);

		pVariable->m_StorageKind = EStorage_Static;
		m_GlobalVariableList.InsertTail (pVariable);
	}

	return pVariable;
}

bool
CVariableMgr::AllocateGlobalVariables ()
{
	rtl::CIteratorT <CVariable> Variable = m_GlobalVariableList.GetHead ();
	for (; Variable; Variable++)
	{
		CVariable* pVariable = *Variable;

		if (!pVariable->m_pLlvmValue)
			pVariable->m_pLlvmValue = new llvm::GlobalVariable (
				*m_pModule->m_pLlvmModule,
				pVariable->m_pType->GetLlvmType (),
				false,
				llvm::GlobalVariable::ExternalLinkage,
				(llvm::Constant*) pVariable->m_pType->GetZeroValue ().GetLlvmValue (),
				(const tchar_t*) pVariable->GetQualifiedName ()
				);
	}

	return true;
}

CVariable*
CVariableMgr::GetScopeLevelVariable ()
{
	if (m_pScopeLevelVariable)
		return m_pScopeLevelVariable;

	CType* pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT);

	m_pScopeLevelVariable = CreateVariable (EVariable_Global, _T("ScopeLevel"), _T("jnc.ScopeLevel"), pType);

	m_pScopeLevelVariable->m_pLlvmValue = new llvm::GlobalVariable (
		*m_pModule->m_pLlvmModule,
		pType->GetLlvmType (),
		false,
		llvm::GlobalVariable::ExternalLinkage,
		(llvm::Constant*) pType->GetZeroValue ().GetLlvmValue (),
		_T("jnc.ScopeLevel"),
		NULL,
		false // true // thread local -- currently JIT produces an error
		);
	
	return m_pScopeLevelVariable;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
