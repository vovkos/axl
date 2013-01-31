#include "stdafx.h"
#include "axl_jnc_DataPtrType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetDataPtrTypeKindString (EDataPtrType PtrTypeKind)
{
	switch (PtrTypeKind)
	{
	case EDataPtrType_Normal:
		return _T("safe");

	case EDataPtrType_Thin:
		return _T("thin");

	case EDataPtrType_Unsafe:
		return _T("unsafe");

	default:
		return _T("undefined-data-ptr-kind");
	}
}

//.............................................................................

CDataPtrType::CDataPtrType ()
{
	m_TypeKind = EType_DataPtr;
	m_PtrTypeKind = EDataPtrType_Normal;
	m_pDataType = NULL;
	m_Size = sizeof (TDataPtr);
}

CStructType* 
CDataPtrType::GetDataPtrStructType ()
{
	return m_pModule->m_TypeMgr.GetDataPtrStructType (m_pDataType);
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

	if (Flags & EPtrTypeFlag_Volatile)
		Signature += 'v';

	if (Flags & EPtrTypeFlag_Const)
		Signature += 'c';

	if (Flags & EPtrTypeFlag_NoNull)
		Signature += 'n';

	Signature += 'D';
	Signature += pBaseType->GetSignature ();
	return Signature;
}

void
CDataPtrType::PrepareTypeString ()
{
	m_TypeString = m_pDataType->GetTypeString ();

	if (m_PtrTypeKind != EClassPtrType_Normal)
	{
		m_TypeString += _T(' ');
		m_TypeString += GetDataPtrTypeKindString (m_PtrTypeKind);
	}

	if (m_Flags & EPtrTypeFlag_Volatile)
		m_TypeString += _T(" volatile");

	if (m_Flags & EPtrTypeFlag_Const)
		m_TypeString += _T(" const");

	if (m_Flags & EPtrTypeFlag_NoNull)
		m_TypeString += _T(" nonull");

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
		m_pLlvmType = llvm::PointerType::get (m_pDataType->GetLlvmType (), 0);
		break;

	default:
		ASSERT (false);
	}
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
