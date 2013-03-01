#include "stdafx.h"
#include "axl_jnc_DataPtrType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetDataPtrTypeKindString (EDataPtrType PtrTypeKind)
{
	static const tchar_t* StringTable [EDataPtrType__Count] = 
	{
		_T("safe"),   // EDataPtrType_Normal = 0,
		_T("thin"),   // EDataPtrType_Thin,
		_T("unsafe"), // EDataPtrType_Unsafe,
	};

	return (size_t) PtrTypeKind < EDataPtrType__Count ? 
		StringTable [PtrTypeKind] : 
		_T("undefined-data-ptr-kind");
}

//.............................................................................

CDataPtrType::CDataPtrType ()
{
	m_TypeKind = EType_DataPtr;
	m_PtrTypeKind = EDataPtrType_Normal;
	m_pTargetType = NULL;
	m_Size = sizeof (TDataPtr);
}

CStructType* 
CDataPtrType::GetDataPtrStructType ()
{
	return m_pModule->m_TypeMgr.GetDataPtrStructType (m_pTargetType);
}

rtl::CStringA
CDataPtrType::CreateSignature (
	CType* pBaseType,
	EType TypeKind,
	EDataPtrType PtrTypeKind,
	int Flags
	)
{
	rtl::CStringA Signature = TypeKind == EType_DataRef ? 'R' : 'P';

	switch (PtrTypeKind)
	{
	case EDataPtrType_Thin:
		Signature += 't';
		break;

	case EDataPtrType_Unsafe:
		Signature += 'u';
		break;
	}

	if (Flags & EPtrTypeFlag_Const)
		Signature += 'c';

	if (Flags & EPtrTypeFlag_Volatile)
		Signature += 'v';

	if (Flags & EPtrTypeFlag_Nullable)
		Signature += 'n';

	if (Flags & EPtrTypeFlag_Event)
		Signature += 'e';

	Signature += 'D';
	Signature += pBaseType->GetSignature ();
	return Signature;
}

void
CDataPtrType::PrepareTypeString ()
{
	m_TypeString += m_pTargetType->GetTypeString ();

	if (m_Flags & EPtrTypeFlag_Const)
		m_TypeString += _T(" const");

	if (m_Flags & EPtrTypeFlag_Volatile)
		m_TypeString += _T(" volatile");

	if (m_Flags & EPtrTypeFlag_Nullable)
		m_TypeString += _T(" nullable");

	if (m_Flags & EPtrTypeFlag_Event)
		m_TypeString += _T(" event");

	if (m_PtrTypeKind != EClassPtrType_Normal)
	{
		m_TypeString += _T(' ');
		m_TypeString += GetDataPtrTypeKindString (m_PtrTypeKind);
	}

	m_TypeString += m_TypeKind == EType_DataRef ? "&" : "*";
}

void
CDataPtrType::PrepareLlvmType ()
{
	switch (m_PtrTypeKind)
	{
	case EDataPtrType_Normal:
		m_pLlvmType = GetDataPtrStructType ()->GetLlvmType ();
		break;

	case EDataPtrType_Thin:
	case EDataPtrType_Unsafe:
		m_pLlvmType = m_pTargetType->GetTypeKind () != EType_Void ? 
			llvm::PointerType::get (m_pTargetType->GetLlvmType (), 0) :
			m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr)->GetLlvmType ();
		break;

	default:
		ASSERT (false);
	}
}

//.............................................................................

} // namespace jnc {
} // namespace axl {

