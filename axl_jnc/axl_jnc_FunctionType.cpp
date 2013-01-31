#include "stdafx.h"
#include "axl_jnc_FunctionType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetCallConvString (ECallConv CallConv)
{
	switch (CallConv)
	{	
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
	m_CallConv = ECallConv_Default;
	m_pAbstractMethodMemberType = NULL;
	m_pFunctionPtrTypeTuple = NULL;
}

rtl::CStringA
CFunctionType::GetArgSignature ()
{
	if (m_ArgSignature.IsEmpty ())
		m_ArgSignature = CreateArgSignature (m_ArgTypeArray, m_ArgTypeArray.GetCount (), m_Flags);

	return m_ArgSignature;
}

CFunctionPtrType* 
CFunctionType::GetFunctionPtrType (
	EType TypeKind,
	EFunctionPtrType PtrTypeKind,
	int Flags
	)
{
	return m_pModule->m_TypeMgr.GetFunctionPtrType (this, TypeKind, PtrTypeKind, Flags);
}

CFunctionType*
CFunctionType::GetAbstractMethodMemberType ()
{
	return m_pModule->m_TypeMgr.GetAbstractMethodMemberType (this);
}

CEventType* 
CFunctionType::GetEventType ()
{
	return m_pModule->m_TypeMgr.GetEventType (this);
}

rtl::CStringA
CFunctionType::CreateArgSignature (
	CType* const* ppArgType,
	size_t ArgCount,
	int Flags
	)
{
	rtl::CStringA String = "(";

	for (size_t i = 0; i < ArgCount; i++)
	{
		CType* pType = ppArgType [i];
		String.Append (pType->GetSignature ());
	}

	String.Append (
		(Flags & EFunctionTypeFlag_VarArg) ? 
		(Flags & EFunctionTypeFlag_UnsafeVarArg) ? ".-)" : ".)" : ")"
		);
	
	return String;
}

rtl::CStringA
CFunctionType::CreateSignature (
	ECallConv CallConv,
	CType* pReturnType,
	CType* const* ppArgType,
	size_t ArgCount,
	int Flags
	)
{
	rtl::CStringA String = "F";

	switch (CallConv)
	{
	case ECallConv_Cdecl:
		String += 'C';
		break;

	case ECallConv_Stdcall:
		String += 'S';
		break;

	default:
		ASSERT (false);
	}

	String.Append (pReturnType->GetSignature ());
	String.Append (CreateArgSignature (ppArgType, ArgCount, Flags));
	return String;
}

rtl::CString
CFunctionType::GetArgTypeString ()
{
	if (!m_ArgTypeString.IsEmpty ())
		return m_ArgTypeString;

	m_ArgTypeString = _T("(");

	size_t ArgCount = m_ArgTypeArray.GetCount ();
	if (ArgCount)
	{
		CType* pType = m_ArgTypeArray [0];
		m_ArgTypeString.Append(pType->GetTypeString ());
	}

	for (size_t i = 1; i < ArgCount; i++)
	{
		CType* pType = m_ArgTypeArray [i];
		m_ArgTypeString.Append(_T(", "));
		m_ArgTypeString.Append(pType->GetTypeString ());
	}

	if (!(m_Flags & EFunctionTypeFlag_VarArg))
		m_ArgTypeString.Append (_T(")"));
	else 
	{
		if (ArgCount)
			m_ArgTypeString.Append (_T(", "));

		m_ArgTypeString.Append ((m_Flags & EFunctionTypeFlag_UnsafeVarArg) ? _T("unsafe ...)") : _T("safe ...)"));
	}

	return m_ArgTypeString;
}

void
CFunctionType::PrepareTypeString ()
{
	rtl::CString String = m_pReturnType->GetTypeString ();

	if (m_CallConv != ECallConv_Default)
	{
		String += ' ';
		String += GetCallConvString (m_CallConv);
	}

	String += ' ';
	String += GetArgTypeString ();
}

void
CFunctionType::PrepareLlvmType ()
{
	size_t ArgCount = m_ArgTypeArray.GetCount ();

	char Buffer [256];
	rtl::CArrayT <llvm::Type*> LlvmArgTypeArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	LlvmArgTypeArray.SetCount (ArgCount);

	for (size_t i = 0; i < ArgCount; i++)
	{
		CType* pType = m_ArgTypeArray [i];
		llvm::Type* pLlvmType = pType->GetLlvmType ();
		LlvmArgTypeArray [i] = pLlvmType;
	}

	m_pLlvmType = llvm::FunctionType::get (
		m_pReturnType->GetLlvmType (),
		llvm::ArrayRef <llvm::Type*> (LlvmArgTypeArray, ArgCount),
		(m_Flags & EFunctionTypeFlag_VarArg) != 0
		);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
