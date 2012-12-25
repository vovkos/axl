#include "stdafx.h"
#include "axl_jnc_FunctionType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetCallingConventionString (ECallConv CallingConvention)
{
	switch (CallingConvention)
	{	
	case ECallConv_Default:
		return _T("default-calling-convention");

	case ECallConv_Cdecl:
		return _T("cdecl");

	case ECallConv_Stdcall:
		return _T("stdcall");

	default:
		return _T("undefined-calling-convention");
	}
}

//.............................................................................

CFunctionType::CFunctionType ()
{
	m_TypeKind = EType_Function;
	m_pReturnType = NULL;
	m_pFunctionPointerType = NULL;
	m_pDefCallConvFunctionType = NULL;
	m_CallingConvention = ECallConv_Default;
}

rtl::CStringA
CFunctionType::GetArgSignature ()
{
	if (m_ArgSignature.IsEmpty ())
		m_ArgSignature = CreateArgSignature (m_ArgTypeArray, m_ArgTypeArray.GetCount (), m_Flags);

	return m_ArgSignature;
}

rtl::CStringA
CFunctionType::CreateArgSignature (
	CType* const* ppArgType,
	size_t ArgCount,
	int Flags
	)
{
	rtl::CString String = "(";

	for (size_t i = 0; i < ArgCount; i++)
	{
		CType* pType = ppArgType [i];
		String.Append (pType->GetSignature ());
	}

	String.Append (
		(Flags & EFunctionTypeFlag_IsVarArg) ? 
		(Flags & EFunctionTypeFlag_IsUnsafeVarArg) ? ".-)" : ".)" : ")"
		);
	
	return String;
}

rtl::CStringA
CFunctionType::CreateSignature (
	ECallConv CallingConvention,
	CType* pReturnType,
	CType* const* ppArgType,
	size_t ArgCount,
	int Flags
	)
{

	rtl::CStringA String = "P";

	switch (CallingConvention)
	{
	case ECallConv_Cdecl:
		String += "C";
		break;

	case ECallConv_Stdcall:
		String += "S";
		break;
	}

	String.Append (pReturnType->GetSignature ());
	String.Append (CreateArgSignature (ppArgType, ArgCount, Flags));

	return String;
}

rtl::CString
CFunctionType::CreateTypeString (
	ECallConv CallingConvention,
	CType* pReturnType,
	CType* const* ppArgType,
	size_t ArgCount,
	int Flags
	)
{
	rtl::CString String = pReturnType->GetTypeString ();

	if (CallingConvention)
	{
		String += ' ';
		String += GetCallingConventionString (CallingConvention);
	}

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
	if (m_pLlvmType)
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
	return pLlvmType;
}

CFunctionPointerType* 
CFunctionType::GetFunctionPointerType ()
{
	return m_pModule->m_TypeMgr.GetFunctionPointerType (this);
}

CFunctionType*
CFunctionType::GetDefCallConvFunctionType ()
{
	if (m_pDefCallConvFunctionType)
		return m_pDefCallConvFunctionType;

	m_pDefCallConvFunctionType = m_pModule->m_TypeMgr.GetFunctionType (m_pReturnType, m_ArgTypeArray, m_Flags);
	return m_pDefCallConvFunctionType;
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

CFunctionPointerType::CFunctionPointerType ()
{
	m_TypeKind = EType_FunctionPointer;
	m_Size = sizeof (TFunctionPtr);
	m_pFunctionType = NULL;
	m_pMemberFunctionType = NULL;
	m_pPointerStructType = NULL;
}

CFunctionType* 
CFunctionPointerType::GetMemberFunctionType ()
{
	if (m_pMemberFunctionType)
		return m_pMemberFunctionType;

	rtl::CArrayT <CType*> ArgTypeArray = m_pFunctionType->GetArgTypeArray ();
	ArgTypeArray.Insert (0, m_pModule->m_TypeMgr.GetStdType (EStdType_AbstractInterfacePtr));

	m_pMemberFunctionType = m_pModule->m_TypeMgr.GetFunctionType (
		m_pFunctionType->GetCallingConvention (),
		m_pFunctionType->GetReturnType (),
		ArgTypeArray,
		m_pFunctionType->GetFlags ()
		);

	return m_pMemberFunctionType;
}

CStructType* 
CFunctionPointerType::GetPointerStructType ()
{
	if (m_pPointerStructType)
		return m_pPointerStructType;

	m_pPointerStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	m_pPointerStructType->m_Tag.Format (_T("pfn"));
	m_pPointerStructType->CreateMember (m_pFunctionType->GetPointerType (EType_Pointer_u));
	m_pPointerStructType->CreateMember (m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p));
	m_pPointerStructType->CreateMember (m_pModule->m_TypeMgr.GetStdType (EStdType_AbstractInterfacePtr));
	m_pPointerStructType->CalcLayout ();

	return m_pPointerStructType;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
