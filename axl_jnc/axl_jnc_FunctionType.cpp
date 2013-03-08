#include "stdafx.h"
#include "axl_jnc_FunctionType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetCallConvString (ECallConv CallConv)
{
	static const tchar_t* StringTable [] = 
	{
		_T("cdecl"),    // ECallConv_Cdecl,
		_T("stdcall"),  // ECallConv_Stdcall,
	};

	return (size_t) CallConv < countof (StringTable) ? 
		StringTable [CallConv] : 
		_T("undefined-calling-convention");
}

//.............................................................................

CFunctionType::CFunctionType ()
{
	m_TypeKind = EType_Function;
	m_pReturnType = NULL;
	m_CallConv = ECallConv_Default;
	m_pStdObjectMethodMemberType = NULL;
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
CFunctionType::GetShortType ()
{
	return m_pModule->m_TypeMgr.GetShortFunctionType (this);
}

CMulticastType* 
CFunctionType::GetMulticastType ()
{
	return m_pModule->m_TypeMgr.GetMulticastType (this);
}

CFunctionType*
CFunctionType::GetMethodMemberType (
	CClassType* pClassType, 
	int ThisArgTypeFlags
	)
{
	return m_pModule->m_TypeMgr.GetMethodMemberType (pClassType, this, ThisArgTypeFlags);
}

CFunctionType*
CFunctionType::GetStdObjectMethodMemberType ()
{
	return m_pModule->m_TypeMgr.GetStdObjectMethodMemberType (this);
}

CFunction*
CFunctionType::GetAbstractFunction ()
{
	if (m_pAbstractFunction)
		return m_pAbstractFunction;

	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction (_T("abtract_method"), this);

	m_pModule->m_FunctionMgr.InternalPrologue (pFunction, NULL, 0);
	m_pModule->m_LlvmBuilder.RuntimeError (ERuntimeError_AbstractFunction);
	m_pModule->m_FunctionMgr.InternalEpilogue ();

	m_pAbstractFunction = pFunction;
	return pFunction;
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

		if (m_Flags & EFunctionTypeFlag_UnsafeVarArg)
			m_ArgTypeString.Append (_T("unsafe "));

		m_ArgTypeString.Append (_T("...)"));
	}

	return m_ArgTypeString;
}

void
CFunctionType::PrepareTypeString ()
{
	m_TypeString = m_pReturnType->GetTypeString ();

	if (m_CallConv != ECallConv_Default)
	{
		m_TypeString += ' ';
		m_TypeString += GetCallConvString (m_CallConv);
	}

	m_TypeString += ' ';
	m_TypeString += GetArgTypeString ();	
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
