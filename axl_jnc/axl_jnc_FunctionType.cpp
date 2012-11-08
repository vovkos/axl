#include "stdafx.h"
#include "axl_jnc_FunctionType.h"

namespace axl {
namespace jnc {

//.............................................................................

CFunctionType::CFunctionType ()
{
	m_TypeKind = EType_Function;
	m_pReturnType = NULL;
	m_Flags = 0;
}

rtl::CStringA
CFunctionType::CreateSignature (
	EType TypeKind,
	CType* pReturnType,
	CType** ppArgType,
	size_t ArgCount,
	int Flags
	)
{
	rtl::CStringA String = TypeKind == EType_Function ? "P(" : "Q(";
	
	String.Append (pReturnType->GetSignature ());
	
	for (size_t i = 0; i < ArgCount; i++)
	{
		CType* pType = ppArgType [i];
		String.Append(pType->GetSignature ());
	}

	String.Append (
		(Flags & EFunctionTypeFlag_IsVarArg) ? 
		(Flags & EFunctionTypeFlag_IsUnsafeVarArg) ? ".-)" : ".)" : ")"
		);

	return String;
}

rtl::CString
CFunctionType::CreateTypeString (
	EType TypeKind,
	CType* pReturnType,
	CType** ppArgType,
	size_t ArgCount,
	int Flags
	)
{
	rtl::CString String = TypeKind == EType_Function ? pReturnType->GetTypeString () : _T("event");

	String.Append (_T(" ("));
	
	if (ArgCount)
	{
		CType* pType = ppArgType [0];
		String.Append(pType->GetTypeString ());
	}

	for (size_t i = 1; i < ArgCount; i++)
	{
		CType* pType = ppArgType [i];
		String.Append(_T(", "));
		String.Append(pType->GetTypeString ());
	}

	if (!(Flags & EFunctionTypeFlag_IsVarArg))
		String.Append (_T(")"));
	else 
	{
		if (ArgCount)
			String.Append (_T(", "));

		String.Append ((Flags & EFunctionTypeFlag_IsUnsafeVarArg) ? _T("unsafe ...)") : _T("safe ...)"));
	}

	return String;
}

llvm::FunctionType* 
CFunctionType::GetLlvmType ()
{
	if (m_Flags & ETypeFlag_IsLlvmReady)
		return (llvm::FunctionType*) m_pLlvmType;

	size_t ArgCount = m_ArgTypeArray.GetCount ();

	rtl::CArrayT <llvm::Type*> LlvmArgTypeArray;
	LlvmArgTypeArray.SetCount (ArgCount);

	for (size_t i = 0; i < ArgCount; i++)
	{
		CType* pType = m_ArgTypeArray [i];
		llvm::Type* pLlvmType = pType->GetLlvmType ();
		LlvmArgTypeArray [i] = pLlvmType;
	}

	llvm::FunctionType* pLlvmType = llvm::FunctionType::get (
		m_pReturnType->GetLlvmType (),
		llvm::ArrayRef <llvm::Type*> (LlvmArgTypeArray, ArgCount),
		(m_Flags & EFunctionTypeFlag_IsVarArg) != 0
		);
	
	m_pLlvmType = pLlvmType;
	m_Flags |= ETypeFlag_IsLlvmReady;
	return pLlvmType;
}

//.............................................................................

bool
CFunctionTypeOverload::AddOverload (CFunctionType* pType)
{
	if (!m_pType)
	{
		m_pType = pType;
		return true;
	}

	// TODO: check no duplicate overloads

	m_OverloadArray.Append (pType);
	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
