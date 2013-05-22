#include "pch.h"
#include "axl_jnc_FunctionPtrType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const char*
GetFunctionPtrTypeKindString (EFunctionPtrType PtrTypeKind)
{
	static const char* StringTable [EFunctionPtrType__Count] = 
	{
		"closure",  // EFunctionPtrType_Normal = 0,
		"weak",     // EFunctionPtrType_Weak,
		"thin",     // EFunctionPtrType_Thin,
		"unsafe",   // EFunctionPtrType_Unsafe,
	};

	return (size_t) PtrTypeKind < EFunctionPtrType__Count ? 
		StringTable [PtrTypeKind] : 
		"undefined-function-ptr-kind";
}

//.............................................................................

CFunctionPtrType::CFunctionPtrType ()
{
	m_TypeKind = EType_FunctionPtr;
	m_PtrTypeKind = EFunctionPtrType_Normal;
	m_Size = sizeof (TFunctionPtr);
	m_pTargetType = NULL;
	m_pMulticastType = NULL;
	m_pMcSnapshotType = NULL;
}

CMulticastType* 
CFunctionPtrType::GetMulticastType ()
{
	return m_pModule->m_TypeMgr.GetMulticastType (this);
}

CMcSnapshotType* 
CFunctionPtrType::GetMcSnapshotType ()
{
	return m_pModule->m_TypeMgr.GetMcSnapshotType (this);
}

CStructType* 
CFunctionPtrType::GetFunctionPtrStructType ()
{
	return m_pModule->m_TypeMgr.GetFunctionPtrStructType (m_pTargetType);
}

CStructType* 
CFunctionPtrType::GetFunctionPtrStructType_w ()
{
	return m_pModule->m_TypeMgr.GetFunctionPtrStructType_w (m_pTargetType);
}

rtl::CString
CFunctionPtrType::CreateSignature (
	CFunctionType* pFunctionType,
	EType TypeKind,
	EFunctionPtrType PtrTypeKind,
	uint_t Flags
	)
{
	rtl::CString Signature = TypeKind == EType_FunctionRef ? 'R' : 'P';

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

	if (Flags & EPtrTypeFlag_Nullable)
		Signature += 'n';

	Signature += pFunctionType->GetSignature ();
	return Signature;
}

rtl::CString
CFunctionPtrType::GetTypeModifierString ()
{
	if (!m_TypeModifierString.IsEmpty ())
		return m_TypeModifierString;

	if (m_Flags & EPtrTypeFlag_Nullable)
		m_TypeModifierString += "nullable ";

	if (m_PtrTypeKind != EFunctionPtrType_Normal)
	{
		m_TypeModifierString += GetFunctionPtrTypeKindString (m_PtrTypeKind);
		m_TypeModifierString += ' ';
	}

	ECallConv CallConv = m_pTargetType->GetCallConv ();
	if (CallConv)
	{
		m_TypeModifierString += GetCallConvString (CallConv);
		m_TypeModifierString += ' ';
	}

	return m_TypeModifierString;
}

void
CFunctionPtrType::PrepareTypeString ()
{
	m_TypeString = m_pTargetType->GetReturnType ()->GetTypeString ();
	m_TypeString += ' ';
	m_TypeString += GetTypeModifierString ();
	m_TypeString += m_TypeKind == EType_FunctionRef ? "function& " : "function* ";
	m_TypeString += m_pTargetType->GetArgString ();
}

void
CFunctionPtrType::PrepareLlvmType ()
{
	switch (m_PtrTypeKind)
	{
	case EFunctionPtrType_Normal:
		m_pLlvmType = GetFunctionPtrStructType ()->GetLlvmType ();
		break;

	case EFunctionPtrType_Weak:
		m_pLlvmType = GetFunctionPtrStructType_w ()->GetLlvmType ();
		break;

	case EFunctionPtrType_Thin:
	case EFunctionPtrType_Unsafe:
		m_pLlvmType = llvm::PointerType::get (m_pTargetType->GetLlvmType (), 0);
		break;

	default:
		ASSERT (false);
	}
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
