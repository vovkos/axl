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
	m_pShortType = this;
	m_pStdObjectMemberMethodType = NULL;
	m_pFunctionPtrTypeTuple = NULL;
}

CNamedType*
CFunctionType::GetThisTargetType ()
{
	CType* pThisArgType = GetThisArgType ();
	if (!pThisArgType)
		return NULL;

	EType ThisArgTypeKind = pThisArgType->GetTypeKind ();
	switch (ThisArgTypeKind)
	{
	case EType_ClassPtr:
		return ((CClassPtrType*) pThisArgType)->GetTargetType ();

	case EType_DataPtr:
		return (CNamedType*) ((CDataPtrType*) pThisArgType)->GetTargetType ();

	default:
		ASSERT (false);
		return NULL;
	}
}

rtl::CStringA
CFunctionType::GetArgSignature ()
{
	if (m_ArgSignature.IsEmpty ())
		m_ArgSignature = CreateArgSignature ();

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

CMulticastType* 
CFunctionType::GetMulticastType ()
{
	return m_pModule->m_TypeMgr.GetMulticastType (this);
}

CFunctionType*
CFunctionType::GetMemberMethodType (
	CNamedType* pParentType, 
	int ThisArgTypeFlags
	)
{
	return m_pModule->m_TypeMgr.GetMemberMethodType (pParentType, this, ThisArgTypeFlags);
}

CFunctionType*
CFunctionType::GetStdObjectMemberMethodType ()
{
	return m_pModule->m_TypeMgr.GetStdObjectMemberMethodType (this);
}

CFunction*
CFunctionType::GetAbstractFunction ()
{
	if (m_pAbstractFunction)
		return m_pAbstractFunction;

	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateInternalFunction (_T("abtract_method"), this);

	m_pModule->m_FunctionMgr.InternalPrologue (pFunction);
	m_pModule->m_LlvmBuilder.RuntimeError (ERuntimeError_AbstractFunction);
	m_pModule->m_FunctionMgr.InternalEpilogue ();

	m_pAbstractFunction = pFunction;
	return pFunction;
}

rtl::CStringA
CFunctionType::CreateArgSignature (
	CType* const* pArgTypeArray,
	size_t ArgCount,
	int Flags
	)
{
	rtl::CStringA String = "(";

	for (size_t i = 0; i < ArgCount; i++)
	{
		CType* pType = pArgTypeArray [i];
		String.Append (pType->GetSignature ());
		String.Append (",");
	}

	String.Append (
		(Flags & EFunctionTypeFlag_VarArg) ? 
		(Flags & EFunctionTypeFlag_UnsafeVarArg) ? ".-)" : ".)" : ")"
		);
	
	return String;
}

rtl::CStringA
CFunctionType::CreateArgSignature (
	CFunctionArg* const* pArgArray,
	size_t ArgCount,
	int Flags
	)
{
	rtl::CStringA String = "(";

	for (size_t i = 0; i < ArgCount; i++)
	{
		CFunctionArg* pArg = pArgArray [i];

		if (pArg->GetPtrTypeFlags () & EPtrTypeFlag_This)
			String.Append ("+");

		String.Append (pArg->GetType ()->GetSignature ());
		String.Append (",");
	}

	String.Append (
		(Flags & EFunctionTypeFlag_VarArg) ? 
		(Flags & EFunctionTypeFlag_UnsafeVarArg) ? ".-)" : ".)" : ")"
		);
	
	return String;
}

rtl::CString 
CFunctionType::CreateCallConvSignature (ECallConv CallConv)
{
	rtl::CStringA String;

	switch (CallConv)
	{
	case ECallConv_Cdecl:
		String = 'C';
		break;

	case ECallConv_Stdcall:
		String = 'S';
		break;

	default:
		ASSERT (false);
	}

	return String;
}

rtl::CStringA
CFunctionType::CreateSignature (
	ECallConv CallConv,
	CType* pReturnType,
	CType* const* pArgTypeArray,
	size_t ArgCount,
	int Flags
	)
{
	rtl::CStringA String = "F";
	String.Append (CreateCallConvSignature (CallConv));
	String.Append (pReturnType->GetSignature ());
	String.Append (CreateArgSignature (pArgTypeArray, ArgCount, Flags));
	return String;
}

rtl::CStringA
CFunctionType::CreateSignature (
	ECallConv CallConv,
	CType* pReturnType,
	CFunctionArg* const* pArgArray,
	size_t ArgCount,
	int Flags
	)
{
	rtl::CStringA String = "F";
	String.Append (CreateCallConvSignature (CallConv));
	String.Append (pReturnType->GetSignature ());
	String.Append (CreateArgSignature (pArgArray, ArgCount, Flags));
	return String;
}

rtl::CString
CFunctionType::GetArgString ()
{
	if (!m_ArgString.IsEmpty ())
		return m_ArgString;

	bool IsUserType = (m_Flags & EFunctionTypeFlag_User) != 0;

	m_ArgString = _T("(");

	if (!m_ArgArray.IsEmpty ())
	{
		CFunctionArg* pArg = m_ArgArray [0];
		m_ArgString.AppendFormat (_T("%s"), pArg->GetType ()->GetTypeString ());

		if (pArg->GetPtrTypeFlags () & EPtrTypeFlag_This)
		{
			m_ArgString.Append (_T(" this"));
		}
		else if (IsUserType)
		{
				if (!pArg->GetName ().IsEmpty ())
					m_ArgString.AppendFormat (_T(" %s"), pArg->GetName ());

				if (!pArg->GetInitializer ().IsEmpty ())
					m_ArgString.AppendFormat (_T(" = %s"), pArg->GetInitializerString ());
		}

		size_t ArgCount = m_ArgArray.GetCount ();
		for (size_t i = 1; i < ArgCount; i++)
		{
			pArg = m_ArgArray [i];

			m_ArgString.AppendFormat (_T(", %s"), pArg->GetType ()->GetTypeString ());

			if (IsUserType)
			{
				if (!pArg->GetName ().IsEmpty ())
					m_ArgString.AppendFormat (_T(" %s"), pArg->GetName ());

				if (!pArg->GetInitializer ().IsEmpty ())
					m_ArgString.AppendFormat (_T(" = %s"), pArg->GetInitializerString ());
			}
		}

		if (m_Flags & EFunctionTypeFlag_VarArg)
			m_ArgString.Append (_T(", "));
	}

	if (!(m_Flags & EFunctionTypeFlag_VarArg))
		m_ArgString.Append (_T(")"));
	else if (m_Flags & EFunctionTypeFlag_UnsafeVarArg)
		m_ArgString.Append (_T("unsafe ...)"));
	else
		m_ArgString.Append (_T("...)"));

	return m_ArgString;
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
	m_TypeString += GetArgString ();	
}

void
CFunctionType::PrepareLlvmType ()
{
	size_t ArgCount = m_ArgArray.GetCount ();

	char Buffer [256];
	rtl::CArrayT <llvm::Type*> LlvmArgTypeArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	LlvmArgTypeArray.SetCount (ArgCount);

	for (size_t i = 0; i < ArgCount; i++)
		LlvmArgTypeArray [i] = m_ArgArray [i]->GetType ()->GetLlvmType ();

	m_pLlvmType = llvm::FunctionType::get (
		m_pReturnType->GetLlvmType (),
		llvm::ArrayRef <llvm::Type*> (LlvmArgTypeArray, ArgCount),
		(m_Flags & EFunctionTypeFlag_VarArg) != 0
		);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
