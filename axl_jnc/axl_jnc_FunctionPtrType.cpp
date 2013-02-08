#include "stdafx.h"
#include "axl_jnc_FunctionPtrType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetFunctionPtrTypeKindString (EFunctionPtrType PtrTypeKind)
{
	switch (PtrTypeKind)
	{
	case EFunctionPtrType_Normal:
		return _T("closure");

	case EFunctionPtrType_Weak:
		return _T("weak");

	case EFunctionPtrType_Thin:
		return _T("thin");

	case EFunctionPtrType_Unsafe:
		return _T("unsafe");

	default:
		return _T("undefined-function-ptr-kind");
	}
}

//.............................................................................

CFunctionPtrType::CFunctionPtrType ()
{
	m_TypeKind = EType_FunctionPtr;
	m_PtrTypeKind = EFunctionPtrType_Normal;
	m_Size = sizeof (TFunctionPtr);
	m_pTargetType = NULL;
	m_pEventType = NULL;
}

CEventType* 
CFunctionPtrType::GetEventType ()
{
	return m_pModule->m_TypeMgr.GetEventType (this);
}

CStructType* 
CFunctionPtrType::GetClosureFunctionPtrStructType ()
{
	return m_pModule->m_TypeMgr.GetClosureFunctionPtrStructType (m_pTargetType);
}

CStructType* 
CFunctionPtrType::GetWeakClosureFunctionPtrStructType ()
{
	return m_pModule->m_TypeMgr.GetWeakClosureFunctionPtrStructType (m_pTargetType);
}

rtl::CStringA
CFunctionPtrType::CreateSignature (
	CFunctionType* pFunctionType,
	EType TypeKind,
	EFunctionPtrType PtrTypeKind,
	int Flags
	)
{
	rtl::CStringA Signature = TypeKind == EType_FunctionRef ? 'R' : 'P';

	switch (PtrTypeKind)
	{
	case EFunctionPtrType_Thin:
		Signature += 't';
		break;

	case EFunctionPtrType_Weak:
		Signature += 'w';
		break;

	case EFunctionPtrType_Unsafe:
		Signature += 'u';
		break;
	}

	if (Flags & EPtrTypeFlag_NoNull)
		Signature += 'n';

	Signature += pFunctionType->GetSignature ();
	return Signature;
}

rtl::CString
CFunctionPtrType::GetTypeModifierString ()
{
	if (!m_TypeModifierString.IsEmpty ())
		return m_TypeModifierString;

	if (m_PtrTypeKind != EClassPtrType_Normal)
	{
		m_TypeString += _T(' ');
		m_TypeString += GetFunctionPtrTypeKindString (m_PtrTypeKind);
	}

	if (m_Flags & EPtrTypeFlag_NoNull)
		m_TypeModifierString += _T(" nonull");

	ECallConv CallConv = m_pTargetType->GetCallConv ();
	if (CallConv)
		m_TypeString.AppendFormat (_T(" %s"), GetCallConvString (CallConv));

	return m_TypeModifierString;
}

void
CFunctionPtrType::PrepareTypeString ()
{
	m_TypeString = m_pTargetType->GetReturnType ()->GetTypeString ();
	m_TypeString += GetTypeModifierString ();
	m_TypeString += m_TypeKind == EType_FunctionRef ? " function& " : " function* ";
	m_TypeString += m_pTargetType->GetArgTypeString ();
}

void
CFunctionPtrType::PrepareLlvmType ()
{
	switch (m_PtrTypeKind)
	{
	case EFunctionPtrType_Normal:
		m_pLlvmType = GetClosureFunctionPtrStructType ()->GetLlvmType ();
		break;

	case EFunctionPtrType_Thin:
	case EFunctionPtrType_Unsafe:
		m_pLlvmType = llvm::PointerType::get (m_pTargetType->GetLlvmType (), 0);
		break;

	case EFunctionPtrType_Weak:
		m_pLlvmType = GetWeakClosureFunctionPtrStructType ()->GetLlvmType ();
		break;

	default:
		ASSERT (false);
	}
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
