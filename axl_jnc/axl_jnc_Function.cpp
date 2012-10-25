#include "stdafx.h"
#include "axl_jnc_Function.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CFunction::CFunction ()
{
	m_ItemKind = EModuleItem_Function;
	m_pType = NULL;
	m_pBlock = NULL;
	m_pScope = NULL;
	m_pLlvmFunction = NULL;
}

rtl::CString
CFunction::CreateArgString ()
{
	if (m_ArgList.IsEmpty ())
		return m_pType->IsVarArg () ? _T("(...)") : _T("()");

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

	if (!m_pType->IsVarArg ())
		String.Append (_T(")"));
	else 
		String.Append (_T(", ...)"));

	return String;
}

llvm::Function* 
CFunction::GetLlvmFunction ()
{
	if (m_pLlvmFunction)
		return m_pLlvmFunction;

	rtl::CString Name;
	Name.Format (_T("function_%x"), this);

	llvm::FunctionType* pLlvmType = m_pType->GetLlvmType ();
	m_pLlvmFunction = llvm::Function::Create (
		pLlvmType, 
		llvm::Function::ExternalLinkage, 
		(const tchar_t*) Name, 
		m_pModule->GetLlvmModule ()
		);

	return m_pLlvmFunction;
}

//.............................................................................

CFunction*
CFunctionOverload::FindOverload (
	CType** ppArgType,
	size_t ArgCount
	) const
{
	// TODO: implement 

	return m_pFunction; 
}

bool
CFunctionOverload::AddOverload (CFunction* pFunction)
{
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
