#include "stdafx.h"
#include "axl_jnc_Function.h"
#include "axl_jnc_Module.h"
#include "axl_jnc_ClassType.h"

namespace axl {
namespace jnc {

//.............................................................................

CFunction::CFunction ()
{
	m_ItemKind = EModuleItem_Function;
	m_FunctionKind = EFunction_Undefined;
	m_pOverload = NULL;
	m_VTableIndex = -1;
	m_pOriginClassType = NULL;
	m_pType = NULL;
	m_pClosureType = NULL;
	m_pProperty = NULL;
	m_pBlock = NULL;
	m_pScope = NULL;
	m_pLlvmFunction = NULL;
	m_pfn = NULL;
}

rtl::CString
CFunction::CreateArgString ()
{
	if (m_ArgList.IsEmpty ())
	return 
		(m_pType->GetFlags () & EFunctionTypeFlag_IsVarArg) ? 
		(m_pType->GetFlags () & EFunctionTypeFlag_IsUnsafeVarArg) ? 
		_T("(unsafe ...)") : _T("(safe ...)") : _T("()");

	rtl::CIteratorT <jnc::CFunctionFormalArg> Arg = m_ArgList.GetHead ();

	rtl::CString String;
	String.Format (
		_T("(%s %s"), 
		Arg->GetType ()->GetTypeString (),
		Arg->GetName ()
		);

	for (Arg++; Arg; Arg++)
		String.AppendFormat (
			_T(", %s %s"), 
			Arg->GetType ()->GetTypeString (),
			Arg->GetName ()
			);

	if (!(m_pType->GetFlags () & EFunctionTypeFlag_IsVarArg))
		String.Append (_T(")"));
	else if (m_pType->GetFlags () & EFunctionTypeFlag_IsUnsafeVarArg)
		String.Append (_T(", unsafe ...)"));
	else
		String.Append (_T(", safe ...)"));

	return String;
}

llvm::Function* 
CFunction::GetLlvmFunction ()
{
	if (m_pLlvmFunction)
		return m_pLlvmFunction;

	llvm::FunctionType* pLlvmType = m_pType->GetLlvmType ();
	m_pLlvmFunction = llvm::Function::Create (
		pLlvmType, 
		llvm::Function::ExternalLinkage, 
		(const tchar_t*) m_Tag, 
		m_pModule->m_pLlvmModule
		);

	return m_pLlvmFunction;
}

CGlobalFunction* 
CFunction::GetGlobalFunction ()
{
	ASSERT (m_FunctionKind == EFunction_Global);
	return (CGlobalFunction*) m_pOverload;
}

CClassMethodMember* 
CFunction::GetClassMethodMember ()
{
	ASSERT (m_FunctionKind == EFunction_Method);
	return (CClassMethodMember*) m_pOverload;
}

//.............................................................................

CFunction*
CFunctionOverload::FindOverload (rtl::CBoxListT <CValue>* pArgList) const
{
	// TODO: implement 

	return m_pFunction; 
}

bool
CFunctionOverload::AddOverload (CFunction* pFunction)
{
	pFunction->m_pOverload = this;

	if (!m_pFunction)
	{
		m_pFunction = pFunction;
		return true;
	}

	m_OverloadArray.Append (pFunction);

	// TODO: check that no overloads overlap

	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
